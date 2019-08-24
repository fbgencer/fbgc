#include "fbgc.h"

struct fbgc_object * 
new_fbgc_fun_object(){
	struct fbgc_fun_object * funo =  (struct fbgc_fun_object*) fbgc_malloc(size_fbgc_fun_object);
    funo->base.type = FUN;
    funo->base.next = NULL;
    
    funo->code = NULL;
    funo->no_arg = -1;

    return (struct fbgc_object*) funo;
}



void print_fbgc_fun_object(struct fbgc_object * obj){
   	
   	struct fbgc_fun_object * funo = cast_fbgc_object_as_fun(obj);

   	if(funo->code != NULL && funo->code->type != TUPLE){

   		cprintf(010,"[Arg#:%d |",funo->no_arg);
 		struct fbgc_object * iter = funo->code;
 		while(iter != obj){
 			print_fbgc_object(iter);
 			iter = iter->next;
 		}
 		cprintf(010,"]");
   	}

     
}