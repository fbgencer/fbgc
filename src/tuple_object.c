#include "fbgc.h"


struct fbgc_object * new_fbgc_tuple_object(size_t cap){
	struct fbgc_tuple_object *to =  (struct fbgc_tuple_object*) fbgc_malloc(sizeof(struct fbgc_tuple_object));
    to->base.type = TUPLE;
    to->base.next = NULL;
    to->size = 0;
    to->capacity = calculate_new_capacity_from_size(cap);
    cprintf(111,"capacity is %d\n",to->capacity);
    //here just allocate a space after to->size, we don't need a pointer we know where we are gonna look.
    fbgc_malloc(sizeof(struct fbgc_object*)*to->capacity);

    return (struct fbgc_object*) to;
}


size_t calculate_new_capacity_from_size(size_t size){
	/*
		Below algorithm calculates the capacity for the given size
		Basically capacity is the closest two's power
		1 : 2
		2,3 : 4
		4,5,6,7 : 8
		8,9,10,11,12,13,14,15 : 16
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

	cprintf(011,"size tuple %d, capacity tuple :%d\n",size,z);
	return z;
}

void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index){
	
	//Index can be negative, assume that array rotates.
	//in order to get the contents of the array use macro from tuple_object.h

	//#############################
	//Add negative index algortihm and boundary check
	//#############################

	assert( index >= 0 && index < size_fbgc_tuple_object(self) );

	struct fbgc_object ** contents = tuple_object_content(self);
	contents[index] = obj; 
	
}
struct fbgc_object *  get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index){
	//#############################
	//Add negative index algortihm and boundary check
	//#############################

	assert( index>=0 || index < size_fbgc_tuple_object(self) );

	struct fbgc_object ** contents = tuple_object_content(self);
	return (struct fbgc_object *) contents[index]; 	
}


struct fbgc_object * push_back_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj){


	cprintf(101,"Push back tuple object!\n");
	/*
		Check the capacity, if there is enough space push back the obj
	*/
	cprintf(101,"Tuple size :%d, capacity %d\n",size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));


	if(size_fbgc_tuple_object(self) == capacity_fbgc_tuple_object(self)){
		cast_fbgc_object_as_tuple(self)->capacity <<= 1; //shift the capacity for the next two's power

    	self = fbgc_realloc(self,sizeof(struct fbgc_tuple_object ) + capacity_fbgc_tuple_object(self) * sizeof(struct fbgc_object*) );

    	//############
    		//check the self pointer, it may be null!
    	//############
	}

	if(size_fbgc_tuple_object(self) < capacity_fbgc_tuple_object(self)){
		struct fbgc_object ** contents = tuple_object_content(self);
		contents[cast_fbgc_object_as_tuple(self)->size] = obj;
		cast_fbgc_object_as_tuple(self)->size++;
		cprintf(001,"New size tuple %d\n",size_fbgc_tuple_object(self));
	}

	return self;

}





struct fbgc_object * add_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b){
    /*
    //(1,2,3) + 5 = (6,7,8);
    // (1,5) + (10,200) = (11,205);
	struct fbgc_object * res = NULL;
	if(a->type == b->type && (cast_fbgc_object_as_tuple(a)->size == cast_fbgc_object_as_tuple(b)->size)){
		// both tuple
		res = new_fbgc_tuple_object(cast_fbgc_object_as_tuple(a)->size);
		for (int i = 0; i < cast_fbgc_object_as_tuple(a)->size; ++i){
			set_object_in_fbgc_tuple_object(res,fbgc_binary_plus(cast_fbgc_object_as_tuple(a)->contents[i],cast_fbgc_object_as_tuple(b)->contents[i]),i);
		}
	}
	else if(a->type == TUPLE){
		res = new_fbgc_tuple_object(cast_fbgc_object_as_tuple(a)->size);
		for (int i = 0; i < cast_fbgc_object_as_tuple(a)->size; ++i){
				set_object_in_fbgc_tuple_object(res,fbgc_binary_plus(cast_fbgc_object_as_tuple(a)->contents[i],b),i);
		}
	}
	else if(b->type == TUPLE){
		res = new_fbgc_tuple_object(cast_fbgc_object_as_tuple(b)->size);
		for (int i = 0; i < cast_fbgc_object_as_tuple(b)->size; ++i){
				set_object_in_fbgc_tuple_object(res,fbgc_binary_plus(cast_fbgc_object_as_tuple(b)->contents[i],a),i);
		}		
	}

    return res;*/
}

void print_fbgc_tuple_object(struct fbgc_object * obj){
	cprintf(011,"(");
	struct fbgc_object ** contents = tuple_object_content(obj);
	for(unsigned int i = 0; i<cast_fbgc_object_as_tuple(obj)->size; i++){
		print_fbgc_object(contents[i]);
		cprintf(011,", ");
	}
	cprintf(011,"\b\b)");
}

void free_fbgc_tuple_object(struct fbgc_object * obj){
   /* for(int i = 0; i<cast_fbgc_object_as_tuple(obj)->size; i++){
    	
    	//cast_fbgc_object_as_tuple(obj)->contents[i]->type &= 0x7F;
    	//cprintf(100,"Deleting my contents 2 0x%X\n",cast_fbgc_object_as_tuple(obj)->contents[i]->type);
    	free_fbgc_object(cast_fbgc_object_as_tuple(obj)->contents[i]);
    }
    free(cast_fbgc_object_as_tuple(obj)->contents);
    free(obj);*/
}
