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

struct fbgc_memory_pool{
	uint8_t * data; 	//<! Actual data pointer which holds the pool buffer, all allocated objects live in this buffer
	uint8_t * tptr;		//<! Traveler pointer which moves only forward in the data buffer
	uint8_t * end; 		//<! Pointer holds the end of the pool, pool size can be extracted from pool_end - data
	size_t size;    	//<! Size of the pool
	struct fbgc_memory_pool * next; //<! Pools are connected to each other via linked list, each pool holds the next pool till NULL
};

struct fbgc_memory_block{
	struct fbgc_memory_pool * raw_memory_head; 	//<! Raw memory pools are held by this pointer as a linked list data structure
	struct fbgc_memory_pool * object_pool_head; //<! Object memory pools are held by this pointer as a linked list data structure
	struct fbgc_vector raw_memory_free_list;	//<! Raw memory free list as fbgc_vector
	struct fbgc_vector object_free_list; 		//<! Object free list as fbgc_vector
	size_t total_allocated_size;				//<! Total allocated size from the system heap, used to check condition for FBGC_MAX_MEMORY_SIZE
};

// /*! @details externed memory block and can only be defined once
static struct fbgc_memory_block fbgc_memb = {
	.raw_memory_head = NULL,
	.object_pool_head = NULL,
	.raw_memory_free_list = {0},
	.object_free_list = {0},
	.total_allocated_size = 0,
};

struct fbgc_static_memory_pool{
	uint8_t data [FBGC_STATIC_INTERNAL_MEMORY_SIZE];  //<! fbgc static memory in bytes
	uint8_t * tptr;	//<! Traveler pointer for new allocations
	uint8_t * end; 	//<! End of the static buffer which is the same data + FBGC_STATIC_INTERNAL_MEMORY_SIZE
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
	uint32_t mark_bit : 2;
	uint32_t capacity : 30;
	uint8_t data[0];
};

/*! @def cast_from_raw_memory_data_to_raw_memory(x)
    \brief cast from raw memory address to struct fbgc_raw_memory type
	\param x : pointer that is allocated via fbgc_malloc 
*/
#define cast_from_raw_memory_data_to_raw_memory(x)( (struct fbgc_raw_memory *)((uint8_t*)x-sizeof(struct fbgc_raw_memory))   )


struct __fbgc_mem_free_list_entry{
	void * start;
	void * end;
	size_t size;
};


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
struct fbgc_object * fbgc_malloc_object(size_t sz){
	return (struct fbgc_object *)_fbgc_malloc_from_object_pool(sz);
}

