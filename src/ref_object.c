#include "fbgc.h"

struct fbgc_object * new_fbgc_ref_object(){
	struct fbgc_ref_object *refo =  (struct fbgc_ref_object*) fbgc_malloc(sizeof(struct fbgc_ref_object));
    refo->base.type = REFERENCE;
    refo->base.next = refo->content = NULL; //content shows address of the object
    return (struct fbgc_object*) refo;
}

struct fbgc_object * initialize_fbgc_ref_object(struct fbgc_object *ref, struct fbgc_object * obj){
	struct fbgc_ref_object * refo = cast_fbgc_object_as_ref(ref);
	refo->content = obj;
	return (struct fbgc_object*) refo; 
}

struct fbgc_object * assign_var_to_fbgc_ref_object(struct fbgc_object * ref,struct fbgc_object * obj){
	#define refo cast_fbgc_object_as_ref(ref)

	if(refo != NULL && refo->content != NULL){
		cprintf(100,"Assignment succesfull!\n");
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
		struct fbgc_object * obj = (struct fbgc_object *)(cast_fbgc_object_as_ref(ref)->content);
		//cprintf(100,"{%s",object_name_array[0x7F & obj->type]);
		cprintf(100,"{%s",content_fbgc_cstr_object(obj));
		cprintf(111,":");
		if(cast_fbgc_object_as_cstr(obj)->base.next != NULL){
			//print_fbgc_object(obj->base.next);
			cprintf(011,"}");
		}
		else
			cprintf(111,"NULL}");		
	//}

	//print_fbgc_object(cast_fbgc_object_as_ref(ref)->content);

}
 

void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete referenced object, only delete yourself!
	//free(refo);
}