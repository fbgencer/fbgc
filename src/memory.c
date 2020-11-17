// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"

#define FBGC_MALLOC_OPTIMIZATION FBGC_MALLOC_OPTIMIZATION_FOR_MEMORY

// /*! @details fbgc memory block and can only be defined once
static struct fbgc_memory_block fbgc_memb = {
	.raw_memory_head = NULL,
	.object_pool_head = NULL,
	.raw_memory_free_list = {0},
	.object_free_list = {0},
	.total_allocated_size = 0,
};

/*! @details fbgc internal memory holder, defined as a static variable and lives for the entire life cycle of the fbgc */
static struct fbgc_static_memory_pool __fbgc_static_internal_memory = {
	.data = {0},
	.tptr = __fbgc_static_internal_memory.data,
	.end = __fbgc_static_internal_memory.data + FBGC_STATIC_INTERNAL_MEMORY_SIZE
};


/*! 
	@details fbgc raw memory holder, outside of this file never used, only for casting the pointer, we always return data pointer to the user 
	must be cast by using (struct fbgc_raw_memory *)((uint8_t*)data - sizeof(struct fbgc_raw_memory))
*/
struct fbgc_raw_memory{
	uint32_t capacity;
	uint8_t data[0];
};

/*! @def cast_from_raw_memory_data_to_raw_memory(x)
    \brief cast from raw memory address to struct fbgc_raw_memory type
	\param x : pointer that is allocated via fbgc_malloc 
*/
#define cast_from_raw_memory_data_to_raw_memory(x)( (struct fbgc_raw_memory *)((uint8_t*)x-sizeof(struct fbgc_raw_memory))   )

/*! @fn static void * _fbgc_malloc_from_object_pool(size_t size)
    \brief Allocates the requested size from object pool
    \details Only the allocation of fbgc_objects must be called with this function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
static void * _fbgc_malloc_from_object_pool(size_t size);

/*! @fn static void * _fbgc_malloc_from_raw_memory_pool(size_t size)
    \brief Allocates the requested size from raw memory pool
    \details Similar to c-malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
static void * _fbgc_malloc_from_raw_memory_pool(size_t size);

/*! @fn struct fbgc_object * fbgc_malloc_object(size_t sz)
    \brief Allocates the requested size from object pool, called by used side
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
inline struct fbgc_object * fbgc_malloc_object(size_t sz){
	return (struct fbgc_object *)_fbgc_malloc_from_object_pool(sz);
}

/*! @fn void * fbgc_malloc(size_t sz)
    \brief Allocates the requested size from raw memory pool, called by user side
    \details Similar to c-malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/

size_t allocation = 0;
inline void * fbgc_malloc(size_t sz){
	allocation += sz;
	return _fbgc_malloc_from_raw_memory_pool(sz);
}

/*! @def is_fbgc_object_pointer(ptr)
    \brief Checks the given pointer lies in object pool or not
    \param ptr : Void pointer
*/
#define is_fbgc_object_pointer(ptr)( check_pointer_is_in_pool(fbgc_memb.object_pool_head,ptr))

/*! @def is_fbgc_raw_memory_pointer(ptr)
    \brief Checks the given pointer lies in raw memory pool or not
    \param ptr : Void pointer
*/
#define is_fbgc_raw_memory_pointer(ptr)( check_pointer_is_in_pool(fbgc_memb.raw_memory_head,ptr))

static void claim_pointer_and_arrange_free_list(struct fbgc_vector * v, void * ptr, size_t size);

static struct fbgc_garbage_collector fbgc_gc = {
	.max_cycle = FBGC_GC_MAXIMUM_CYCLE,
	.cycle = 0,
	.stack_bottom = NULL,
	.state = GC_STATE_NOT_INITIALIZED,
	.pools_scan_finished = false,
	.cstack_scan_finished = false,
	.free_list_is_ready = false,
};

static void print_free_list(struct fbgc_vector * v);

/*! @fn static struct fbgc_memory_pool * new_fbgc_memory_pool(size_t pg_size)
    \brief Allocated a new pool by using c-malloc
    \param size : Requested pool size in bytes
    \return Allocated pool address
*/
static struct fbgc_memory_pool * new_fbgc_memory_pool(size_t pg_size){

	/*
		This is the only function fbgc allocates from the system by using malloc,
		we never use malloc other than allocation of pools.
		If system has a constraint on memory it is checked here, otherwise we expect that malloc may return NULL
		fbgc allocators never return NULL, thus there is no need to check everytime if the requested data by the allocators is NULL.
		If new pool cannot be allocated then this functions throws an error and halts the program
	*/

	FBGC_LOGD(MEMORY,"New pool request size:%lu\n",pg_size);
	FBGC_LOGW("New pool request size:%lu\n",pg_size);

	if(FBGC_MAX_MEMORY_SIZE && (fbgc_memb.total_allocated_size+pg_size) >= FBGC_MAX_MEMORY_SIZE){
		FBGC_LOGE("fbgc run out of memory\n");
		//@TODO: implement memory handler and close the program do not use asserts
		assert(0);
	}

	struct fbgc_memory_pool * mp = (struct fbgc_memory_pool *) malloc(sizeof(struct fbgc_memory_pool));
	//@TODO: implement memory handler and close the program
	assert(mp != NULL); 
	mp->data = (uint8_t*)calloc(pg_size,1);
	//@TODO: implement memory handler and close the program
	assert(mp->data != NULL);
	mp->tptr = mp->data;
	mp->size = pg_size;
	mp->end = mp->data + pg_size;
	mp->next = NULL;
	return mp;
}

static struct fbgc_memory_pool * new_fbgc_memory_pool_add_into_free_list(size_t pg_size,struct fbgc_vector * freel){
	struct fbgc_memory_pool * mp = new_fbgc_memory_pool(pg_size);
	struct __fbgc_mem_free_list_entry fle = {0};
	fle.start = mp->data;
	fle.end = mp->end;
	fle.size = mp->size;
	push_back_fbgc_vector(freel,&fle);
	return mp;
}

