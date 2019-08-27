#include "fbgc.h"


struct fbgc_object * new_fbgc_ref_object(struct fbgc_object ** address){
	struct fbgc_ref_object *refo =  (struct fbgc_ref_object*) fbgc_malloc(sizeof(struct fbgc_ref_object));
    refo->base.type = REFERENCE;
    refo->base.next = NULL;
    refo->address = address;
    return (struct fbgc_object*) refo;
}


struct fbgc_object * get_var_from_fbgc_ref_object(struct fbgc_object *ref){

}



void print_fbgc_ref_object(struct fbgc_object * ref){
	#define refo cast_fbgc_object_as_ref(ref)
	print_fbgc_object((refo->address) );
	#undef refo

}
 

void free_fbgc_ref_object(struct fbgc_object * refo){
	//do not delete referenced object, only delete yourself!
	//free(refo);
}

