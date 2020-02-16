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

new_fbgc_cfunction(fbgc_print,"print")
{
	
	for(size_t i = 0; i<argc; ++i){
		printf_fbgc_object(arg[i]);
		fprintf(stdout," ");
	}
	fprintf(stdout,"\n");

	return NULL;
}

new_fbgc_cfunction(fbgc_read,"read"){

	assert(argc > 0);
	
	printf_fbgc_object(arg[0]);
	char *str_content;
	assert(scanf("%ms",&str_content) == 1);
	
	struct fbgc_object * res = new_fbgc_str_object(str_content);
	free(str_content);
	return res;
}



//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// CHANGE THIS INITIALIZER
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

const struct fbgc_cfunction fbgc_io_initializer_struct = {"io",fbgc_io_initializer};
extern struct fbgc_object * fbgc_io_initializer (struct fbgc_object ** arg, int argc){
	return NULL;
}


/*
new_fbgc_cfunction(fbgc_io_initializer,"io"){
	return arg;
}*/

//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_io_module = 
{
	.initializer = &fbgc_io_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_print_struct,
		&fbgc_read_struct,
		NULL
	}
};



/*
struct fbgc_object * fbgc_io_module_init(){
	struct fbgc_cmodule_object * fbgc_io_module = (struct fbgc_cmodule_object *)malloc(sizeof(fbgc_cmodule_object));
	fbgc_io_module.base->next = NULL;
	fbgc_io_module.base->type = UNKNOWN;
	fbgc_io_module->name; 
}*/
 
