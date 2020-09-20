#include "fbgc.h"



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_cfun_arg * ) ){
	if(function_obj == NULL) return NULL;
	struct fbgc_cfun_object *cfuno =  (struct fbgc_cfun_object*) fbgc_malloc_object(sizeof(struct fbgc_cfun_object));
    cfuno->base.type = CFUN;
    cfuno->function = function_obj; 
    return (struct fbgc_object*) cfuno;
}

struct fbgc_object * new_fbgc_cmodule_object(const struct fbgc_object_property_holder * p){
	struct fbgc_cmodule_object * cmo = (struct fbgc_cmodule_object *) fbgc_malloc_object(sizeof(struct fbgc_cmodule_object ));
	cmo->base.type = CMODULE;
	cmo->properties = p;
	return (struct fbgc_object *) cmo;	
}


/*
void print_fbgc_cfun_object(const struct fbgc_cmodule_object * obj){
	cprintf(100,"Cmodule Object\n{");
	cprintf(100,"name:%s\n",obj->name);
	cprintf(100,"}\n");
}*/


struct fbgc_object * new_cfun_object_from_str(struct fbgc_object * field_obj, const char * str){
	
	struct fbgc_tuple_object * _lib_tuple = cast_fbgc_object_as_tuple(get_fbgc_field_cmodules(field_obj));

	for(uint8_t i = 0; i<size_fbgc_tuple_object(_lib_tuple); ++i){
		const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(_lib_tuple->content[i]);
	
		int8_t w = _find_property(p->bits,_BIT_METHODS);
		if(w != -1){
			const struct fbgc_object_method * methods = p->properties[w].methods;
			uint8_t len = methods->len;
			while(len--){
				if(strcmp(methods->method[len].name,str) == 0){
					return new_fbgc_cfun_object(methods->method[len].function);
				}
			}
		}

		/*w = _find_property(p->bits,_BIT_MEMBERS);
		if(w != -1){
			const struct fbgc_object_member *members = p->properties[w].members;
			uint8_t len = members->len;
			
			while(len--){
				if(!strcmp(members->member[len].name,str)){
					return members->member[len].function(o,NULL);
				}
			}
		}*/

	}
	return NULL;
}


void print_fbgc_cmodule(const struct fbgc_cmodule * obj){
	/*cprintf(100,"[Cmodule Object]:{%s}\n",obj->initializer.name);
	
	cprintf(111,"Functions:\n");
	for (const struct fbgc_cfunction * cc = obj->functions; cc->name != NULL; ++cc){
		cprintf(101,"{%s}\n",cc->name);
	}
	cprintf(100,"-----\n");*/
}