static inline struct fbgc_memory_pool * new_fbgc_object_pool(size_t pg_size){
	return new_fbgc_memory_pool_add_into_free_list(pg_size,&fbgc_memb.object_free_list);
}
static inline struct fbgc_memory_pool * new_fbgc_raw_memory_pool(size_t pg_size){
	return new_fbgc_memory_pool_add_into_free_list(pg_size,&fbgc_memb.raw_memory_free_list);
}

static void free_fbgc_memory_pool(struct fbgc_memory_pool * mp){
	while(mp != NULL){
		free(mp->data);
		struct fbgc_memory_pool * temp = mp->next;
		free(mp);
		mp = temp;
	}	
}

static uint8_t check_pointer_is_in_pool(const struct fbgc_memory_pool * iter, const void * ptr){
	while(iter != NULL){
		const uint8_t * start = (uint8_t*)iter->data;
		const uint8_t * end = (uint8_t*)(iter->data) + iter->size;
		if((uint8_t*)ptr >= start && (uint8_t*)ptr <= end) return 1;
		iter = iter->next;
	}
	return 0;
}

//Do not use this function unless it is a debug.
static uint8_t check_pointer_align_in_object_pool(const void * ptr){
	const struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;
	while(iter != NULL){
		const uint8_t * const start = (uint8_t*)iter->data;
		const uint8_t * const end = (uint8_t*)(iter->tptr);
		if((uint8_t*)ptr >= start && (uint8_t*)ptr <= end){
			//Ok ptr is in the pool, scan from the beginning (so exhausting)
			for(const uint8_t * it = start; it <= end;){
				if(it == NULL)	
					break;
				if(it == ptr) return 1;	
				it += size_of_fbgc_object((struct fbgc_object*)it);
			}
		};
		iter = iter->next;
	}
	return 0;
}
//Do not use this function unless it is a debug.
static uint8_t check_pointer_align_in_raw_memory_pool(const void * ptr){
	const struct fbgc_memory_pool * iter = fbgc_memb.raw_memory_head;
	while(iter != NULL){
		const uint8_t * const start = (uint8_t*)iter->data;
		const uint8_t * const end = (uint8_t*)(iter->tptr);
		if((uint8_t*)ptr >= start && (uint8_t*)ptr <= end){
			//Ok ptr is in the pool, scan from the beginning (so exhausting)
			for(const uint8_t * it = start; it <= end;){
				if(it == NULL)	
					break;
				struct fbgc_raw_memory * rb = (struct fbgc_raw_memory *) it;
				if(rb->data == ptr) return 1;
				it += (rb->capacity + sizeof(struct fbgc_raw_memory));
			}
		};
		iter = iter->next;
	}
	return 0;
}


#define OBJECT_FREE_LIST_INITIAL_SIZE 32
#define RAW_MEMORY_FREE_LIST_INITIAL_SIZE 64

void initialize_fbgc_memory_block(){
	//Initialize free lists in the static buffer
	//In the static buffer we cannot use realloc, so these objects are really static and capacity must be checked all the time

	init_static_fbgc_vector_with_allocater(&fbgc_memb.object_free_list,OBJECT_FREE_LIST_INITIAL_SIZE, sizeof(struct __fbgc_mem_free_list_entry), &fbgc_malloc_static);
	init_static_fbgc_vector_with_allocater(&fbgc_memb.raw_memory_free_list,RAW_MEMORY_FREE_LIST_INITIAL_SIZE, sizeof(struct __fbgc_mem_free_list_entry), &fbgc_malloc_static);
	
	//Allocates the pools of the internal memory
	fbgc_memb.raw_memory_head = new_fbgc_raw_memory_pool(FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE);
	fbgc_memb.object_pool_head = new_fbgc_object_pool(FBGC_DEFAULT_OBJECT_POOL_SIZE);
}


void free_fbgc_memory_block(){	
	//Clean up evertyhing we have, give it to the system
	free_fbgc_memory_pool(fbgc_memb.raw_memory_head);
	free_fbgc_memory_pool(fbgc_memb.object_pool_head);

	fbgc_memb.raw_memory_head = NULL;
	fbgc_memb.object_pool_head = NULL;
}

static void * _fbgc_malloc_from_object_pool(size_t size){
	FBGC_LOGV(MEMORY,"FBGC_MALLOC IS CALLED from object pool size :%lu\n",size);

	struct fbgc_memory_pool * pool = fbgc_memb.object_pool_head;
	
	#if FBGC_MALLOC_OPTIMIZATION == FBGC_MALLOC_OPTIMIZATION_FOR_MEMORY
	//Remove second condition, we will remove tptr anyway
		if(fbgc_gc.free_list_is_ready && pool->tptr != pool->data){
			struct fbgc_vector * v = &fbgc_memb.object_free_list;
			//Traverse back
			for (int i = size_fbgc_vector(v)-1; i>=0; --i){
				struct __fbgc_mem_free_list_entry * fle = (struct __fbgc_mem_free_list_entry *) at_fbgc_vector(v,i);
				int32_t diff = (fle->size - size);
				FBGC_LOGV(MEMORY,"checking fle->size :%ld,diff:%d\n",fle->size,diff);
				//Do not divide if divided space is too small to fit at least one int object, or already smaller than the size
				if(diff == 0 || diff >= (int32_t)sizeof(struct fbgc_int_object)){
					FBGC_LOGV(MEMORY,"Requested memory found in the free list size:%ld | ptr:%p\n", size,fle->start);
					//exact match or a good match so we can divide easily this portion
					void * return_ptr = fle->start;
					claim_pointer_and_arrange_free_list(v,return_ptr,size);
					print_free_list(v);
					return return_ptr;
				}
			}
		}
	#endif


	do{
		FBGC_LOGV(MEMORY,"Requested memory from chunk: %lu, total memory : %lu,available mem: %lu \n", size, pool->size,(pool->end - pool->tptr));

		if( (ptrdiff_t)(pool->end - pool->tptr) >= (ptrdiff_t)size){
			//memory is available, give it and shift tptr
			void * ret_ptr = pool->tptr;
			pool->tptr += size; 	
			return ret_ptr;	
		}
		//iterate through the chunks
		if(pool->next == NULL)
			break;
		pool = pool->next;
	}while(1);
	
//#################################################################################################################
// Seek in object free list!!
//#################################################################################################################

	FBGC_LOGV(MEMORY,"There is no enough memory, new object pool allocation:\n");
	
	//if above code did not return succesfully, we need to allocate a new chunk!
	//Notice that iter shows the end of the chunks, we will use iter

	pool->next  = (struct fbgc_memory_pool *) new_fbgc_object_pool(FBGC_DEFAULT_OBJECT_POOL_SIZE*((size_t)(size/(FBGC_DEFAULT_OBJECT_POOL_SIZE*1.0))+1));
	pool = pool->next;
	//calculate the multiplicity of the new chunk
	//only allow to allocate integer multiples of the page size
	//assume size = 111, FBGC_DEFAULT_POOL_SIZE = 20, new alloceted page size becomes 120, 
	FBGC_LOGV(MEMORY,"Allocated new pool size :%lu\n",pool->end - pool->data);
	pool->tptr = pool->data + size;
	//Just return the beginning of the pool
	return pool->data;
}

