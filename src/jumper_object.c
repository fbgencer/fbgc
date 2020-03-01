#include "fbgc.h"


struct
fbgc_object * new_fbgc_jumper_object(fbgc_token tok){
	struct fbgc_jumper_object *o = fbgc_malloc(sizeof(struct fbgc_jumper_object));
    o->base.type = tok;
    o->content = NULL;
    return (struct fbgc_object*) o;
}


