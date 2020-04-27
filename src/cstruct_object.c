#include "fbgc.h"


struct fbgc_object * new_fbgc_cstruct_object(size_t bsz, const struct fbgc_object_property_holder * prop){
	struct fbgc_cstruct_object * so =  (struct fbgc_cstruct_object*) fbgc_malloc(sizeof(struct fbgc_cstruct_object)+bsz);
    so->base.type = CSTRUCT;
    so->block_size = bsz;
    so->properties = prop;
    so->cstruct = ((uint8_t*)&so->cstruct + sizeof(so->cstruct));  
    return (struct fbgc_object*) so;
}

struct fbgc_object * get_set_fbgc_cstruct_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs){
	/*struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(o);
	const struct fbgc_cmodule * cm = so->parent;
	for (int i = 0; ; ++i){
		const struct fbgc_cfunction * cc = cm->functions[i];
		if(cc == NULL) break;
		
		if(!my_strcmp(str,cc->name)){
			return new_fbgc_cfun_object(cc->function);
		} 
		
	}
	return NULL;*/
}