static void * _fbgc_malloc_from_raw_memory_pool(size_t size){
	FBGC_LOGV(MEMORY,"FBGC_MALLOC IS CALLED RAW MEMORY pool size :%lu\n",size);
	
	struct fbgc_memory_pool * pool = fbgc_memb.raw_memory_head;

	size_t requested_size = size+sizeof(struct fbgc_raw_memory);

	do{
		FBGC_LOGV(MEMORY,"Requested memory from chunk: %lu, total memory : %lu,available mem: %lu \n",
				size,pool->size,(pool->end - pool->tptr));
		
		//Before calculating the space also include the byte size of the chunk
		if( (ptrdiff_t)(pool->end - pool->tptr) >= (ptrdiff_t)requested_size){
			
			struct fbgc_raw_memory * rb =  (struct fbgc_raw_memory *)pool->tptr;
			rb->capacity = size;
			//memory is available, give it and shift tptr
			pool->tptr += requested_size;
			return rb->data;
		}
		//iterate through the chunks
		if(pool->next == NULL)
			break;

		pool = pool->next;
	}while(1);
	
//#################################################################################################################
// 
//	After writing GC algorithm uncomment below!	(See end of the file)
//
//#################################################################################################################

	//NEW_POOL_ALLOCATION:

	FBGC_LOGV(MEMORY,"There is no enough memory, new allocation raw memory pool\n");
	
	//if above code did not return succesfully, we need to allocate a new chunk!
	//Notice that iter shows the end of the chunks, we will use iter

	pool->next  = (struct fbgc_memory_pool *)new_fbgc_raw_memory_pool(FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE*((size_t)(size/(FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE*1.0))+1));
	pool = pool->next;
	
	//calculate the multiplicity of the new chunk
	//only allow to allocate integer multiples of the page size
	//assume size = 111, FBGC_DEFAULT_POOL_SIZE = 20, new alloceted page size becomes 120, 
	
	FBGC_LOGV(MEMORY,"Allocated new pool size :%lu\n",pool->size);

	struct fbgc_raw_memory * rb =  (struct fbgc_raw_memory *)pool->data;
	rb->capacity = size;
	pool->tptr = pool->data + requested_size;
	return ( pool->data + sizeof(struct fbgc_raw_memory) );
}

void * fbgc_realloc(void * ptr, size_t size){
	/*
		See thread : https://codereview.stackexchange.com/questions/151019/implementing-realloc-in-c
	*/
	if(ptr != NULL && !is_fbgc_raw_memory_pointer(ptr)){
		assert(0); //This is a programmer error not from fbgc side!
	}

	if(ptr == NULL) return fbgc_malloc(size); //If the input pointer is NULL realloc behaves like malloc | malloc(0) is system dependent, we do not have protection!
	else if(size == 0) return NULL; //If size is zero realloc return NULL

	//Realloc cannot be call for objects we need to make sure it must be come from raw memory pointer

	struct fbgc_raw_memory * rb_old = cast_from_raw_memory_data_to_raw_memory(ptr);

	size_t old_capacity = rb_old->capacity;

	FBGC_LOGV(MEMORY,"Requested memory in realloc is %lu, block size : %ld\n",size,old_capacity);
	 
	if(size > old_capacity){
		FBGC_LOGV(MEMORY,"Allocating new block size : %lu and copying\n",size);
		
		if(fbgc_memb.raw_memory_head->tptr == (uint8_t*)(ptr)+old_capacity ){
			fbgc_memb.raw_memory_head->tptr += size-old_capacity;
			struct fbgc_raw_memory * rb = cast_from_raw_memory_data_to_raw_memory(ptr);
			rb->capacity = size;
			return ptr;
		}
		void * new_mem_ptr = fbgc_malloc(size);
		struct fbgc_raw_memory * rb = cast_from_raw_memory_data_to_raw_memory(new_mem_ptr);

		if(new_mem_ptr == NULL) return NULL;
		FBGC_LOGV(MEMORY,"succesfully allocated size : %lu,old address:%p new address:%p\n",size,rb_old,rb);

		memcpy(new_mem_ptr,ptr,old_capacity);
		FBGC_LOGV(MEMORY,"New buffer capacity : %d\n",rb->capacity);
		//Do we need it ?
		//MARK TO WHITE
		fbgc_free(ptr);
		return new_mem_ptr;
	}

	//if the requested size is smaller than the previous size just decrease the size of the block, let the gc handle later
	rb_old->capacity = size;
	return ptr;	
}

void fbgc_free(void *ptr){
	if(!ptr) return;

	// if(is_fbgc_raw_memory_pointer(ptr)){
	// 	set_raw_memory_gc_white(ptr);
	// 	//add to free list
	// 	return;
	// }
	//set_gc_white((struct fbgc_object*)ptr);
	//push_back_fbgc_vector(&fbgc_memb.free_list,ptr);
}

