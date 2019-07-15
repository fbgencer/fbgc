#include "../src/fbgc.h"
#include "fbgc_io.h"

/*
#define declare_new_fbgc_cfunction(fun_name)\
struct fbgc_object * fun_name(struct fbgc_object * sm);\
extern const struct fbgc_cfunction fun_name##_struct

#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/

//new_fbgc_cfunction(fbgc_print,"print")


static struct fbgc_object * fbgc_print(struct fbgc_object * arg)
{
	if(arg->type == TUPLE){
		for(unsigned int i = 0; i<cast_fbgc_object_as_tuple(arg)->size; i++){
			//print_fbgc_object(cast_fbgc_object_as_tuple(arg)->contents[i]);		
			//printf(" tp");
		}
	} 
	else print_fbgc_object(arg);
	//printf("\n");
	return new_fbgc_object(UNKNOWN);
}

static const struct fbgc_cfunction fbgc_print_struct  = {"print",fbgc_print};



new_fbgc_cfunction(fbgc_read,"read"){

	printf("I am a reader! but your type is :%s\n",object_name_array[arg->type] );

	return arg;
}

/*
struct fbgc_object * fbgc_io_module_init(){
	struct fbgc_cmodule_object * fbgc_io_module = (struct fbgc_cmodule_object *)malloc(sizeof(fbgc_cmodule_object));
	fbgc_io_module.base->next = NULL;
	fbgc_io_module.base->type = UNKNOWN;
	fbgc_io_module->name; 
}*/
 

//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_io_module = 
{
	.name = "io",
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_print_struct,&fbgc_read_struct,NULL
	}
};

