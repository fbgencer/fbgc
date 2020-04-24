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

	FBGC_LOGV(ARRAY_OBJECT,"For the requested size %lu new capacity is calculated as :%lu\n",size,z);
	//#ifdef ARRAY_DEBUG
	//#endif
	return z;
}

struct fbgc_object * new_fbgc_array_object(size_t cap, size_t bsize){
	//here just allocate a space after to->size, we don't need a pointer we know where we are gonna look.

	cap = array_calculate_new_capacity_from_size(cap);

	struct fbgc_array_object *ao =  (struct fbgc_array_object*) fbgc_malloc(sizeof(struct fbgc_array_object) + bsize * cap);
    ao->base.type = ARRAY;
    //ao->base.next = NULL;
    ao->block_size = bsize;
    ao->size = 0;
    ao->capacity = cap;

    FBGC_LOGV(ARRAY_OBJECT,"Array is created succesfully!\n");
    FBGC_LOGV(ARRAY_OBJECT,"Block size %lu, Size %lu, Capacity:%lu\n",ao->block_size,ao->size,ao->capacity);
    
    return (struct fbgc_object*) ao;
}


void set_in_fbgc_array_object(struct fbgc_object * self,void * data,int index){
	index = (index < 0) * capacity_fbgc_array_object(self) +  index;
	assert( index >= 0 && index < capacity_fbgc_array_object(self));
	memcpy(at_fbgc_array_object(self,index), (uint8_t*)data, block_size_fbgc_array_object(self));	
}


void * get_address_in_fbgc_array_object(struct fbgc_object * self,int index){

	index = (index < 0) * size_fbgc_array_object(self) +  index;
	assert( index>=0 || index < size_fbgc_array_object(self) );
	return (void*) (at_fbgc_array_object(self,index));
}


void * get_back_in_fbgc_array_object(struct fbgc_object * self){
	return get_address_in_fbgc_array_object(self,-1);
}
void * get_front_in_fbgc_array_object(struct fbgc_object * self){
	return get_address_in_fbgc_array_object(self,0);
}


struct fbgc_object * push_back_fbgc_array_object(struct fbgc_object * self,void * obj){

	FBGC_LOGV(ARRAY_OBJECT,"Push back array object!\n");
	FBGC_LOGV(ARRAY_OBJECT,"Array size :%lu, capacity %lu\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
	//	Check the capacity, if there is enough space push back the obj

	size_t old_size = size_fbgc_array_object(self);

	if(size_fbgc_array_object(self) == capacity_fbgc_array_object(self)){

		//Before sending to realloc, request a larger block after a request change for the capacity of the array

    	self = (struct fbgc_object*)fbgc_realloc(self,
    		sizeof(struct fbgc_array_object ) + 
			(cast_fbgc_object_as_array(self)->capacity << 1) * block_size_fbgc_array_object(self) );

    	assert(self != NULL);
    	cast_fbgc_object_as_array(self)->capacity <<= 1; //shift the capacity for the next two's power

		
		FBGC_LOGD(ARRAY_OBJECT,"New memory reallocated!\n");
		FBGC_LOGD(ARRAY_OBJECT,"After realloc Array size :%lu, capacity %lu\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
	}

	if(size_fbgc_array_object(self) < capacity_fbgc_array_object(self)){

		
		FBGC_LOGD(ARRAY_OBJECT,"There is enough space to push, pushing the object\n");

		set_in_fbgc_array_object(self,obj,old_size);
		cast_fbgc_object_as_array(self)->size = old_size+1;

		FBGC_LOGD(ARRAY_OBJECT,"New size array %lu\n",size_fbgc_array_object(self));
	}

	return (struct fbgc_object*) self;
}


void _push_back_fbgc_array_object(struct fbgc_object ** arr,void * obj){

	#define self *arr

	FBGC_LOGV(ARRAY_OBJECT,"Push back array object!\n");
	FBGC_LOGV(ARRAY_OBJECT,"Array size :%lu, capacity %lu\n",size_fbgc_array_object(self), capacity_fbgc_array_object(self));
	//	Check the capacity, if there is enough space push back the obj

	size_t old_size = size_fbgc_array_object(self);

	if(size_fbgc_array_object(self) == capacity_fbgc_array_object(self)){

		//Before sending to realloc, request a larger block after a request change for the capacity of the array

    	self = (struct fbgc_object*)fbgc_realloc(self,
    		sizeof(struct fbgc_array_object ) + 
			(cast_fbgc_object_as_array(self)->capacity << 1) * block_size_fbgc_array_object(self) );

    	cast_fbgc_object_as_array(self)->capacity <<= 1; //shift the capacity for the next two's power

		
		FBGC_LOGD(ARRAY_OBJECT,".New memory reallocated!\n");
		FBGC_LOGD(ARRAY_OBJECT,"After realloc Array size :%lu, capacity %lu\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
	}

		
	FBGC_LOGD(ARRAY_OBJECT,"There is enough space to push, pushing the object\n");

	set_in_fbgc_array_object(self,obj,old_size);
	cast_fbgc_object_as_array(self)->size = old_size+1;

	FBGC_LOGD(ARRAY_OBJECT,"New size array %lu\n",size_fbgc_array_object(self));


	#undef self
}
