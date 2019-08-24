#include "fbgc.h"

struct fbgc_object * new_fbgc_ref_object(const int i){
	struct fbgc_ref_object *refo =  (struct fbgc_ref_object*) fbgc_malloc(sizeof(struct fbgc_ref_object));
    refo->base.type = REFERENCE;
    refo->base.next = NULL;
    refo->loc = i;
    return (struct fbgc_object*) refo;
}

struct fbgc_object * assign_var_to_fbgc_ref_object(struct fbgc_object * ref,struct fbgc_object * obj){
	#define refo cast_fbgc_object_as_ref(ref)

	/*if(refo != NULL && refo->content != NULL){
		
		refo->content = obj;
		print_fbgc_object(ref);
		cprintf(100,"Assignment is ok ! \n");


	}else{
		//should we delete this kind of errors ? 
		cprintf(100,"Undefined variable !\n");
	}*/
	return ref; 	
	#undef refo
}



struct fbgc_object * get_var_from_fbgc_ref_object(struct fbgc_object *ref){
	/*if(get_fbgc_object_type(ref) == REFERENCE){
		//claim_ownership(ref);
		return cast_fbgc_object_as_ref(ref)->content;
	}
	return  ref;*/
}



void print_fbgc_ref_object(struct fbgc_object * ref){
	#define refo cast_fbgc_object_as_ref(ref)

	cprintf(011,"[%s<%d>]",object_name_array[ref->type],cast_fbgc_object_as_ref(ref)->loc);
	
	#undef refo

}
 

void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete referenced object, only delete yourself!
	//free(refo);
}
