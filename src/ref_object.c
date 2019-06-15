#include "fbgc.h"

struct fbgc_object * new_fbgc_ref_object(){
	struct fbgc_ref_object *refo =  (struct fbgc_ref_object*) malloc(sizeof(struct fbgc_ref_object));
    refo->base.type = REFERENCE;
    refo->base.next = NULL;
    refo->content = NULL; //content shows address of the object
    return (struct fbgc_object*) refo;
}

struct fbgc_object * initialize_fbgc_ref_object(struct fbgc_object *ref, struct fbgc_object * obj){
	struct fbgc_ref_object * refo = cast_fbgc_object_as_ref(ref);
	refo->content = obj;
	return (struct fbgc_object*) refo; 
}

void assign_var_to_fbgc_ref_object(struct fbgc_object * ref,struct fbgc_object * obj){
	#define refo cast_fbgc_object_as_ref(ref)

	if(refo != NULL && refo->content != NULL){
		refo->content->next = obj;
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
		return cast_fbgc_object_as_ref(ref)->content->next;
	}
	return  ref;
}



void print_fbgc_ref_object(struct fbgc_object * ref){
	//if(cast_fbgc_object_as_ref(ref)->content->type == STRING){
		struct fbgc_str_object * obj = (struct fbgc_str_object *)(cast_fbgc_object_as_ref(ref)->content);
		if(obj->content != NULL)cprintf(100,"{%s",obj->content);
		cprintf(111,":");
		if(obj->base.next != NULL){
			print_fbgc_object(obj->base.next);
			cprintf(011,"}");
		}
		else cprintf(111,"NULL}");		
	//}

	//print_fbgc_object(cast_fbgc_object_as_ref(ref)->content);

}
 

void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete referenced object, only delete yourself!
	free(refo);
}