/*! @fn static void * __fbgc_malloc_static_internal(size_t size, bool is_raw_memory)
    \brief Allocated from fbgc internal static buffer
	\details fbgc_object or any other data type can be allocated with this function. 
		These object never collected by the garbage collector nor they die. They live until the end of the program
    \param size : Requested size in bytes
	\param is_raw_memory : If the requested byte is not for fbgc_object types then this parameter must be true, otherwise it must be false
    \return Allocated address of the requested bytes
*/
static void * __fbgc_malloc_static_internal(size_t size, bool is_raw_memory){

	size_t requested_size = size;
	if(is_raw_memory){
		requested_size += sizeof(struct fbgc_raw_memory);
	}
	if( (__fbgc_static_internal_memory.end - __fbgc_static_internal_memory.tptr) >= requested_size){
		if(is_raw_memory){
			struct fbgc_raw_memory * rb =  (struct fbgc_raw_memory *)__fbgc_static_internal_memory.tptr;
			rb->capacity = size;
			__fbgc_static_internal_memory.tptr += sizeof(struct fbgc_raw_memory);
			//memory is available, give it and shift tptr
			void * ret_ptr = __fbgc_static_internal_memory.tptr;
			__fbgc_static_internal_memory.tptr += size;	
			return ret_ptr;	
		}
		else{
			//memory is available, give it and shift tptr
			void * ret_ptr = __fbgc_static_internal_memory.tptr;
			__fbgc_static_internal_memory.tptr += size; 	
			return ret_ptr;	
		}
	}
	//We shouldn't be here, if we are then we need to increase our static size
	assert(0);
}

void * fbgc_malloc_static(size_t size){
	return __fbgc_malloc_static_internal(size,true);
}

void * fbgc_malloc_object_static(size_t size){
	return __fbgc_malloc_static_internal(size,false);
}

static inline uint32_t capacity_in_bytes_fbgc_raw_memory(void * ptr){
	return cast_from_raw_memory_data_to_raw_memory(ptr)->capacity;
}

uint32_t capacity_fbgc_raw_memory(void * ptr,size_t block_size){
	return capacity_in_bytes_fbgc_raw_memory(ptr)/block_size;	
}


//####################    Garbage Collector   ####################################

static void claim_fbgc_object_and_arrange_free_list(struct fbgc_object * ptr);
static void claim_fbgc_raw_memory_and_arrange_free_list(struct fbgc_raw_memory * rb);

void fbgc_gc_init(struct fbgc_object * root, void * stack_bottom){
	fbgc_gc.state = GC_STATE_INITIALIZED;
	fbgc_gc.stack_bottom = stack_bottom;
	
	init_static_fbgc_queue_with_allocater(&fbgc_gc.tpe_queue, TRACEABLE_POINTER_LIST_INITIAL_CAPACITY,sizeof(struct traceable_pointer_entry),&fbgc_malloc_static);
	FBGC_LOGD(MEMORY,"Garbage collector initialized!\n");	

	if(root){
		gc_mark_fbgc_object(root);
		if(is_fbgc_object_pointer(root)){
			claim_fbgc_object_and_arrange_free_list(root);
		}
	}
}

static int compare_free_list__(const void * a, const void * b){
	const struct __fbgc_mem_free_list_entry * flea = (struct __fbgc_mem_free_list_entry *)a;
	const struct __fbgc_mem_free_list_entry * fleb = (struct __fbgc_mem_free_list_entry *)b;
	//Reverse sorting
  	return ( fleb->size - flea->size );
}
static void claim_pointer_and_arrange_free_list(struct fbgc_vector * v, void * ptr, size_t size){

	//@TODO if free list_entry size is zero we cannot remove it..
	uint8_t * p1 = (uint8_t*)ptr, * p2 = (uint8_t*) ptr+size;
	FBGC_LOGD(MEMORY,"Claimed pointer{%p:%p}:%ld\n",p1,p2,size);


	for(size_t i = 0; i<size_fbgc_vector(v); ++i){
		struct __fbgc_mem_free_list_entry * fle = (struct __fbgc_mem_free_list_entry *) at_fbgc_vector(v,i);
		if(fle->size == 0) continue; //Remove this we do not need to check size==0, they must be removed!!!
		
		bool add_sector = ((uint8_t*)fle->start < p1) && ((uint8_t*)fle->end>p2);
		//FBGC_LOGD(MEMORY,"[%lu] fle->start:%p, fle->end:%p,add_sector = %d\n",i,fle->start,fle->end,add_sector);
		if(add_sector){
			void * old_fle_end = fle->end;
			fle->end = p1;
			fle->size = (uint8_t*)fle->end-(uint8_t*)fle->start;
			if(p2 != old_fle_end){
				struct __fbgc_mem_free_list_entry temp = {0};
				temp.start = p2;
				temp.end = old_fle_end;
				temp.size =  (uint8_t*)old_fle_end -(uint8_t*)p2;
				FBGC_LOGD(MEMORY,"Adding sector[%p-%p]size:%lu\n",temp.start,temp.end,temp.size);
				push_back_fbgc_vector(v,&temp);
			}
			break;
		}
		else if(fle->start == p1){
			fle->start = p2;
			fle->size = (uint8_t*)fle->end-(uint8_t*)fle->start;
			FBGC_LOGD(MEMORY,"Updating sector[%p-%p] to p2 size %lu\n",fle->start,fle->end,fle->size);
			break;
		}
		else if(fle->end == p2){
			fle->end = p1;
			fle->size = (uint8_t*)fle->end-(uint8_t*)fle->start;
			FBGC_LOGD(MEMORY,"Updating sector[%p-%p] to p2 size %lu\n",fle->start,fle->end,fle->size);
			break;
		}
	}
	if(size_fbgc_vector(v) > 0){
		sort_fbgc_vector(v,compare_free_list__);
		struct __fbgc_mem_free_list_entry * fleback = (struct __fbgc_mem_free_list_entry *) back_fbgc_vector(v);
		if(fleback->size == 0){
		//This is not safe but if we call pop_back it may call realloc however this is from static memory		
			--v->size; 
		}
	}
}

