#include "../src/fbgc.h"
#include "fbgc_io.h"


static struct fbgc_object * fbgc_io_print(struct fbgc_object ** arg, int argc){
	
	for(size_t i = 0; i<argc; ++i){
		 
		//print_fbgc_object(arg[i]);
		const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(arg[i]);

		int8_t w = _find_property(p->bits,_BIT_PRINT);
		if(w != -1){
			p->properties[w].print(arg[i]);
		}
		else{
			//put an error
			assert(0);
		}
		//printf_fbgc_object(arg[i]);
		fprintf(stdout," ");
	}
	fprintf(stdout,"\n");

	return __fbgc_nil;
}

static struct fbgc_object * fbgc_io_read(struct fbgc_object ** arg, int argc){

	assert(argc > 0);
	
	printf_fbgc_object(arg[0]);
	char *str_content;
	assert(scanf("%ms",&str_content) == 1);
	
	struct fbgc_object * res = new_fbgc_str_object(str_content);
	free(str_content);
	return res;
}


const struct fbgc_cmodule fbgc_io_module = 
{
	.initializer = {.name = "io", .function = NULL},
	.functions = {
		{.name = "print", .function = &fbgc_io_print},
		{.name = "read", .function = &fbgc_io_read},
		{NULL,NULL}
	}
};