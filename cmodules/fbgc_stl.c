#include "../src/fbgc.h"
#include "fbgc_stl.h"

/*
#define declare_new_fbgc_cfunction(fun_name)\
struct fbgc_object * fun_name(struct fbgc_object * sm);\
extern const struct fbgc_cfunction fun_name##_struct

#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/

//new_fbgc_cfunction(fbgc_print,"print")


new_fbgc_cfunction(fbgc_len,"len")
{
	if(argc == 1){
		return get_length_fbgc_object(arg[0]);
	}
	else cprintf(100,"<len> takes only 1 argument!\n");
	return NULL;
}



new_fbgc_cfunction(fbgc_matrix,"matrix"){

	return NULL;
}




const struct fbgc_cfunction fbgc_stl_initializer_struct = {"stl",fbgc_stl_initializer};
extern struct fbgc_object * fbgc_stl_initializer (struct fbgc_object ** arg,int argc){
	return arg;
}



//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_stl_module = 
{
	.initializer = &fbgc_stl_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_len_struct,&fbgc_matrix_struct,NULL
	}
};



/*
struct fbgc_object * fbgc_io_module_init(){
	struct fbgc_cmodule_object * fbgc_io_module = (struct fbgc_cmodule_object *)malloc(sizeof(fbgc_cmodule_object));
	fbgc_io_module.base->next = NULL;
	fbgc_io_module.base->type = UNKNOWN;
	fbgc_io_module->name; 
}*/
 