static void print_free_list(struct fbgc_vector * v){
	for(size_t i = 0; i<size_fbgc_vector(v); ++i){
		struct __fbgc_mem_free_list_entry * fle = (struct __fbgc_mem_free_list_entry *) at_fbgc_vector(v,i);
		cprintf(101,"[%ld]Garbage[%p - %p] {%lu}\n",i,fle->start,fle->end,fle->size);
	}
}
static void claim_fbgc_object_and_arrange_free_list(struct fbgc_object * ptr){
	claim_pointer_and_arrange_free_list(&fbgc_memb.object_free_list,ptr,size_of_fbgc_object(ptr));
	
}
static void claim_fbgc_raw_memory_and_arrange_free_list(struct fbgc_raw_memory * rb){
	claim_pointer_and_arrange_free_list(&fbgc_memb.raw_memory_free_list,rb,rb->capacity + sizeof(struct fbgc_raw_memory));
}

static void internal_fbgc_gc_trace_stack(void){
/*
	Before calling this function drop all the registers into stack so we can trace them
	void * (*volatile _mark_stack)(void*) = fbgc_gc_trace_stack;
    jmp_buf ctx;
    memset(&ctx, 0, sizeof(jmp_buf));
    setjmp(ctx);
    void * bb = _mark_stack(__builtin_frame_address(0));
*/
    void * tos = __builtin_frame_address(0);
	void * bos = fbgc_gc.stack_bottom;
	if(tos>bos){
		void * temp = bos;
		bos = tos;
		tos = temp;
	}
	char * p = (char*) tos;
	while(p <= ((char*) bos - sizeof(void*))){
        //printf("Data adr : maybe %p\n",p);
        void * data_maybe = *(void **)p;
        if(data_maybe) {
			//cprintf(011,"Data maybe %p \n",data_maybe);
			if(is_fbgc_raw_memory_pointer(data_maybe)){
				FBGC_LOGV(MEMORY,"Raw memory found :%p\n",data_maybe);
				if(check_pointer_align_in_raw_memory_pool(data_maybe)){
					struct fbgc_raw_memory * rb = cast_from_raw_memory_data_to_raw_memory(data_maybe);
					claim_fbgc_raw_memory_and_arrange_free_list(rb);
					if(rb->capacity >= sizeof(void*)) //Ok at least it can hold a pointer
						fbgc_gc_mark_pointer(rb->data,1);
				}
				else{
					FBGC_LOGV(MEMORY,"Alignment problem, this is a bug!!\n");
				}
				
			}
			else if(is_fbgc_object_pointer(data_maybe)){
				FBGC_LOGV(MEMORY,"Object found :%p\n",data_maybe);
				claim_fbgc_object_and_arrange_free_list(data_maybe);
				gc_mark_fbgc_object(data_maybe);
			}
			//Most of the time we have the else situation
        }
		++p;
		//p += sizeof(void*);
    }
	fbgc_gc.cstack_scan_finished = true;
}

static void fbgc_gc_trace_cstack(){
	//Dump registers into stack
	void (*volatile _mark_stack)(void) = internal_fbgc_gc_trace_stack;
	// Dumping registers creates bugs in the memory..
    jmp_buf myjump;
    memset(&myjump, 0, sizeof(jmp_buf));
    setjmp(myjump);
    _mark_stack();	
}



