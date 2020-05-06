#include "fbgc.h"



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object **,int ) ){
	if(function_obj == NULL) return NULL;
	struct fbgc_cfun_object *cfuno =  (struct fbgc_cfun_object*) fbgc_malloc_object(sizeof(struct fbgc_cfun_object));
    cfuno->base.type = CFUN;
    cfuno->function = function_obj; 
    return (struct fbgc_object*) cfuno;
}

struct fbgc_object * new_fbgc_cmodule_object(const struct fbgc_cmodule * cm){
	struct fbgc_cmodule_object * cmo = (struct fbgc_cmodule_object *) fbgc_malloc_object(sizeof(struct fbgc_cmodule_object ));
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


struct fbgc_object * new_cfun_object_from_str(struct fbgc_object * field_obj, const char * str){

	struct fbgc_ll * ll = _cast_llbase_as_ll( cast_fbgc_object_as_field(field_obj)->modules );
	struct fbgc_ll_constant * xx = (struct fbgc_ll_constant *)ll->base.next;
	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)xx->content;


	while(xx != NULL && (struct fbgc_ll_base*)xx != ll->tail){
		
		for (const struct fbgc_cfunction * cc = cm->module->functions; cc->name != NULL; ++cc){
			if(!my_strcmp(str,cc->name) ){
				//cprintf(111,"Catch!\n");
				return new_fbgc_cfun_object(cc->function);
			}
		}
		xx = (struct fbgc_ll_constant *)xx->base.next;
		cm = (struct fbgc_cmodule_object *)xx->content;
	}

	return NULL;
}


void print_fbgc_cmodule(const struct fbgc_cmodule * obj){
	cprintf(100,"[Cmodule Object]:{%s}\n",obj->initializer.name);
	
	cprintf(111,"Functions:\n");
	for (const struct fbgc_cfunction * cc = obj->functions; cc->name != NULL; ++cc){
		cprintf(101,"{%s}\n",cc->name);
	}
	cprintf(100,"-----\n");
}

void free_fbgc_cfun_object(struct fbgc_object * obj){
	//free(obj);
}


struct fbgc_object * get_set_fbgc_cmodule_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs){
	const struct fbgc_cmodule * cm = cast_fbgc_object_as_cmodule(o)->module;
	
	for (const struct fbgc_cfunction * cc = cm->functions; cc->name != NULL; ++cc){
		if(!my_strcmp(str,cc->name) ){
			return new_fbgc_cfun_object(cc->function);
		}
	}
	return NULL;
}


const struct fbgc_object_property_holder fbgc_cmodule_object_property_holder = {
	.bits = 
	_BIT_GET_SET_MEMBER
	,
	.properties ={
		{.get_set_member = &get_set_fbgc_cmodule_object_member},			
	}
};