/*! @fn void * fbgc_malloc(size_t sz)
    \brief Allocates the requested size from raw memory pool, called by user side
    \details Similar to c-malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
void * fbgc_malloc(size_t sz){
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

#define OBJECT_FREE_LIST_INITIAL_SIZE 32
#define RAW_MEMORY_FREE_LIST_INITIAL_SIZE 64

void initialize_fbgc_memory_block(){
	//Allocates the pools of the internal memory
	fbgc_memb.raw_memory_head = new_fbgc_memory_pool(FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE);
	fbgc_memb.object_pool_head = new_fbgc_memory_pool(FBGC_DEFAULT_OBJECT_POOL_SIZE);

	//Inıtıalize free lists in the static buffer
	//In the static buffer we cannot use realloc, so these objects are really static and capacity must be checked all the time
	init_static_fbgc_vector_with_allocater(&fbgc_memb.object_free_list,OBJECT_FREE_LIST_INITIAL_SIZE, sizeof(struct __fbgc_mem_free_list_entry), &fbgc_malloc_static);
	init_static_fbgc_vector_with_allocater(&fbgc_memb.raw_memory_free_list,RAW_MEMORY_FREE_LIST_INITIAL_SIZE, sizeof(struct __fbgc_mem_free_list_entry), &fbgc_malloc_static);
	

	//
	// GC IS DISABLED 
	//

	//fbgc_gc.current_object_pool = fbgc_memb.object_pool_head;
	//fbgc_gc.current_raw_memory = fbgc_memb.raw_memory_head;

	
	//init_static_fbgc_queue(&fbgc_gc.tpe_queue, TRACEABLE_POINTER_LIST_INITIAL_CAPACITY,sizeof(struct traceable_pointer_entry));
	//Is there any easy way to mark this object ?
	//set_gc_dark(cast_from_raw_memory_data_to_raw_memory(fbgc_gc.tpe_queue.content));


	//Later allocate from internal memory not with malloc
	//fbgc_gc.ptr_list.data = (struct traceable_pointer_entry*) malloc(sizeof(struct traceable_pointer_entry)*TRACEABLE_POINTER_LIST_INITIAL_CAPACITY);
	
	//fbgc_gc.state = GC_STATE_INITIALIZED;
	//FBGC_LOGD(MEMORY,"Garbage collector initialized!\n");
}


void free_fbgc_memory_block(){	
	//Clean up evertyhing we have, give it to the system
	free_fbgc_memory_pool(fbgc_memb.raw_memory_head);
	free_fbgc_memory_pool(fbgc_memb.object_pool_head);

	fbgc_memb.raw_memory_head = NULL;
	fbgc_memb.object_pool_head = NULL;
	
	//free(fbgc_gc.ptr_list.data);
}

static void * _fbgc_malloc_from_object_pool(size_t size){
	FBGC_LOGV(MEMORY,"FBGC_MALLOC IS CALLED from object pool size :%lu\n",size);

	struct fbgc_memory_pool * pool = fbgc_memb.object_pool_head;
	
	do{
		FBGC_LOGV(MEMORY,"Requested memory from chunk: %lu, total memory : %lu,available mem: %lu \n", size, pool->size,(pool->end - pool->tptr));

		if( (ptrdiff_t)(pool->end - pool->tptr) >= (ptrdiff_t)size){
			//memory is available, give it and shift tptr
			void * ret_ptr = pool->tptr;
			pool->tptr += size; 	
			return ret_ptr;	
		}
		//iterate through the chunks
	}while(pool->next != NULL && (pool = pool->next) );
	
//#################################################################################################################
// Seek in object free list!!
//#################################################################################################################

	FBGC_LOGV(MEMORY,"There is no enough memory, new object pool allocation:\n");
	
	//if above code did not return succesfully, we need to allocate a new chunk!
	//Notice that iter shows the end of the chunks, we will use iter

	pool->next  = (struct fbgc_memory_pool *) new_fbgc_memory_pool(FBGC_DEFAULT_OBJECT_POOL_SIZE*((size_t)(size/(FBGC_DEFAULT_OBJECT_POOL_SIZE*1.0))+1));
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
			rb->mark_bit = GC_WHITE;
			rb->capacity = size;
			pool->tptr += sizeof(struct fbgc_raw_memory);
			//memory is available, give it and shift tptr
			void * ret_ptr = pool->tptr;
			pool->tptr += size;	
			return ret_ptr;	
		}
		//iterate through the chunks
	}while(pool->next != NULL && (pool = pool->next) );
	
//#################################################################################################################
// 
//	After writing GC algorithm uncomment below!	(See end of the file)
//
//#################################################################################################################

	//NEW_POOL_ALLOCATION:

	FBGC_LOGV(MEMORY,"There is no enough memory, new allocation raw memory pool\n");
	
	//if above code did not return succesfully, we need to allocate a new chunk!
	//Notice that iter shows the end of the chunks, we will use iter

	pool->next  = (struct fbgc_memory_pool *)new_fbgc_memory_pool(FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE*((size_t)(size/(FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE*1.0))+1));
	pool = pool->next;
	
	//calculate the multiplicity of the new chunk
	//only allow to allocate integer multiples of the page size
	//assume size = 111, FBGC_DEFAULT_POOL_SIZE = 20, new alloceted page size becomes 120, 
	
	FBGC_LOGV(MEMORY,"Allocated new pool size :%lu\n",pool->size);

	struct fbgc_raw_memory * rb =  (struct fbgc_raw_memory *)pool->data;
	rb->mark_bit = GC_WHITE;
	rb->capacity = size;
	pool->tptr = pool->data + requested_size;
	return ( pool->data + sizeof(struct fbgc_raw_memory) );
}

void * fbgc_realloc(void * ptr, size_t size){

/*
See thread : https://codereview.stackexchange.com/questions/151019/implementing-realloc-in-c

[Realloc requirements]
>> If the requested block size is smaller than the original size of the block, 
realloc either frees the unwanted memory at the end of the block and returns
the input pointer unchanged, or allocates a new block of the appropriate size,
frees the original block, and returns a pointer to this newly-allocated block.

>> If the requested block size is larger than the original size of the block,
realloc may allocate an expanded block at a new address and copy the contents of
the original block into the new location. In this case, a pointer to the expanded
block is returned, and the extended portion of the block is left uninitialized. Or, if possible,
it may expand the original block in-place and return the input pointer unchanged.

>>If realloc cannot satisfy a request to expand a block, it returns a null pointer
and does not free the original block. (realloc will always succeed
when you request to shrink a block.)

>>If the input pointer is null,
then realloc behaves exactly as if you had called malloc(size),
returning a pointer to the newly-allocated block of the requested size,
or a null pointer if the request could not be satisfied.

>>If the requested size is 0 and the input pointer is non-null,
then realloc behaves exactly as if you had called free(ptr),
and always returns a null pointer.

>>If the input pointer is null and the requested size is 0,
then the result is undefined!
*/
	if(ptr != NULL && !is_fbgc_raw_memory_pointer(ptr)){
		assert(0); //This is a programmer error not from fbgc side!
	}

	if(ptr == NULL) return fbgc_malloc(size);
	else if(size == 0) return NULL;

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
		rb->mark_bit = rb_old->mark_bit;
		rb_old->mark_bit = GC_WHITE;
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

	if(is_fbgc_raw_memory_pointer(ptr)){
		set_raw_memory_gc_white(ptr);
		//add to free list
		return;
	}
	set_gc_white((struct fbgc_object*)ptr);
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


