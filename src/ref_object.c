#include "fbgc.h"

struct fbgc_object * new_fbgc_ref_object(){
	struct fbgc_ref_object *refo =  (struct fbgc_ref_object*) malloc(sizeof(struct fbgc_ref_object));
    refo->base.type = REFERENCE;
    refo->base.next = NULL;
    refo->content = NULL; //content shows address of the object
    return (struct fbgc_object*) refo;
}

struct fbgc_object * assign_fbgc_ref_object(struct fbgc_object *ref, struct fbgc_object * obj){
	struct fbgc_ref_object * refo = cast_fbgc_object_as_ref(ref);
	refo->content = obj;
	return (struct fbgc_object*) refo; 
}

void print_fbgc_ref_object(struct fbgc_object * ref){
	struct fbgc_str_object * obj = cast_fbgc_object_as_ref(ref)->content;
	cprintf(111,"{Ref-> Name:%s} Content=",obj->content);
	if(obj->base.next != NULL) print_fbgc_object(obj->base.next);

}



void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete reference object, only delete yourself!
	free(refo);
}