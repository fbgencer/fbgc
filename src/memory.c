#include "fbgc.h"



void initialize_fbgc_memory_block(){
	//first allocate a big chunck that contains both inital buffer and object pool
	/*fbgc_memb.internal_buffer_head = (void *) calloc(INITIAL_INTERNAL_BUFFER_SIZE,1);
	assert(fbgc_memb.internal_buffer_head);
	fbgc_memb.internal_buffer_size = INITIAL_INTERNAL_BUFFER_SIZE;*/


	fbgc_memb.object_pool_head  =  (struct fbgc_memory_pool *) malloc(sizeof(struct fbgc_memory_pool));	
	assert(fbgc_memb.object_pool_head != NULL);
	fbgc_memb.object_pool_size = 1;

	fbgc_memb.object_pool_head->data =  calloc(PAGE_SIZE,1);	
	assert(fbgc_memb.object_pool_head->data != NULL);
	fbgc_memb.object_pool_head->tptr = fbgc_memb.object_pool_head->data;
	fbgc_memb.object_pool_head->size = PAGE_SIZE;
	fbgc_memb.object_pool_head->next = NULL;

}

void * fbgc_malloc(size_t size){
	#ifdef MEM_DEBUG
		cprintf(100,"FBGC_MALLOC IS CALLED size :%d\n",size);
	#endif


	/*
		[FBGC Malloc algorithm]
		
		In this section, there are different situations to allocate memory efficiently.

		Requested size : size, available size will be asize 

		Memory structure contains object memory pools which are allocated spaces in the heap
		fbgc_memory_pool contains *data,*tptr, size and next pointer.
		Different pools are connected each other like a single linked list with next pointer.
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
	struct fbgc_memory_pool * opool_iter = fbgc_memb.object_pool_head;

	char state = 0;

	while(opool_iter != NULL){
		//check the size

		//work on this
		if(size > opool_iter->size) state = 1;
		else state = 0;

		#ifdef MEM_DEBUG
			cprintf(111,"Requested memory from chunk: %d, total memory : %d,available mem: %d \n",
				size,opool_iter->size,(opool_iter->data + opool_iter->size - opool_iter->tptr));
		#endif


		if( (opool_iter->data + opool_iter->size - opool_iter->tptr) >= size){
			#ifdef MEM_DEBUG
				//cprintf(111,"Requested memory is available\n");
			#endif

			opool_iter->tptr += size; 	
			return (opool_iter->tptr - size);		
		}		
		//iterate through the chunks
		//we put this for the next use of opool_iter, we don't want it as NULL
		if(opool_iter->next == NULL) break;

		opool_iter = opool_iter->next;
	}

	#ifdef MEM_DEBUG
	if(state == 1)
		cprintf(011,"Goto new allocation!");
	#endif	

	if(state == 1) goto NEW_POOL_ALLOCATION;

//#################################################################################################################
// 
//	After writing GC algorithm uncomment below!	
//
//#################################################################################################################

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


	NEW_POOL_ALLOCATION:

	#ifdef MEM_DEBUG
		cprintf(111,"There is no enough memory, new allocation\n");
	#endif	

	//if above code did not return succesfully, we need to allocate a new chunk!
	//Notice that iter shows the end of the chunks, we will use iter

	opool_iter->next = (struct fbgc_memory_pool *) malloc(sizeof(struct fbgc_memory_pool));	
	assert(opool_iter->next != NULL);
	fbgc_memb.object_pool_size++;
	opool_iter = opool_iter->next;
	opool_iter->next = NULL;

	//calculate the multiplicity of the new chunk
	//only allow to allocate integer multiples of the page size
	//assume size = 111, page_size = 20, mpage becomes 120, 
	opool_iter->size =  PAGE_SIZE*((size_t)((size+sizeof(struct fbgc_garbage_object))/(PAGE_SIZE*1.0))+1);
	#ifdef MEM_DEBUG
		cprintf(111,"Allocated new pool size :%d\n",opool_iter->size);
	#endif		
	opool_iter->data = calloc(opool_iter->size,1);	
	assert(opool_iter->data != NULL);
	opool_iter->tptr = opool_iter->data + size;
	
	return (opool_iter->tptr - size);	
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

	size_t block_size = get_fbgc_object_size(ptr);

	#ifdef MEM_DEBUG
		cprintf(010,"~~~~~~~Realloc~~~~~~~~\n");
		cprintf(010,"Requested memory in realloc is %d, block size : %d\n",size,block_size);
	#endif

	if(size > block_size){
	#ifdef MEM_DEBUG
		cprintf(010,"Allocating new block size : %d and copying\n",size);
	#endif
		void * new_mem_ptr = fbgc_malloc(size);
		if(new_mem_ptr == NULL) return NULL;

	#ifdef MEM_DEBUG
		cprintf(010,"succesfully allocated size : %d,old address:%p new address:%p\n",size,ptr,new_mem_ptr);
	#endif

		memcpy(new_mem_ptr,ptr,block_size);
		//Do we need it ?
		fbgc_free(ptr);

		return new_mem_ptr;
	}

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


void print_fbgc_memory_block(){

	cprintf(010,"############[MEMORY_BLOCK]############\n");
	cprintf(111,"Internal buffer size : %d\n",fbgc_memb.internal_buffer_size);
    	for(int i = 0; i<fbgc_memb.internal_buffer_size; i++){
		int val = *( char *)(fbgc_memb.internal_buffer_head+i); 
	   	cprintf(010,"[%p]:%0x\n",(int *)(fbgc_memb.internal_buffer_head+i),val);
    	}    
	cprintf(111,"Object pool size: %d\n",fbgc_memb.object_pool_size );

	cprintf(011,"############[OBJECT POOL]############\n");
	struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;

	//just to clear obj locations
	
	for(int i = 0; i<fbgc_memb.object_pool_size && iter != NULL; i++){
		cprintf(111,"[%d.Chunk] size: %d\n",i,iter->size);
		size_t obj_start = 0;
		for(int j = 0; j<iter->size; j++){
			int val = *( char *)((iter->data)+j); 

			if(j == obj_start && ((char *)iter->data+j) != iter->tptr){
				struct fbgc_object * dummy = ((iter->data)+j);
				obj_start += get_fbgc_object_size(dummy);
				//print the type of the data in the memory
				cprintf(101,">>[%s] obj size in memory %d\n",object_name_array[0x7F & val],get_fbgc_object_size(dummy));				
				//print the address and the data in the memory!
				//cprintf(101,"[%d][%p]:%0x",j,((iter->data)+j),val);


			}
			//else
			{
	   			cprintf(010,"[%d][%p]",j,((iter->data)+j));
	   			cprintf(110,":%0x",val);
	   			
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
	cprintf(011,"############[OBJECT POOL]############\n");
	cprintf(111,"Object pool size: %d\n",fbgc_memb.object_pool_size );
	
	cprintf(010,"############[MEMORY_BLOCK]############\n");
}


void free_fbgc_memory_block(){
	free(fbgc_memb.internal_buffer_head);
	fbgc_memb.internal_buffer_size = 0;


	struct fbgc_memory_pool * iter = fbgc_memb.object_pool_head;
	struct fbgc_memory_pool * temp = iter;


	for (size_t i = 0; i < fbgc_memb.object_pool_size; ++i){
		free(iter->data);
		temp = iter->next;
		free(iter);
		iter = temp;
	}
}