//####################    Garbage Collector


/**
 * @brief fbgc Garbage collector states
 */
typedef enum {
	GC_STATE_NOT_INITIALIZED = 0,
	GC_STATE_READY_TO_SWEEP,
	GC_STATE_INITIALIZED,
	GC_STATE_PAUSED,
	GC_STATE_MARKING,
	GC_STATE_SWEEPING,
	GC_STATE_STOPPED
}GC_STATES;


struct fbgc_garbage_collector fbgc_gc = {
	.state = GC_STATE_NOT_INITIALIZED,
	.threshold = 100,
	.cycle = 0,
	.last_location = NULL,
	.current_raw_memory = NULL,
	.current_object_pool = NULL,
	.ptr_list = {.data = NULL, .size = 0, .capacity = 0},
};



uint8_t fbgc_gc_register_pointer(void * base_ptr,uint8_t nested, ...){
	
	//example when we implement this
	// uint8_t *** x = (uint8_t***)fbgc_malloc(sizeof(uint8_t**)*2);
	// x[0] = (uint8_t**)fbgc_malloc(sizeof(uint8_t*)*3);
	// x[1] = (uint8_t**)fbgc_malloc(sizeof(uint8_t*)*4);

	// for (int i = 0; i < 3; ++i)
	// {
	// 	x[0][i] = (uint8_t*)fbgc_malloc(sizeof(uint8_t)*2);
	// }
	// for (int i = 0; i < 4; ++i)
	// {
	// 	x[1][i] = (uint8_t*)fbgc_malloc(sizeof(uint8_t)*2);
	// }

	// fbgc_gc_register_pointer(x,2,sizeof(uint8_t**),sizeof(uint8_t*));
	//the idea is giving each nest sizeof pointers so we will guarantee to visit them one by one

	assert(nested < 3);

	printf("Registering ptr %p\n",base_ptr);

	void * dummy = base_ptr;
	struct fbgc_raw_memory * rb = cast_from_raw_memory_data_to_raw_memory(dummy);

	rb->mark_bit = GC_BLACK;
	uint8_t * ptr;

	va_list sizes;

	va_start(sizes,nested);

	for (int i = 0; i < nested; ++i){

		size_t bs = va_arg (sizes, size_t); 
		printf("Block sizes[%d] : %lu\n",i,bs);


		for (int a = 0; a < (rb->capacity/bs); ++a){
			ptr = rb->data+(a*bs);
			void ** temp = (void**)ptr;
			cprintf(100,"[%p] will be visited \n",ptr);
			//fbgc_gc_register_pointer(*temp,nested-1,bs);
		}



		// for (int j = 0; j < tpe->size; ++j){
		// 	cprintf(100,"[%d]:[%p] will be visited \n",i,base);
		// 	base += x;
		// }		
	}

	va_end(sizes);

	//struct traceable_pointer_list * list = &fbgc_gc.ptr_list;

	/*const size_t it = list->size;
	struct traceable_pointer_entry * tpe = &(list->data[it]);
	tpe->base_ptr = (uint8_t*)base_ptr;
	tpe->tptr = tpe->base_ptr;
	tpe->size = size;
	tpe->block_size = block_size;
	tpe->offset = offset;

	fbgc_gc.ptr_list.size++;

	if(fbgc_gc.ptr_list.size > fbgc_gc.ptr_list.capacity){
		//allocate some memory!
		;
	}

	//Print possible pointers that we are gonna visit
	uint8_t * base = tpe->base_ptr + tpe->offset;
	for (int i = 0; i < tpe->size; ++i){
		printf("[%d]:[%p] will be visited \n",i,base);
		base += tpe->block_size;
	}*/

	//Check overflow!
	return 1;
}

