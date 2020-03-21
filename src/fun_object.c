#include "fbgc.h"

struct fbgc_object * 
new_fbgc_fun_object(){ 
	struct fbgc_fun_object * funo =  (struct fbgc_fun_object*) fbgc_malloc(sizeof_fbgc_fun_object());
    funo->base.type = FUN;
    funo->code = NULL;
    funo->no_arg = -1;
    funo->no_locals = -1;

    return (struct fbgc_object*) funo;
}



void print_fbgc_fun_object(struct fbgc_object * obj){
    struct fbgc_fun_object * funo = cast_fbgc_object_as_fun(obj);

    if(funo->code != NULL && funo->code->type != TUPLE){

      cprintf(010,"[Arg#:%d Local#:%d|",funo->no_arg,funo->no_locals);
      _print_fbgc_ll_code(funo->code,NULL);
      cprintf(010,"]");
    }
}


void fprintf_fbgc_fun_object(struct fbgc_object * obj){
    
    struct fbgc_fun_object * funo = cast_fbgc_object_as_fun(obj);

    if(funo->code != NULL && funo->code->type != TUPLE){

      cprintf(010,"[Arg#:%d Local#:%d|",funo->no_arg,funo->no_locals);
      _print_fbgc_ll_code(funo->code,NULL);
      cprintf(010,"]");
    }
}