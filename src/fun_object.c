#include "fbgc.h"

struct fbgc_object * 
new_fbgc_fun_object(struct fbgc_object * arg_start, struct fbgc_object * arg_end){
	struct fbgc_fun_object * funo =  (struct fbgc_fun_object*) fbgc_malloc(size_fbgc_fun_object);
    funo->base.type = FUN;
    funo->base.next = NULL;
    
    funo->code = NULL;
    
    funo->locals = NULL;//new_fbgc_tuple_object

    return (struct fbgc_object*) funo;
}