static void fbgc_gc_trace_pools(){
	if(fbgc_gc.state == GC_STATE_NOT_INITIALIZED){
		_FBGC_LOGE("GC not started\n");
		return;
	}	
	/*
		Fbgc has two pools and both of them can show reference to each other
		So we could have 4 possible situations to trace
		1) a fbgc_object has reference to another fbgc_object (object_pool -> object_pool) (ie range object : start and end is another fbgc object)
		2) a fbgc_object has reference to raw memory (object_pool -> raw_memory_pool)(ie tuple object : tuple content is a c array in the raw memory pool)
		3) a raw memory has reference to fbgc_object (raw_memory_pool -> object_pool)(ie tuple object : tuple contents hold fbgc_object pointers)
		4) a raw memory has reference to raw_memory (raw_memory_pool -> raw_memory_pool)

		Roots of the fbgc are the main_field object, fbgc-interpreter stack and stack frames in C.
		When we have a fbgc object we ask its fbgc_gc function to assign its references into traceable entry queue
		1 and 2 is covered by object member function : gc_mark_fbgc_object(obj) 
		We do not make a guess here and it is totally precise, we ask the object about its references and object claims ownership
		3 is also easy
	
	*/
	fbgc_gc.state = GC_STATE_SCANNING_POOLS;

	//first check is there any pending request in the queue?
	struct fbgc_queue * queue = &(fbgc_gc.tpe_queue);

	while(!is_empty_fbgc_queue(queue)){
		if(fbgc_gc.cycle == 0){
			//This scan phase is finished because of the cycle counter, we just return and wait for the next run
			FBGC_LOGD(MEMORY,"Phase is finished, gc cycle is zero\n");
			FBGC_LOGD(MEMORY,"Pending queue size %ld\n",size_fbgc_queue(queue));
			break; //Break the queue loop we just finished this phase of scanning traces
		}
		FBGC_LOGD(MEMORY,"Pending queue is not empty!\n");
		//We do not take entries queue directly, we just take the front reference and change its tptr because if the array is traced too big to scan it once
		//we do not want to stop-the-world approach, instead we try to stop the world frequently but with small stop scan times
		struct traceable_pointer_entry * tpe = (struct traceable_pointer_entry *) front_fbgc_queue(queue);
		uint8_t * tpe_end = NULL;

		if(is_fbgc_object_pointer(tpe->base_ptr)){
			struct fbgc_object * obj = (struct fbgc_object * )tpe->tptr; //Traveler is also starts from the base
			tpe_end = (uint8_t*)tpe->base_ptr + size_of_fbgc_object(obj) * tpe->block_size;
			FBGC_LOGV(MEMORY,"Founded ptr %p is in object pool\n",tpe->tptr);
			do{
				--fbgc_gc.cycle; _FBGC_LOGV(MEMORY,"--cycle\n");
				//Instead of calling gc_gray(obj) claim object as owned by root and let the free list be changed
				claim_fbgc_object_and_arrange_free_list(obj);
				gc_mark_fbgc_object(obj); //Allow object to put its own references to the queue (if object has no mark function throws no error!)
				tpe->tptr += size_of_fbgc_object(obj);
				if(tpe->tptr >= tpe_end){
					FBGC_LOGV(MEMORY,"End of object:%p, popping from the queue\n",tpe->base_ptr);
					//We have finished this object remove the object from the queue
					pop_fbgc_queue(queue); 
					break; //Break the cycle loop and check for a new queue item
				}
			}while(fbgc_gc.cycle);
		}
		else if(is_fbgc_raw_memory_pointer(tpe->base_ptr)){
			struct fbgc_raw_memory * rb = (struct fbgc_raw_memory *)tpe->base_ptr;
			tpe_end = (uint8_t*)tpe->base_ptr + rb->capacity + sizeof(struct fbgc_raw_memory);
			claim_fbgc_raw_memory_and_arrange_free_list(rb);
			FBGC_LOGV(MEMORY,"Founded ptr %p is in raw_memory pool | finishes at %p\n",tpe->base_ptr,tpe_end);
			do{
				FBGC_LOGV(MEMORY,"Traveler ptr %p | %p\n",tpe->tptr,tpe_end);
				//That is OK, pointer could be NULL, just shift and look for the new references
				if(tpe->tptr != NULL){
					--fbgc_gc.cycle; _FBGC_LOGV(MEMORY,"--cycle\n");
					void * ptr = *(void **)tpe->tptr; //We will dereference this pointer and check its location
					FBGC_LOGV(MEMORY,"Dereferencing tpe ptr : %p, to %p\n",tpe->tptr,ptr);
					//So this pointer where does it live ?
					if(is_fbgc_object_pointer(ptr)){
						FBGC_LOGV(MEMORY,"Raw buffer has reference to fbgc_object %p\n",ptr);
						//So situation 3) arises here, internal buffer holds reference to fbgc object
						struct fbgc_object * object = (struct fbgc_object *) ptr;
						if(object != NULL){
							claim_fbgc_object_and_arrange_free_list(object);
							gc_mark_fbgc_object(object);
						}
					}
					else if(is_fbgc_raw_memory_pointer(ptr)){
						FBGC_LOGV(MEMORY,"Raw buffer has reference to raw_buffer %p\n",ptr);
						//So situation 4) arises here, internal buffer holds reference to another pointer
						//HOWEVER this could be a misunderstanding and we just referenced a data : conservative gc approach

						//@TODO: We should not use this function, instead validate pointer with something
						if(check_pointer_align_in_raw_memory_pool(ptr)){
							struct fbgc_raw_memory * maybe_raw_memory = cast_from_raw_memory_data_to_raw_memory(ptr);
							//So this is really a pointer inside the raw memory pool
							claim_fbgc_raw_memory_and_arrange_free_list(maybe_raw_memory);
							if(maybe_raw_memory->capacity >= sizeof(void*))
								fbgc_gc_mark_pointer(maybe_raw_memory->data,sizeof(void*));
						}
					}
				}
				tpe->tptr += tpe->block_size;
				FBGC_LOGV(MEMORY,"Traveler ptr %p is iterated\n",tpe->tptr);
				if(tpe->tptr >= tpe_end){
					FBGC_LOGV(MEMORY,"End of object:%p, popping from the queue\n",tpe->base_ptr);
					//We have finished this buffer remove the buffer from the queue
					pop_fbgc_queue(queue);
					break; //Break the cycle loop and check for a new queue item
				}
			}while(fbgc_gc.cycle);
		}
	}

	if(is_empty_fbgc_queue(queue)){
		//So we scanned everything in the pools
		fbgc_gc.pools_scan_finished = true;
		FBGC_LOGV(MEMORY,"Left cycle %ld\n",fbgc_gc.cycle);
	}
}

void fbgc_gc_mark_pointer(void * base_ptr, size_t block_size){
	//fbgc_gc_mark_pointer((cast_fbgc_object_as_tuple(self)->content),sizeof(struct fbgc_object *));
	if(fbgc_gc.state == GC_STATE_NOT_INITIALIZED){
		FBGC_LOGE("GC not started\n");
	}
	struct traceable_pointer_entry tpe = {0};

	if(is_fbgc_raw_memory_pointer(base_ptr)){
		tpe.base_ptr = cast_from_raw_memory_data_to_raw_memory(base_ptr);
		tpe.tptr = (uint8_t*)base_ptr;
		tpe.block_size = block_size;
 		FBGC_LOGD(MEMORY,"Adding raw memory pointer into tpe queue real base:%p|Base:%p|bsize:%ld|cap:%d \n",tpe.base_ptr,base_ptr,block_size,((struct fbgc_raw_memory*)tpe.base_ptr)->capacity);
	}
	else if(is_fbgc_object_pointer(base_ptr)){
		tpe.base_ptr = base_ptr;
		tpe.tptr = (uint8_t*)base_ptr;
		tpe.block_size = block_size; // !!!! not a block size anymore, this is array length | block sizes are just sizeof(struct fbgc_object*)
		FBGC_LOGD(MEMORY,"Adding object array into tpe queue Base:%p|size:%ld\n",base_ptr,block_size);
	}
	else{
		assert(0);
	}
	//First check that can gc mark this right now
	//For this we will check its threshold value to where we are at the pause, if there is no time to mark all the pointers
	//We should put this in a queue to make marking at the next stages
	//Checking gc first is trying not to overflow queue of pending pointers
	

	struct traceable_pointer_entry * tpe_front = NULL;
	
	while(iterator_fbgc_queue_front_to_back(&fbgc_gc.tpe_queue, (void**)&tpe_front)){
		//cprintf(001,"Comparing frontbase:%p, tpe.base:%p\n",tpe_front->base_ptr,tpe.base_ptr);
		if(tpe_front->base_ptr == tpe.base_ptr){
			//FBGC_LOGE("CATCHED\n");
			return;
		}
	}

	FBGC_LOGV(MEMORY,"TPE Pushed base %p\n",tpe.base_ptr);
	push_fbgc_queue(&fbgc_gc.tpe_queue,&tpe);
	
	// void * dummy = NULL;
	// FBGC_LOGW("Checking what we pushed\n");
	// while(iterator_fbgc_queue_front_to_back(&fbgc_gc.tpe_queue, &dummy)){
	// 	struct traceable_pointer_entry * front = (struct traceable_pointer_entry *)dummy;
	// 	cprintf(001,"...frontbase:%p, tpe.base:%p\n",front->base_ptr,tpe.base_ptr);
	// }	
}




