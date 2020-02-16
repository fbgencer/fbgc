#include "fbgc.h"



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object **,int ) ){
	if(function_obj == NULL) return NULL;
	struct fbgc_cfun_object *cfuno =  (struct fbgc_cfun_object*) fbgc_malloc(sizeof(struct fbgc_cfun_object));
    cfuno->base.type = CFUN;
   //cfuno->base.next = NULL;
    cfuno->function = function_obj; 
    return (struct fbgc_object*) cfuno;
}

struct fbgc_object * new_fbgc_cmodule_object(const struct fbgc_cmodule * cm){
	struct fbgc_cmodule_object * cmo = (struct fbgc_cmodule_object *) fbgc_malloc(sizeof(struct fbgc_cmodule_object ));
	cmo->base.type = CMODULE;
	cmo->module = cm;
	return (struct fbgc_object *) cmo;	
}


/*
void print_fbgc_cfun_object(const struct fbgc_cmodule_object * obj){
	cprintf(100,"Cmodule Object\n{");
	cprintf(100,"name:%s\n",obj->name);
	cprintf(100,"}\n");
}*/


struct fbgc_object * new_cfun_object_from_str(struct fbgc_object * field_obj,const char * str){

	struct fbgc_ll_object * ll = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(field_obj)->modules );
	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)ll->base.next;
	while(cm!= NULL && (struct fbgc_object * )cm != ll->tail){
		const struct fbgc_cfunction * cc = cm->module->functions[0];
		//cprintf(111,"Functions:\n");
		for (int i = 1; cc!=NULL; ++i){
			//optimize strlen part
			if(!my_strcmp(str,cc->name) ){
				return new_fbgc_cfun_object(cc->function);
			} 
			//cprintf(101,"{%s}\n",cc->name);
			cc = cm->module->functions[i];
		}
		cm = (struct fbgc_cmodule_object * )cm->base.next;
	}
	return NULL;
}


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

