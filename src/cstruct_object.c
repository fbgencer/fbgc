#include "fbgc.h"


struct fbgc_object * new_fbgc_cstruct_object(size_t bsz,const struct fbgc_cmodule * parent){
	struct fbgc_cstruct_object * so =  (struct fbgc_cstruct_object*) fbgc_malloc(sizeof(struct fbgc_cstruct_object)+bsz);
    so->base.type = CSTRUCT;
    so->block_size = bsz;
    so->parent = parent;
    so->cstruct = ((char*)&so->cstruct + sizeof(so->cstruct));  
    return (struct fbgc_object*) so;
}
