#include "fbgc.h"



void initialize_fbgc_memory_block(){
	//first allocate a big chunck that contains both inital buffer and object pool
	fbgc_memb.internal_buffer_head = (void *) calloc(1,INITIAL_INTERNAL_BUFFER_SIZE);
	assert(fbgc_memb.internal_buffer_head);
	fbgc_memb.internal_buffer_size = INITIAL_INTERNAL_BUFFER_SIZE;


	fbgc_memb.object_pool_head  =  (void *) calloc(1,INITIAL_OBJECT_POOL_SIZE);	
	assert(fbgc_memb.object_pool_head != NULL);
	fbgc_memb.object_pool_size = INITIAL_OBJECT_POOL_SIZE;

	fbgc_memb.object_pool_ptr= fbgc_memb.object_pool_head;

}

void * fbgc_malloc(size_t size){
	/*
		[FBGC Malloc algorithm]
		Conditions
		1. Requested size can be bigger than or equal to our object pool
			Allocate a new space but calculate new allocation size by using the ptr also.
			If we have 100bytes object pool and 30bytes available space, and the requested size is 120 bytes then
			we need to allocate new 90bytes 

		2. Requested size is smaller than the object pool
			>> Check the size and the available space 

			-if we have enough space return the pointer of this space	
				[---------------------------------|                  ]
				^						    ^			   ^
			object_pool_head                object_pool_ptr    object_pool_head + object_pool_size 

	
			-if the requested size is bigger than the availabe space
				run garbage collector
				try to find a spot to put the data
				if there is no space just allocate a new space
			

	*/
	if( (fbgc_memb.object_pool_head + fbgc_memb.object_pool_size - fbgc_memb.object_pool_ptr) >= size){
		#ifdef MEM_DEBUG
			cprintf(111,"Requested memory is available\n");
		#endif

		fbgc_memb.object_pool_ptr += size; 	
		return (fbgc_memb.object_pool_ptr - size);		
		
	}
	else{
		#ifdef MEM_DEBUG
			cprintf(111,"There is no enough memory!\n");
		#endif		
			
			//run gc
			//find a spot

			//if we don't return until this point, double the size
			void * new_op = realloc(fbgc_memb.object_pool_head,fbgc_memb.object_pool_size * 2);
			if(new_op == NULL){
				cprintf(111,"Realloc null returned...\n");
			}
			
			cprintf(101,"Realloc basarili\n");

			fbgc_memb.object_pool_size *= 2;
			cprintf(101,"Yenib yout %d\n",fbgc_memb.object_pool_size);
			//calculate the old distance between ptr and the head and find the new location, it could be the same
			fbgc_memb.object_pool_ptr = new_op + (fbgc_memb.object_pool_ptr - fbgc_memb.object_pool_head);
			fbgc_memb.object_pool_head = new_op;
			fbgc_memb.object_pool_ptr += size; 	
			return (fbgc_memb.object_pool_ptr - size);	

	}


		

}

void * fbgc_realloc(void *ptr, size_t size){

}

void fbgc_free(void *ptr){

}


void print_fbgc_memory_block(){

	cprintf(010,"############[MEMORY_BLOCK]############\n");
	cprintf(111,"Internal buffer size : %d\n",fbgc_memb.internal_buffer_size);
    	for(int i = 0; i<fbgc_memb.internal_buffer_size; i++){
		int val = *( char *)(fbgc_memb.internal_buffer_head+i); 
	   	cprintf(010,"[%d]:%0x\n",(int *)(fbgc_memb.internal_buffer_head+i),val);
    	}    
	cprintf(111,"Object pool size: %d\n",fbgc_memb.object_pool_size );

	for(int i = 0; i<fbgc_memb.object_pool_size; i++){
		int val = *( char *)(fbgc_memb.object_pool_head+i); 
	   	cprintf(010,"[%d]:%0x\n",(int *)(fbgc_memb.object_pool_head+i),val);
    	} 

	cprintf(010,"############[MEMORY_BLOCK]############\n");
}


void free_fbgc_memory_block(){
	free(fbgc_memb.internal_buffer_head);
	fbgc_memb.internal_buffer_size = 0;
	free(fbgc_memb.object_pool_head);
	fbgc_memb.object_pool_size = 0;
}
