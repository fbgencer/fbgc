#include "fbgc.h"

size_t calculate_new_capacity_from_size(size_t size){
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

	#ifdef TUPLE_DEBUG
	cprintf(011,"For the size %d new capacity is calculated as :%d\n",size,z);
	#endif
	return z;
}


struct fbgc_object * new_fbgc_tuple_object(size_t cap){
	//here just allocate a space after to->size, we don't need a pointer we know where we are gonna look.

	cap = calculate_new_capacity_from_size(cap);

	struct fbgc_tuple_object *to =  (struct fbgc_tuple_object*) fbgc_malloc(sizeof(struct fbgc_tuple_object) + sizeof(struct fbgc_object*)*cap);
    to->base.type = TUPLE;
    to->base.next = NULL;
    to->size = 0;
    to->capacity = cap;

    return (struct fbgc_object*) to;
}


struct fbgc_object * new_fbgc_tuple_object_from_tuple_content(struct fbgc_object ** src, int num){

	struct fbgc_object * to = new_fbgc_tuple_object(num);
	size_fbgc_tuple_object(to) = num;

	struct fbgc_object ** contents = tuple_object_content(to);
	for(int i = 0; i<num; ++i){
		contents[i] = src[i];
	}
	
	/*cap = calculate_new_capacity_from_size(cap);
	struct fbgc_tuple_object * to =  (struct fbgc_tuple_object*) fbgc_malloc(sizeof(struct fbgc_tuple_object) + sizeof(struct fbgc_object*)*cap);
    to->base.type = TUPLE;
    to->base.next = NULL;
    to->size = 0;
    to->capacity = cap;
*/
    return (struct fbgc_object*) to;
}


void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index){
	
	//Index can be negative, assume that array rotates.
	//in order to get the contents of the array use macro from tuple_object.h

	//#############################
	//Add negative index algortihm and boundary check
	//#############################
	//index size'dan büyükse hata,
	//index sıfırdan küçükse ama -size+1'den de büyükse sorun yok

	assert( index>=0 || index < size_fbgc_tuple_object(self) );

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


struct fbgc_object *  get_top_in_fbgc_tuple_object(struct fbgc_object * self){

	struct fbgc_object ** contents = tuple_object_content(self);
	return size_fbgc_tuple_object(self) != 0 ? 
			(struct fbgc_object *) contents[size_fbgc_tuple_object(self)-1] :
			NULL;
}


struct fbgc_object * push_back_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj){

	#ifdef TUPLE_DEBUG
	cprintf(101,"Push back tuple object!\n");
	//
	//	Check the capacity, if there is enough space push back the obj
	//
	cprintf(101,"Tuple size :%d, capacity %d\n",size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));
	#endif

	size_t old_size = size_fbgc_tuple_object(self);

	if(size_fbgc_tuple_object(self) == capacity_fbgc_tuple_object(self)){

		//Before sending to realloc, request a larger block after requesting change the capacity of the tuple

    	self = fbgc_realloc(self,
    		sizeof(struct fbgc_tuple_object ) + 
			(cast_fbgc_object_as_tuple(self)->capacity << 1) * sizeof(struct fbgc_object*) );

    	assert(self != NULL);
    	cast_fbgc_object_as_tuple(self)->capacity <<= 1; //shift the capacity for the next two's power

		#ifdef TUPLE_DEBUG
		cprintf(101,"New memory reallocated!\n");
		cprintf(101,"After realloc Tuple size :%d, capacity %d\n",size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));
		#endif

    	//############
    		//check the self pointer, it might be null!
    	//############
	}

	if(size_fbgc_tuple_object(self) < capacity_fbgc_tuple_object(self)){

		#ifdef TUPLE_DEBUG
		cprintf(001,"There is enough space to push, pushing the object..\n");
		#endif

		
		struct fbgc_object ** contents = tuple_object_content(self);
		contents[old_size] = obj;
		cast_fbgc_object_as_tuple(self)->size = old_size+1;
		

		#ifdef TUPLE_DEBUG
		cprintf(001,"New size tuple %d\n",size_fbgc_tuple_object(self));
		#endif
	}

	return self;
}

/*
struct fbgc_object * push_front_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj){

	#ifdef TUPLE_DEBUG
	cprintf(101,"Push front tuple object!\n");
	//
	//	Check the capacity, if there is enough space push back the obj
	//
	cprintf(101,"Tuple size :%d, capacity %d\n",size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));
	#endif

	size_t old_size = size_fbgc_tuple_object(self);

	if(size_fbgc_tuple_object(self) == capacity_fbgc_tuple_object(self)){

		//Before sending to realloc, request a larger block after requesting change the capacity of the tuple

    	self = fbgc_realloc(self,
    		sizeof(struct fbgc_tuple_object ) + 
			(cast_fbgc_object_as_tuple(self)->capacity << 1) * sizeof(struct fbgc_object*) );

    	assert(self != NULL);
    	cast_fbgc_object_as_tuple(self)->capacity <<= 1; //shift the capacity for the next two's power

		#ifdef TUPLE_DEBUG
		cprintf(101,"New memory reallocated!\n");
		cprintf(101,"After realloc Tuple size :%d, capacity %d\n",size_fbgc_tuple_object(self) , capacity_fbgc_tuple_object(self));
		#endif

    	//############
    		//check the self pointer, it might be null!
    	//############
	}

	if(size_fbgc_tuple_object(self) < capacity_fbgc_tuple_object(self)){

		#ifdef TUPLE_DEBUG
		cprintf(001,"There is enough space to push, pushing the object..\n");
		#endif

		
		struct fbgc_object ** contents = tuple_object_content(self);
		contents[old_size] = obj;
		cast_fbgc_object_as_tuple(self)->size = old_size+1;
		

		#ifdef TUPLE_DEBUG
		cprintf(001,"New size tuple %d\n",size_fbgc_tuple_object(self));
		#endif
	}

	return self;
}*/

int index_fbgc_tuple_object(struct fbgc_object * self, struct fbgc_object * obj){

	struct fbgc_object ** contents = tuple_object_content(self);

	for(size_t i = 0; i<size_fbgc_tuple_object(self); i++){
		if(contents[i] == obj) return i;
	}

	return -1;

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

    return a;
}

void print_fbgc_tuple_object(struct fbgc_object * obj){
	cprintf(011,"(");
	struct fbgc_object ** contents = tuple_object_content(obj);

	for(size_t i = 0; i<cast_fbgc_object_as_tuple(obj)->size; i++){
		print_fbgc_object(contents[i]);
		cprintf(011,", ");
	}

	//##########TODO#############
	//improve this part 
	cprintf(011,")");
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
