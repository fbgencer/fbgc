#include "fbgc.h"


struct fbgc_object * new_fbgc_tuple_object(int size){
	fbgc_assert(size <0,"Negative size for tuple");
	struct fbgc_tuple_object *to =  (struct fbgc_tuple_object*) malloc(sizeof(struct fbgc_tuple_object));
    to->base.type = TUPLE;
    to->base.next = NULL;
    to->contents = (struct fbgc_object **) malloc(sizeof(struct fbgc_object*)*size);
    to->size = size;
    return (struct fbgc_object*) to;
}

/*
void push_front_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj){

}
*/

void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index){
	struct fbgc_tuple_object * to = cast_fbgc_object_as_tuple(self);
	if(index >= 0 && index < to->size ){
		to->contents[index] = obj;
	}
	else {
		fbgc_assert(1,"Negative index int set tuple\n");
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
	struct fbgc_tuple_object * to = cast_fbgc_object_as_tuple(obj);
	cprintf(011,"(");
	for(unsigned int i = 0; i<to->size; i++){
		print_fbgc_object(to->contents[i]);
		cprintf(011,", ");
	}
	cprintf(011,"\b\b)");
}

void free_fbgc_tuple_object(struct fbgc_object * obj){
    struct fbgc_tuple_object * to = cast_fbgc_object_as_tuple(obj);
    for(int i = 0; i<to->size; i++)
    	free_fbgc_object(to->contents[i]);
    free(to->contents);
    free(to);
}