void fbgc_gc_mark_object(struct fbgc_object * obj){
	set_gc_gray(obj);
}

uint8_t fbgc_gc_mark_pointer(void * base_ptr, size_t block_size){

	if(fbgc_gc.state == GC_STATE_NOT_INITIALIZED){
		FBGC_LOGE("GC not started\n");
		return 0;
	}

	struct traceable_pointer_entry tpe;

	if(is_fbgc_raw_memory_pointer(base_ptr)){
		tpe.base_ptr = cast_from_raw_memory_data_to_raw_memory(base_ptr);
		tpe.tptr = (uint8_t*)base_ptr;
		tpe.block_size = block_size;
		//tpe.which_memory_pool = MEM_RAW_MEMORY_POOL;
 		FBGC_LOGD(MEMORY,"Marking new ptr Base ptr:%p | block_size:%ld | capacity:%d \n",base_ptr,block_size,((struct fbgc_raw_memory*)tpe.base_ptr)->capacity);
	}
	else{

		tpe.base_ptr = base_ptr;
		tpe.tptr = (uint8_t*)base_ptr;
		tpe.block_size = block_size; // !!!! not a block size anymore, this is array length
		//tpe.which_memory_pool = MEM_OBJECT_POOL;
	}
	
	//First check that can gc mark this right now
	//For this we will check its threshold value to where we are at the pause, if there is no time to mark all the pointers
	//We should put this in a queue to make marking at the next stages
	//Checking gc first is trying not to overflow queue of pending pointers
	
	push_fbgc_queue(&fbgc_gc.tpe_queue,&tpe);
	return 1;
}


void fbgc_gc_init(struct fbgc_object * root){
	
	//Next pointer of the root in the heap
	if(root){
		fbgc_gc.last_location = (uint8_t*)root + size_of_fbgc_object(root);	
	}
	else{
		fbgc_gc.last_location = fbgc_memb.object_pool_head->data;
	}

	if(root){
		set_gc_black(root);
		gc_mark_fbgc_object(root);		
	}

}

void gc_update_free_list(struct fbgc_object ** obj){
	struct fbgc_memory_pool * mp = fbgc_gc.current_object_pool;
}



