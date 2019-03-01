#include "fbgc.h"


struct fbgc_object * new_fbgc_tuple_object(int size){
	struct fbgc_tuple_object *to =  (struct fbgc_tuple_object*) malloc(sizeof(struct fbgc_tuple_object));
    to->base.type = TUPLE;
    to->base.next = NULL;
    to->contents = (struct fbgc_object **) malloc(sizeof(struct fbgc_object*)*size);
    to->size = size;
    cprintf(100,"TUPLE opened size:%d\n",size);
    return (struct fbgc_object*) to;
}

void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index){
	if(index >= 0 && index < cast_fbgc_object_as_tuple(self)->size ){
		cast_fbgc_object_as_tuple(self)->contents[index] = obj;
		//obj->type |= 0x80;	
	}
	else {
		assert("Negative index int set tuple\n");
	}
}
struct fbgc_object *  get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index){
	if(index >= 0 && index < cast_fbgc_object_as_tuple(self)->size ){
		return cast_fbgc_object_as_tuple(self)->contents[index];
	}
	else {
		assert("Negative index int set tuple\n");
		return NULL;
	}
}

struct fbgc_object * add_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b){
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

    return res;
}

void print_fbgc_tuple_object(struct fbgc_object * obj){
	cprintf(011,"(");
	for(unsigned int i = 0; i<cast_fbgc_object_as_tuple(obj)->size; i++){
		print_fbgc_object(cast_fbgc_object_as_tuple(obj)->contents[i]);
		cprintf(011,", ");
	}
	cprintf(011,"\b\b)");
}

void free_fbgc_tuple_object(struct fbgc_object * obj){
    for(int i = 0; i<cast_fbgc_object_as_tuple(obj)->size; i++){
    	
    	//cast_fbgc_object_as_tuple(obj)->contents[i]->type &= 0x7F;
    	//cprintf(100,"Deleting my contents 2 0x%X\n",cast_fbgc_object_as_tuple(obj)->contents[i]->type);
    	free_fbgc_object(cast_fbgc_object_as_tuple(obj)->contents[i]);
    }
    free(cast_fbgc_object_as_tuple(obj)->contents);
    free(obj);
}
