#include "fbgc.h"

size_t array_calculate_new_capacity_from_size(size_t size){
	/*
		Below algorithm calculates the capacity for the given size
		Basically capacity is the closest two's power
		0,1,2 : 2
		3,4 : 4
		5,6,7,8 : 8
		9,10,11,12,13,14,15 : 16
		and so on

		Take 5 for example, in binary its 0b00101
		take z = 1
		0b0001 <= 0b0101 , shift z left
		0b0010 <= 0b0101 , (ditto)
		0b0100 <= 0b0101 , (ditto)
		0b1000 <= 0b0101 , stop here don't shift, z is 8, the closest two's power for 5
	*/
	size_t z = 1;
	while(z < size)
		z <<= 1;

	#ifdef ARRAY_DEBUG
	cprintf(011,"For the size %d new capacity is calculated as :%d\n",size,z);
	#endif
	return z;
}

struct fbgc_object * new_fbgc_array_object(size_t cap, size_t b_size){
	//here just allocate a space after to->size, we don't need a pointer we know where we are gonna look.

	cap = array_calculate_new_capacity_from_size(cap);

	struct fbgc_array_object *ao =  (struct fbgc_array_object*) fbgc_malloc(sizeof(struct fbgc_array_object) + b_size * cap);
    ao->base.type = ARRAY;
    //ao->base.next = NULL;
    ao->block_size = b_size;
    ao->size = 0;
    ao->capacity = cap;

    #ifdef ARRAY_DEBUG
    cprintf(111,"Array is created succesfully!\n");
    cprintf(111,"Block size %d, Size %d, Capacity:%d\n",ao->block_size,ao->size,ao->capacity);
    #endif

    return (struct fbgc_object*) ao;
}


void set_in_fbgc_array_object(struct fbgc_object * self,void * data,int index){
	
	//Index can be negative, assume that array rotates.
	//in order to get the contents of the array use macro from array_object.h
	//#############################
	//Add negative index algortihm and boundary check
	//#############################

	assert( index >= 0 && index < capacity_fbgc_array_object(self));
/*
	struct fbgc_array_object * ao = cast_fbgc_object_as_array(self);
	unsigned char ** contents = (unsigned char **)( (unsigned char*) &self->type + sizeof(struct fbgc_array_object));
	//contents[index] = data; 

	cprintf(111,"Base type %p\n",&ao->base.type);
	cprintf(111,"Base next %p\n",&ao->base.next);
	cprintf(111,"Block_size %p\n",&ao->block_size);
	cprintf(111,"Capacity %p\n",&ao->capacity);
	cprintf(111,"Size %p\n",&ao->size);
	

	cprintf(111,"Array object content address %p, size address %p\n",(char*)contents+1,&cast_fbgc_object_as_array(self)->size);*/

	//memcpy((char*)contents+index*ao->block_size, (unsigned char*)data,ao->block_size);

	memcpy(array_object_at(self,index), (unsigned char*)data, block_size_fbgc_array_object(self));	
}


void *  get_content_in_fbgc_array_object(struct fbgc_object * self,int index){

	//#############################
	//Add negative index algortihm and boundary check
	//#############################

	assert( index>=0 || index < size_fbgc_array_object(self) );	

	unsigned char ** contents = (unsigned char **)( (unsigned char*) &self->type + sizeof(struct fbgc_array_object));
	return (void*)contents[index]; 		
}


void *  get_address_in_fbgc_array_object(struct fbgc_object * self,int index){
	//#############################
	//Add negative index algortihm and boundary check
	//#############################

	assert( index>=0 || index < size_fbgc_array_object(self) );	

	return (void*) (array_object_at(self,index));
}


void * get_top_in_fbgc_array_object(struct fbgc_object * self){
	return size_fbgc_array_object(self) != 0 ? get_address_in_fbgc_array_object(self,size_fbgc_array_object(self)-1): NULL;
}


struct fbgc_object * push_back_fbgc_array_object(struct fbgc_object * self,void * obj){

	#ifdef ARRAY_DEBUG
	cprintf(101,"Push back array object!\n");
	//
	//	Check the capacity, if there is enough space push back the obj
	//
	cprintf(101,"Array size :%d, capacity %d\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
	#endif

	size_t old_size = size_fbgc_array_object(self);

	if(size_fbgc_array_object(self) == capacity_fbgc_array_object(self)){

		//Before sending to realloc, request a larger block after a request change for the capacity of the array

    	self = fbgc_realloc(self,
    		sizeof(struct fbgc_array_object ) + 
			(cast_fbgc_object_as_array(self)->capacity << 1) * block_size_fbgc_array_object(self) );

    	assert(self != NULL);
    	cast_fbgc_object_as_array(self)->capacity <<= 1; //shift the capacity for the next two's power

		#ifdef ARRAY_DEBUG
		cprintf(101,"New memory reallocated!\n");
		cprintf(101,"After realloc Array size :%d, capacity %d\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
		#endif

    	//############
    		//check the self pointer, it might be null!
    	//############
	}

	if(size_fbgc_array_object(self) < capacity_fbgc_array_object(self)){

		#ifdef ARRAY_DEBUG
		cprintf(001,"There is enough space to push, pushing the object..\n");
		#endif

		set_in_fbgc_array_object(self,obj,old_size);

		//void ** contents = array_object_content(self);
		//contents[old_size] = obj;

		cast_fbgc_object_as_array(self)->size = old_size+1;
		

		#ifdef ARRAY_DEBUG
		cprintf(001,"New size array %d\n",size_fbgc_array_object(self));
		#endif
	}

	return (struct fbgc_object*) self;
}



int index_fbgc_array_object(struct fbgc_object * self, void * obj){
	
	unsigned char ** contents = (unsigned char **)( (unsigned char*) &self->type + sizeof(struct fbgc_array_object));
	
	for(size_t i = 0; i<size_fbgc_array_object(self); i++){
		if(array_object_at(self,i) == obj) return i;
	}
	return -1;
}
