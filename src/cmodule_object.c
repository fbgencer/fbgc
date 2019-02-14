#include "fbgc.h"



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object *) ){
	if(function_obj == NULL) return NULL;
	struct fbgc_cfun_object *cfuno =  (struct fbgc_cfun_object*) malloc(sizeof(struct fbgc_cfun_object));
    cfuno->base.type = CFUN;
    cfuno->base.next = NULL;
    cfuno->function = function_obj; 
    return (struct fbgc_object*) cfuno;
}

void free_fbgc_cfun_object(struct fbgc_object * obj){
	free(obj);
}