// void fbgc_gc_sweep(){
// 	if(fbgc_gc.state == GC_STATE_READY_TO_SWEEP){
// 		struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;

// 		//set_gc_white(cast_from_raw_memory_data_to_raw_memory(fbgc_memb.free_list.content));
// 		//erase_fbgc_vector(&fbgc_memb.free_list);

// 		while(iter != NULL){
// 			uint8_t * ptr = (uint8_t*)iter->data;
// 			while(ptr != iter->tptr){
// 				struct fbgc_object * obj = (struct fbgc_object*)ptr;
// 				if(is_gc_white(obj)){
// 					//push_back_fbgc_vector(&fbgc_memb.free_list,obj);
// 				}
// 				ptr += size_of_fbgc_object((struct fbgc_object*)ptr);
// 			}
// 			iter = iter->next;
// 		}
// 		fbgc_gc.state = GC_STATE_INITIALIZED;
// 	}
// }


void fbgc_gc_run(size_t run_cycle){

	FBGC_LOGV(MEMORY,"---------Garbage collector will start scanning cycle:%ld--------\n\n",run_cycle);

	// if(!fbgc_gc.cstack_scan_finished){
	// 	FBGC_LOGD(MEMORY,"Starting ctack scanning.\n");
	// 	fbgc_gc_trace_cstack();
	// }	
	if(!fbgc_gc.pools_scan_finished){
		FBGC_LOGD(MEMORY,"\n........Starting ctack scanning.........\n");
		fbgc_gc_trace_cstack();
		FBGC_LOGD(MEMORY,"\n........Starting pool scanning.........\n");
		if(run_cycle > fbgc_gc.max_cycle)
			run_cycle = fbgc_gc.max_cycle;
		fbgc_gc.cycle = run_cycle;
		fbgc_gc_trace_pools();
		//If we cannot finish the scanning of heap we cannot trust old stack scan, we need to scan it always
	}
	//&& fbgc_gc.cstack_scan_finished
	if(fbgc_gc.pools_scan_finished){
		fbgc_gc.free_list_is_ready = true;
		FBGC_LOGV(MEMORY,"Dumping info of free lists\n");
		FBGC_LOGV(MEMORY,"Object list\n");
		print_free_list(&fbgc_memb.object_free_list);
		FBGC_LOGV(MEMORY,"Raw memory list\n");
		print_free_list(&fbgc_memb.raw_memory_free_list);
	}
}


void print_fbgc_memory_object_pool(){
	cprintf(011,"############[OBJECT POOL]############\n");
	struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;

	//just to clear obj locations
	
	for(int i = 0; iter != NULL; i++){
		cprintf(111,"[%d.Chunk] size: %d\n",i,iter->size);
		size_t obj_start = 0;
		for(int j = 0; j<iter->size; j++){
			int val = *( uint8_t *)((iter->data)+j); 

			if(j == obj_start && ((uint8_t *)iter->data+j) != iter->tptr){
				struct fbgc_object * dummy = (struct fbgc_object *)((uint8_t*)(iter->data)+j);
				obj_start += size_of_fbgc_object(dummy);
				//print the type of the data in the memory
				cprintf(101,">>[%s] obj size in memory %d | color %d\n",objtp2str(dummy),size_of_fbgc_object(dummy),dummy->mark_bit);				
				//print the address and the data in the memory!
				//cprintf(101,"[%d][%p]:%0x",j,((iter->data)+j),val);

			}
			//else
			{
	   			cprintf(010,"[%d][%p]",j,((iter->data)+j));
	   			cprintf(110,":%0x = %u",(uint8_t)val,(uint8_t)val);
	   			
			}
			

	   		if(iter->tptr == iter->data+j){
	   			cprintf(100," <--tptr");
	   			cprintf(100,"\n");
	   			break;
	   		} 	

	   		cprintf(010,"\n");
		}
		cprintf(111,"[%d.] Chunk size: %d\n",i,iter->size);
		iter = iter->next;
    } 		
}

void print_fbgc_memory_raw_memory(){
	cprintf(011,"############[RAW BUFFER]############\n");
	struct fbgc_memory_pool * iter = fbgc_memb.raw_memory_head;


	//loop for raw buffer pages
	for(int i = 0; iter != NULL; i++){
		cprintf(111,"[%d.Chunk] size: %d\n",i,iter->size);

		size_t chunk_filled_size = iter->tptr - iter->data;
		printf("%ld chunk filled\n",chunk_filled_size );
		size_t obj_start = 0;
		for(size_t j = 0; j< chunk_filled_size;){

			uint8_t * where = (uint8_t*)(iter->data);
			where += j;
			
	   		struct fbgc_raw_memory * ptr = (struct fbgc_raw_memory*)where;

			cprintf(101,"Chunk byte size:%ld\n",ptr->capacity);
			
			long end = ptr->capacity+sizeof(struct fbgc_raw_memory);
			long lastj = j;
			

			uint8_t * data = (uint8_t*)ptr;
			for(long k = 0; k < end;  ++k){
				cprintf(010,"[%d][%p]",lastj+k,data);
	   			cprintf(110,":0x%02x = %02u",(uint8_t)*data,(uint8_t)*data);
	   			cprintf(010,"\n");
	   			++data;
			}
			j += end;
		}
		cprintf(100," <---tptr\n");
		cprintf(111,"[%d.] Chunk size: %d\n",i,iter->size);
		iter = iter->next;
    }
}


void print_fbgc_memory_block(){

	cprintf(010,"############[MEMORY_BLOCK]############\n");
	
	print_fbgc_memory_object_pool();
	print_fbgc_memory_raw_memory();

	
	cprintf(010,"############[MEMORY_BLOCK]############\n");
}


