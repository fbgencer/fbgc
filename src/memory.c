#include "fbgc.h"



struct fbgc_memory_block fbgc_memb = {	.raw_buffer_head = NULL,
										.object_pool_head = NULL,
										.empty_chunk_head = {.type=NIL}
									};


static struct fbgc_memory_pool * new_fbgc_memory_pool(uint8_t tp, size_t pg_size){

	FBGC_LOGD(MEMORY,"New pool request type:%u, size:%lu\n",tp,pg_size);

	struct fbgc_memory_pool * mp = (struct fbgc_memory_pool *) malloc(sizeof(struct fbgc_memory_pool));
	assert(mp != NULL);
	mp->type = tp;
	mp->data =  calloc(pg_size,1);	
	assert(mp->data != NULL);
	mp->tptr = mp->data;
	mp->size = pg_size;
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


void initialize_fbgc_memory_block(){
	//first allocate a big chunck that contains both inital buffer and object pool
	fbgc_memb.raw_buffer_head = new_fbgc_memory_pool(MEM_RAW_BUFFER,PAGE_SIZE);
	fbgc_memb.object_pool_head = new_fbgc_memory_pool(MEM_OBJECT_POOL,PAGE_SIZE);


	/*fbgc_memb.raw_buffer_head  =  (struct fbgc_memory_pool *) malloc(sizeof(struct fbgc_memory_pool));	
	assert(fbgc_memb.raw_buffer_head != NULL);
	fbgc_memb.raw_buffer_size = 1;

	fbgc_memb.object_pool_head  =  (struct fbgc_memory_pool *) malloc(sizeof(struct fbgc_memory_pool));	
	assert(fbgc_memb.object_pool_head != NULL);
	fbgc_memb.object_pool_size = 1;

	fbgc_memb.object_pool_head->data =  calloc(PAGE_SIZE,1);	
	assert(fbgc_memb.object_pool_head->data != NULL);
	fbgc_memb.object_pool_head->tptr = fbgc_memb.object_pool_head->data;
	fbgc_memb.object_pool_head->size = PAGE_SIZE;
	fbgc_memb.object_pool_head->next = NULL;*/

}


static void * _fbgc_malloc(struct fbgc_memory_pool * opool_iter, size_t size){
	FBGC_LOGV(MEMORY,"FBGC_MALLOC IS CALLED size :%lu\n",size);
	
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
				Calculate the empty space that we have by using tptr(traverser)
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
	
	do{
		//check the size, if it is already too big go to new pool allocation
		//if(size > opool_iter->size) goto NEW_POOL_ALLOCATION;
		
		FBGC_LOGV(MEMORY,"Requested memory from chunk: %lu, total memory : %lu,available mem: %lu \n",
				size,opool_iter->size,(opool_iter->data + opool_iter->size - opool_iter->tptr));
		
		
		if(opool_iter->type == MEM_RAW_BUFFER){
			//Before calculating the space also include the byte size of the chunk
			if( ((uint8_t*)opool_iter->data + opool_iter->size - (uint8_t*)opool_iter->tptr) >= (size+sizeof(struct fbgc_raw_buffer))){
				FBGC_LOGV(MEMORY,"memory is given from raw buffer\n");
				struct fbgc_raw_buffer * rb =  (struct fbgc_raw_buffer *)opool_iter->tptr;
				rb->mark_bit = GC_WHITE;
				rb->capacity = size;
				opool_iter->tptr += sizeof(struct fbgc_raw_buffer);
				//memory is available, give it and shift tptr
				void * ret_ptr = opool_iter->tptr;
				opool_iter->tptr += size; 	
				return ret_ptr;	
			}				
		}
		else{
			if( ((uint8_t*)opool_iter->data + opool_iter->size - (uint8_t*)opool_iter->tptr) >= size){

				//memory is available, give it and shift tptr
				void * ret_ptr = opool_iter->tptr;
				opool_iter->tptr += size; 	
				return ret_ptr;	
			}	
		}	
		if(opool_iter->next == NULL) break;
		//iterate through the chunks
		opool_iter = opool_iter->next;
	}while(1);
	
//#################################################################################################################
// 
//	After writing GC algorithm uncomment below!	(See end of the file)
//
//#################################################################################################################

	//NEW_POOL_ALLOCATION:

	FBGC_LOGV(MEMORY,"There is no enough memory, new allocation opool type :%d\n",opool_iter->type);
	
	//if above code did not return succesfully, we need to allocate a new chunk!
	//Notice that iter shows the end of the chunks, we will use iter

	opool_iter->next  = (struct fbgc_memory_pool *)
		new_fbgc_memory_pool(opool_iter->type,PAGE_SIZE*((size_t)(size/(PAGE_SIZE*1.0))+1));

	opool_iter = opool_iter->next;
	
	//calculate the multiplicity of the new chunk
	//only allow to allocate integer multiples of the page size
	//assume size = 111, page_size = 20, new alloceted page size becomes 120, 
	
	FBGC_LOGV(MEMORY,"Allocated new pool size :%lu\n",opool_iter->size);

	if(opool_iter->type == MEM_RAW_BUFFER){
		struct fbgc_raw_buffer * rb =  (struct fbgc_raw_buffer *)opool_iter->data;
		rb->mark_bit = GC_WHITE;
		rb->capacity = size;
		opool_iter->tptr = opool_iter->data + size + sizeof(struct fbgc_raw_buffer);
		return ( opool_iter->data + sizeof(struct fbgc_raw_buffer) );
	}

	
	opool_iter->tptr = opool_iter->data + size;
	return opool_iter->data;
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
	if(ptr == NULL) return fbgc_malloc(size);

	long block_size = capacity_in_bytes_fbgc_raw_memory(ptr);

	
	FBGC_LOGV(MEMORY,"~~~~~~~Realloc~~~~~~~~\n");
	FBGC_LOGV(MEMORY,"Requested memory in realloc is %lu, block size : %ld\n",size,block_size);
	

	if(size > block_size){
		FBGC_LOGV(MEMORY,"Allocating new block size : %lu and copying\n",size);
	
		void * new_mem_ptr = fbgc_malloc(size);
		if(new_mem_ptr == NULL) return NULL;
		FBGC_LOGV(MEMORY,"succesfully allocated size : %lu,old address:%p new address:%p\n",size,ptr,new_mem_ptr);
		memcpy(new_mem_ptr,ptr,block_size);
		//Do we need it ?
		fbgc_free(ptr);

		return new_mem_ptr;
	}

	//if the requested size is smaller than the previous size just decrease the size of the block, let the gc handle later

	long * byte_size =  (long *)((uint8_t*)ptr - sizeof(long));
	*byte_size = size;
	
	return ptr;	
}

void fbgc_free(void *ptr){

	/*
		Free function is NOT a c-free function in fbgc.
		It does not give allocated memory to the system, only clears the object to make it unrecognizable.
		In order to do that just delete the objects type, make it garbage, so for the later use we can use this empty memory.
	*/

	//##########################
	// Make the connections proper for the garbage objects.
	/*struct fbgc_garbage_object * gb = (struct fbgc_garbage_object *) ptr;

	gb->size =  get_fbgc_object_size(ptr);
	gb->base.type = GARBAGE;*/
}



void * fbgc_malloc_object(size_t sz){
	return _fbgc_malloc(fbgc_memb.object_pool_head,sz);
}
void * fbgc_malloc(size_t sz){
	return _fbgc_malloc(fbgc_memb.raw_buffer_head,sz);
}

void * fbgc_malloc_static(size_t sz){
	uint8_t * ptr = (uint8_t*)_fbgc_malloc(fbgc_memb.raw_buffer_head,sz);
	if(ptr){
		struct fbgc_raw_buffer * rb = (struct fbgc_raw_buffer*)(ptr-sizeof(struct fbgc_raw_buffer));
		set_gc_dark(rb);
	}
	return(void*)ptr;
}


void print_fbgc_memory_object_pool(){
	cprintf(011,"############[OBJECT POOL]############\n");
	struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;

	//just to clear obj locations
	
	for(int i = 0; iter != NULL; i++){
		cprintf(111,"[%d.Chunk] size: %d\n",i,iter->size);
		size_t obj_start = 0;
		for(int j = 0; j<iter->size; j++){
			int val = *( char *)((iter->data)+j); 

			if(j == obj_start && ((char *)iter->data+j) != iter->tptr){
				struct fbgc_object * dummy = (struct fbgc_object *)((uint8_t*)(iter->data)+j);
				obj_start += get_fbgc_object_size(dummy);
				//print the type of the data in the memory
				cprintf(101,">>[%s] obj size in memory %d\n",objtp2str(dummy),get_fbgc_object_size(dummy));				
				//print the address and the data in the memory!
				//cprintf(101,"[%d][%p]:%0x",j,((iter->data)+j),val);


			}
			//else
			{
	   			cprintf(010,"[%d][%p]",j,((iter->data)+j));
	   			cprintf(110,":%0x = %u",(uint8_t)val,(uint8_t)val);
	   			
			}
			

	   		if(iter->tptr == ((char *)iter->data+j)){
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

void print_fbgc_memory_raw_buffer(){
	cprintf(011,"############[RAW BUFFER]############\n");
	struct fbgc_memory_pool * iter = fbgc_memb.raw_buffer_head;


	//loop for raw buffer pages
	for(int i = 0; iter != NULL; i++){
		cprintf(111,"[%d.Chunk] size: %d\n",i,iter->size);

		size_t chunk_filled_size = iter->tptr - iter->data;
		printf("%ld chunk filled\n",chunk_filled_size );
		size_t obj_start = 0;
		for(size_t j = 0; j< chunk_filled_size;){

			uint8_t * where = (uint8_t*)(iter->data);
			where+=j;
	   		struct fbgc_raw_buffer * ptr = (struct fbgc_raw_buffer*)where;

			cprintf(101,"Chunk byte size:%ld , color:%d\n",ptr->capacity,ptr->mark_bit);
			
			long end = ptr->capacity+sizeof(struct fbgc_raw_buffer);
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
	print_fbgc_memory_raw_buffer();

	
	cprintf(010,"############[MEMORY_BLOCK]############\n");
}


void free_fbgc_memory_block(){
	
	free_fbgc_memory_pool(fbgc_memb.raw_buffer_head);
	free_fbgc_memory_pool(fbgc_memb.object_pool_head);

	fbgc_memb.raw_buffer_head = NULL;
	fbgc_memb.object_pool_head = NULL;

	free(fbgc_gc.ptr_list.data);
}



long capacity_in_bytes_fbgc_raw_memory(void * x){
	void * ptr  = (uint8_t*)x - sizeof(long);
	long byte_size = *( (long*)((uint8_t *)ptr) );
	return byte_size;
}



//####################    Garbage Collector

struct fbgc_garbage_collector fbgc_gc = {
	.state = GC_STATE_NOT_STARTED,
	.threshold = 10,
	.last_location = NULL,
	.current_raw_buffer = NULL,
	.current_object_pool = NULL,
	.ptr_list = {.data = NULL, .size = 0, .capacity = 0}
};

uint8_t fbgc_gc_register_pointer(void * base_ptr,size_t size, size_t block_size, size_t offset){
	printf("Registering ptr %p\n",base_ptr);

	struct traceable_pointer_list * list = &fbgc_gc.ptr_list;

	const size_t it = list->size;
	struct traceable_pointer_entry * tpe = &(list->data[it]);
	tpe->ptr = base_ptr;
	tpe->size = size;
	tpe->block_size = block_size;
	tpe->offset = offset;

	fbgc_gc.ptr_list.size++;

	if(fbgc_gc.ptr_list.size > fbgc_gc.ptr_list.capacity){
		//allocate some memory!
		;
	}


	//Print possible pointers that we are gonna visit
	uint8_t * base = (uint8_t*)(tpe->ptr) + tpe->offset;
	for (int i = 0; i < tpe->size; ++i){
		printf("[%d]:[%p] will be visited \n",i,base);
		base += tpe->block_size;
	}

	//Check overflow!
	return 1;
}

void fbgc_gc_init(struct fbgc_object * root){
	
	//Next pointer of the root in the heap
	fbgc_gc.last_location = (uint8_t*)root + size_of_fbgc_object(root);
	fbgc_gc.current_object_pool = fbgc_memb.object_pool_head;
	fbgc_gc.current_raw_buffer = fbgc_memb.raw_buffer_head;

	set_gc_black(root);
	gc_mark_fbgc_object(root);
	fbgc_gc.state = GC_STATE_PAUSED;

	//Later allocate from internal memory not with malloc
	fbgc_gc.ptr_list.data = (struct traceable_pointer_entry*) malloc(sizeof(struct traceable_pointer_entry)*TRACEABLE_POINTER_LIST_INITIAL_CAPACITY);
}


void fbgc_gc_mark(){

	if(fbgc_gc.state == GC_STATE_NOT_STARTED){
		FBGC_LOGE("GC not started\n");
		return;
	}

	struct fbgc_object * ptr = fbgc_gc.last_location;
	printf("last loc address :%p\n",ptr );

	fbgc_gc.state = GC_STATE_MARKING;

	for(size_t i = 0; i<fbgc_gc.threshold; ++i){
		if((void*)ptr >= fbgc_gc.current_object_pool->tptr) break;
		if(is_gc_gray(ptr)){
			set_gc_black(ptr);
			gc_mark_fbgc_object(ptr);
		}
		else{
			FBGC_LOGE("Addd is not gray! :%p\n",ptr );
		}
		ptr += size_of_fbgc_object(ptr);
	}

	fbgc_gc.state = GC_STATE_PAUSED;
	fbgc_gc.last_location = ptr;
}


void fbgc_gc_sweep(){

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