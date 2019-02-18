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

struct fbgc_object * assign_var_to_fbgc_ref_object(struct fbgc_object * table, struct fbgc_object *ref,struct fbgc_object * obj){
	struct fbgc_ref_object * refo = cast_fbgc_object_as_ref(ref);
	if(refo != NULL && refo->content != NULL){
		if(refo->content->next != NULL && !is_object_referenced_in_symbol_table(table,ref,refo->content->next)){
			free_fbgc_object(refo->content->next);
		}
		refo->content->next = obj;
		//cprintf(010,"refo assigned!\n");
	}else{
		cprintf(100,"Undefined variable !\n");
	}
	return (struct fbgc_object*) refo; 	
}

struct fbgc_object * get_var_from_fbgc_ref_object(struct fbgc_object *ref){
	if(ref == NULL){
		cprintf(100,"Error reference object is not defined !\n");
	}
	else if(ref->type == REFERENCE){
		return cast_fbgc_object_as_ref(ref)->content->next;
	}
	return (struct fbgc_object*) ref; 	
}

void print_fbgc_ref_object(struct fbgc_object * ref){
	struct fbgc_str_object * obj = (struct fbgc_str_object *)(cast_fbgc_object_as_ref(ref)->content);
	cprintf(100,"{%s",obj->content);
	cprintf(111,":");
	if(obj->base.next != NULL){
		print_fbgc_object(obj->base.next);
		cprintf(011,"}");
	}
	else cprintf(111,"NULL}");
	

}
 

void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete referenced object, only delete yourself!
	free(refo);
}