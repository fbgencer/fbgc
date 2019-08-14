#include "fbgc.h"

struct fbgc_object * new_fbgc_ref_object(){
	struct fbgc_ref_object *refo =  (struct fbgc_ref_object*) fbgc_malloc(sizeof(struct fbgc_ref_object));
    refo->base.type = REFERENCE;
    refo->base.next = refo->content = NULL; //content shows address of the object
    return (struct fbgc_object*) refo;
}

struct fbgc_object * initialize_fbgc_ref_object(struct fbgc_object *ref, struct fbgc_object * obj){
	cast_fbgc_object_as_ref(ref)->content = obj;
	return (struct fbgc_object*) ref; 
}

struct fbgc_object * assign_var_to_fbgc_ref_object(struct fbgc_object * ref,struct fbgc_object * obj){
	#define refo cast_fbgc_object_as_ref(ref)

	if(refo != NULL && refo->content != NULL){
		
		refo->content = obj;
		print_fbgc_object(ref);
		cprintf(100,"Assignment is ok ! \n");


	}else{
		//should we delete this kind of errors ? 
		cprintf(100,"Undefined variable !\n");
	}
	return ref; 	
	#undef refo
}



struct fbgc_object * get_var_from_fbgc_ref_object(struct fbgc_object *ref){
	if(get_fbgc_object_type(ref) == REFERENCE){
		//claim_ownership(ref);
		return cast_fbgc_object_as_ref(ref)->content;
	}
	return  ref;
}



void print_fbgc_ref_object(struct fbgc_object * ref){
	#define refo cast_fbgc_object_as_ref(ref)

	cprintf(001,"REF:[");
	print_fbgc_object(refo->content);
	cprintf(001,"]");
	
	#undef refo

}
 

void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete referenced object, only delete yourself!
	//free(refo);
}