/*
	#ifdef MEM_DEBUG
		cprintf(111,"Running garbage collector\n");
	#endif	

	//Start from head, sweep inside all objects until the end of the chunk,
	//if chunk is not available search for the new chunks.

	//mark(); // run mark phase

	#ifdef MEM_DEBUG
		cprintf(111,"Creating free chunk linked list\n");
	#endif	

	struct fbgc_memory_pool * opool_iter2 = fbgc_memb.object_pool_head;
	struct fbgc_object * chunk_iter = &fbgc_memb.empty_chunk_head; //this time get the head of the chunk!

	while(opool_iter2 != NULL){
	
		struct fbgc_object * current_obj = opool_iter2->data;
		opool_iter2->tptr = opool_iter2->data;
		while(current_obj != NULL && current_obj <= (opool_iter2->data + opool_iter2->size) ){
			
			#ifdef MEM_DEBUG
			cprintf(100,"$$$$$$$$$$$$$$$$$\n");
			cprintf(111,"Searching inside object pool\n");
			#endif

			//Find size, it tells location for the next object
			size_t obj_size = get_fbgc_object_size(current_obj);

			#ifdef MEM_DEBUG
			cprintf(111,"Going forward in object pool.. go to +%p!, current_obj->type:%d\n",(char*)current_obj+obj_size,current_obj->type);
			#endif
			
			if(current_obj->type == UNKNOWN || obj_size == 0){//&& 
				#ifdef MEM_DEBUG
					cprintf(111,"obj size = 0 or unknown obje in the memory ! change the memory pool!\n");
					cprintf(111,"It is possible that after this point memory was not filled. So try to put new object\n");
				#endif	

				//this part can be optimized. basically, we are calling the first three lines in this function
				if( (opool_iter2->data + opool_iter2->size - opool_iter2->tptr) >= size){
					#ifdef MEM_DEBUG
						cprintf(111,"Requested memory is available after free chunk..\n");
					#endif

					opool_iter2->tptr += size; 	
					return (opool_iter2->tptr - size);		
				}		

				break;
			}	

			if( get_fbgc_object_mark_bit(current_obj->type) == 0){
				#ifdef MEM_DEBUG
					cprintf(111,"Object has no owner found!\n");
				#endif					
				chunk_iter->next = current_obj;
				chunk_iter = chunk_iter->next;
			}
			else {
				
				opool_iter2->tptr = current_obj;
				#ifdef MEM_DEBUG
					cprintf(111,"Object has owner, move tptr from %p to %p!\n",opool_iter2->tptr,opool_iter2->tptr+obj_size);
				#endif	

				opool_iter2->tptr += obj_size; 
			}

			current_obj = (char*) current_obj + obj_size;


			//optimize this part, somehow it is required but we can predict this
			if(current_obj >= ((char *)opool_iter2->data + opool_iter2->size)){
				#ifdef MEM_DEBUG
					cprintf(100,"We hit end of the memory!\n");
				#endif				
				break;
			}

			#ifdef MEM_DEBUG
			cprintf(100,"#########################\n");
			#endif
		}
		
		opool_iter2 = opool_iter2->next;
		#ifdef MEM_DEBUG
		cprintf(100,"#########################\n");
		#endif
	}


	#ifdef MEM_DEBUG
		cprintf(111,"Checking empty chunks\n");
	#endif	

	//### change this void type!
	chunk_iter = &fbgc_memb.empty_chunk_head; 
	
	while(chunk_iter->next != NULL){
		
		size_t available_size = get_fbgc_object_size(chunk_iter->next);

		#ifdef MEM_DEBUG
		cprintf(110,"available size :%d from empty chunk, requested :%d \n",available_size,size);
		#endif

		if(size == available_size){
			//chunk has enough space as we wanted
			// make the linked list connection proper and return the address
			
			#ifdef MEM_DEBUG
			cprintf(101,"Size == available_size\n");
			#endif
			struct fbgc_object * temp = chunk_iter->next;
			chunk_iter->next = temp->next;
			return temp;
		}
		else if(size < available_size && (available_size - size) >= sizeof(struct fbgc_garbage_object) ){
			//Size of this chunk is bigger than what we wanted
			//we will give the address but we will divide space into two pieces to put gb object

			#ifdef MEM_DEBUG
			cprintf(101,"Size < available_size, we need to create gc object!\n");
			cprintf(101,"Empty space starts at %p\n",chunk_iter->next);
			cprintf(101,"Garbage starts at %p\n",(char*)(chunk_iter->next) + size);
			#endif


			struct fbgc_garbage_object * gb = (struct fbgc_garbage_object *)((char *) (chunk_iter->next) + size);

			gb->base.type = GARBAGE;
			struct fbgc_object * temp = chunk_iter->next;
			gb->base.next = temp->next;
			gb->size =  (available_size - size);
			
			#ifdef MEM_DEBUG
			cprintf(101,"Garbage size %d, garbage next :%p\n",gb->size,gb->base.next);
			#endif

			return temp;				 
		}
		
		chunk_iter = chunk_iter->next;
	}
*/

/*
		[FBGC Malloc algorithm]
		
		In this section, there are different situations to allocate memory efficiently.

		Requested size : size, available size will be asize 

		Memory structure contains object memory pools which are allocated spaces in the heap
		fbgc_memory_pool contains *data,*tptr, size and next pointer.
		Different pools are connected each other like a single linked list with the next pointer.
		Size holds the size of the memory pool. Each pool creates a single pool which is called as object pool in fbgc.

		This algorithm allocates new spaces, runs gc and so on.

		1.Start iterating inside memory pools. Check their size and compare it with the requested size.
			While iterating in each list we will check the following condition.			
			o) Requested size is smaller than the object pool, 
				Calculate the empty space that we have by using tptr(traveler)
				if we have enough space return the pointer of this space	
					[---------------------------------|                  ]
					^						    ^			   ^
				memory_pool_data                memory_pool_tptr    memory_pool_data + memory_pool_size 

			o) We iterated in each memory pools and requested size is bigger than any of them.
			Here new allocation takes place and due to the state we directly use goto for allocation.

			o) Requested size is not bigger than the memory pools. However, there is no enough space to give the requsted.
			We will check our empty lists, which is the list from previous garbage collection running. If there is enough space in there 
			no need to run gc in this moment.
			If this also fails, the worst case scenario happens for this function: running garbage collector!
			Garbage collector will run, basically it will tell all the variables to mark their objects
			We will sweep in each part of the memory pools one by one to create empty memory lists.
			Again empty memory list will be searched and if there is NO space we will allocate new memory.
			New memory is basically another memory pool for the requested size.

	*/