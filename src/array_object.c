// #include "fbgc.h"


// struct fbgc_object * new_fbgc_array_object(size_t cap, size_t bsize){
// 	//here just allocate a space after to->size, we don't need a pointer we know where we are gonna look.

// 	cap = fbgc_round_to_closest_power_of_two(cap);

// 	struct fbgc_array_object *ao =  (struct fbgc_array_object*) fbgc_malloc_object(sizeof(struct fbgc_array_object));
//     ao->base.type = ARRAY;
//     ao->block_size = bsize;
//     ao->size = 0;
    
//     ao->content = fbgc_malloc(bsize * cap);

//     FBGC_LOGV(ARRAY_OBJECT,"Array is created succesfully!\n");
//     FBGC_LOGV(ARRAY_OBJECT,"Block size %lu, Size %lu, Capacity:%lu\n",ao->block_size,ao->size,capacity_fbgc_array_object(ao));
    
//     return (struct fbgc_object*) ao;
// }


// void set_in_fbgc_array_object(struct fbgc_object * self,void * data,int index){
// 	index = (index < 0) * size_fbgc_array_object(self) +  index;
// 	assert( index >= 0 && index < capacity_fbgc_array_object(self));
// 	memcpy(at_fbgc_array_object(self,index), (uint8_t*)data, block_size_fbgc_array_object(self));	
// }


// void * get_address_in_fbgc_array_object(struct fbgc_object * self,int index){

// 	index = (index < 0) * size_fbgc_array_object(self) +  index;
// 	assert( index>=0 || index < size_fbgc_array_object(self) );
// 	return (void*) (at_fbgc_array_object(self,index));
// }


// void * get_back_in_fbgc_array_object(struct fbgc_object * self){
// 	return get_address_in_fbgc_array_object(self,-1);
// }
// void * get_front_in_fbgc_array_object(struct fbgc_object * self){
// 	return get_address_in_fbgc_array_object(self,0);
// }


// struct fbgc_object * push_back_fbgc_array_object(struct fbgc_object * self,void * obj){

// 	FBGC_LOGV(ARRAY_OBJECT,"Push back array object!\n");
// 	FBGC_LOGV(ARRAY_OBJECT,"Array size :%lu, capacity %lu\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
// 	//	Check the capacity, if there is enough space push back the obj

// 	size_t old_size = size_fbgc_array_object(self);

// 	if(size_fbgc_array_object(self) == capacity_fbgc_array_object(self)){

// 		//Before sending to realloc, request a larger block after a request change for the capacity of the array
		
// 		cast_fbgc_object_as_array(self)->content = 
// 										fbgc_realloc(cast_fbgc_object_as_array(self)->content,(capacity_fbgc_array_object(self)<<1) * block_size_fbgc_array_object(self) );
		
// 		FBGC_LOGD(ARRAY_OBJECT,"New memory reallocated!\n");
// 		FBGC_LOGD(ARRAY_OBJECT,"After realloc Array size :%lu, capacity %lu\n",size_fbgc_array_object(self) , capacity_fbgc_array_object(self));
// 	}

	

// 	FBGC_LOGD(ARRAY_OBJECT,"There is enough space to push, pushing the object\n");
	
// 	memcpy(at_fbgc_array_object(self,old_size), (uint8_t*)obj, block_size_fbgc_array_object(self));

// 	//set_in_fbgc_array_object(self,obj,old_size);
// 	cast_fbgc_object_as_array(self)->size = old_size+1;

// 	FBGC_LOGD(ARRAY_OBJECT,"New size array %lu\n",size_fbgc_array_object(self));
	

// 	return (struct fbgc_object*) self;
// }
