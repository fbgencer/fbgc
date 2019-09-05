#include "fbgc.h"



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object *) ){
	if(function_obj == NULL) return NULL;
	struct fbgc_cfun_object *cfuno =  (struct fbgc_cfun_object*) fbgc_malloc(sizeof(struct fbgc_cfun_object));
    cfuno->base.type = CFUN;
    cfuno->base.next = NULL;
    cfuno->function = function_obj; 
    return (struct fbgc_object*) cfuno;
}

/*
void print_fbgc_cfun_object(const struct fbgc_cmodule_object * obj){
	cprintf(100,"Cmodule Object\n{");
	cprintf(100,"name:%s\n",obj->name);
	cprintf(100,"}\n");
}*/

void print_fbgc_cmodule(const struct fbgc_cmodule * obj){
	const struct fbgc_cfunction * cc  = obj->initializer;
	cprintf(100,"[Cmodule Object]:{%s}\n",cc->name);
	cc = obj->functions[0];
	cprintf(111,"Functions:\n");
	for (int i = 1; cc!=NULL; ++i){
		cprintf(101,"{%s}\n",cc->name);
		cc = obj->functions[i];
	}
	cprintf(100,"-----\n");
}

void free_fbgc_cfun_object(struct fbgc_object * obj){
	//free(obj);
}