void fbgc_gc_mark(){

	if(fbgc_gc.state == GC_STATE_NOT_INITIALIZED){
		FBGC_LOGE("GC not started\n");
		return;
	}

	FBGC_LOGD(MEMORY,"Starting marking phase\n");

	fbgc_gc.state = GC_STATE_MARKING;

	uint8_t * ptr = fbgc_gc.last_location;

	while(fbgc_gc.cycle < fbgc_gc.threshold){
		
		//first check is there any pending request in the queue?
		struct fbgc_queue * queue = &(fbgc_gc.tpe_queue);

		while(!is_empty_fbgc_queue(queue)){
			
			FBGC_LOGD(MEMORY,"Pending queue is not empty!\n");

			struct traceable_pointer_entry * tpe = (struct traceable_pointer_entry *) front_fbgc_queue(queue);
			uint8_t * tpe_end = NULL;
			if(is_fbgc_raw_memory_pointer(tpe->base_ptr)){
				
				struct fbgc_raw_memory * rb = (struct fbgc_raw_memory *)tpe->base_ptr;
				tpe_end = (uint8_t*)tpe->base_ptr + rb->capacity;
				set_gc_gray(rb);


				printf("TPE :%p | base :%p | end:%p | at the end:%d\n",tpe,tpe->base_ptr,tpe_end,tpe->tptr == tpe_end);
				
				while(fbgc_gc.cycle < fbgc_gc.threshold){

					FBGC_LOGV(MEMORY,"Founded ptr %p is in raw buffer pool\n",tpe->tptr);
					assert(tpe->tptr);

					++fbgc_gc.cycle;

					if(tpe->tptr == NULL){
						printf("NULL!\n");
					}

					//allocated with fbgc_malloc
					struct fbgc_object ** obj = (struct fbgc_object **) tpe->tptr;
					FBGC_LOGV(MEMORY,"fbgc_object Address: %p\n",*obj);

					if(*obj && !is_fbgc_object_pointer(*obj)){
						FBGC_LOGE("Something wrong!\n");
						return;
					}

					if(*obj != NULL && is_gc_white(*obj) ){
						printf("Setting %p to gray\n",*obj );
						set_gc_black(*obj);
						gc_mark_fbgc_object(*obj);
						
						//gc_update_free_list(obj);

						/*if(gc_mark_fbgc_object(*obj) == 0){
							if((uint8_t*)(*obj) < fbgc_gc.last_location){
								set_gc_black(*obj);
							}
						}*/
					}
					tpe->tptr += tpe->block_size;

					printf("TPE tptr :%p\n",tpe->tptr );

					if(tpe->tptr >= tpe_end){
						FBGC_LOGV(MEMORY,"End of %p, setting to black!\n",tpe->base_ptr);
						set_gc_black(rb);
						pop_fbgc_queue(queue);
						printf("Popped %p\n",tpe );
						//finished this object, pop it from the queue
						break;
					}	
				}
			}
			else{
				struct fbgc_object * obj = (struct fbgc_object * )tpe->base_ptr;
				tpe_end = (uint8_t*)tpe->base_ptr + size_of_fbgc_object(obj) * tpe->block_size;

				while(fbgc_gc.cycle < fbgc_gc.threshold){
					obj = (struct fbgc_object * )tpe->tptr;
					FBGC_LOGV(MEMORY,"Founded ptr %p is  in object pool \n",tpe->tptr);
					set_gc_gray(obj);
					gc_mark_fbgc_object(obj);

					tpe->tptr += size_of_fbgc_object(obj);

					if(tpe->tptr >= tpe_end){
						FBGC_LOGV(MEMORY,"End of %p, setting to black!\n",tpe->base_ptr);
						set_gc_black((struct fbgc_object * )tpe->tptr);
						pop_fbgc_queue(queue);
						printf("Popped object %p\n",tpe );
						//finished this object, pop it from the queue
						break;
					}
				}

				
				
			}
				
			
			if(fbgc_gc.cycle >= fbgc_gc.threshold){
				//set cycle to zero, we finished this marking phase, next phase will start where we left from
				fbgc_gc.cycle = 0;
				goto end_of_fbgc_gc_mark;
			}
		}
		
		if(ptr >= (uint8_t*)(fbgc_gc.current_object_pool->tptr)) break;

		//Now queue is somehow finished and there is cycle available to run marking for the objects
		struct fbgc_object * obj = (struct fbgc_object * ) ptr;

		if(is_gc_gray(obj)){
			FBGC_LOGW("Address is GRAY! :%p\n",ptr );
			set_gc_black(obj);

			FBGC_LOGV(MEMORY,"Setting %p to black\n",ptr );
		}
		else{
			FBGC_LOGW("Address is not gray! :%p | color :%d\n",ptr,obj->mark_bit );
		}

		ptr += size_of_fbgc_object((struct fbgc_object*)ptr);
	}

	end_of_fbgc_gc_mark:

	if(ptr < (uint8_t*)(fbgc_gc.current_object_pool->tptr)){
		fbgc_gc.state = GC_STATE_PAUSED;
		fbgc_gc.last_location = ptr;
	}
	else{
		fbgc_gc.state = GC_STATE_READY_TO_SWEEP;	
	}
}


void fbgc_gc_sweep(){
	if(fbgc_gc.state == GC_STATE_READY_TO_SWEEP){
		struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;

		//set_gc_white(cast_from_raw_memory_data_to_raw_memory(fbgc_memb.free_list.content));
		//erase_fbgc_vector(&fbgc_memb.free_list);

		while(iter != NULL){
			uint8_t * ptr = (uint8_t*)iter->data;
			while(ptr != iter->tptr){
				struct fbgc_object * obj = (struct fbgc_object*)ptr;
				if(is_gc_white(obj)){
					//push_back_fbgc_vector(&fbgc_memb.free_list,obj);
				}
				ptr += size_of_fbgc_object((struct fbgc_object*)ptr);
			}
			iter = iter->next;
		}
		fbgc_gc.state = GC_STATE_INITIALIZED;
	}
}


void fbgc_gc_run(){
	fbgc_gc_mark();
	fbgc_gc_sweep();
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

			cprintf(101,"Chunk byte size:%ld , color:%d\n",ptr->capacity,ptr->mark_bit);
			
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