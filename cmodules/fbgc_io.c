#include "../src/fbgc.h"
#include "fbgc_io.h"



static struct fbgc_object * fbgc_io_print(struct fbgc_cfun_arg * arg){
	

	static const char * keywords [] = {"end","sep",NULL};
	struct fbgc_object * kwargs = NULL;
	if(arg->kwargs_flag)
		kwargs = arg->arg[arg->argc];


	uint8_t _argc = arg->argc-1;
	for(uint8_t i = 0; i<arg->argc; ++i){
		 
		printf_fbgc_object(arg->arg[i]);
		
		//Change this printing seperator part
		if(i != _argc){
			if(kwargs){
				struct fbgc_object * sepobj = fbgc_map_object_lookup_str(kwargs,keywords[1]);
				if(sepobj != NULL){
					printf_fbgc_object(sepobj);
					continue;
				}

			}
			fprintf(stdout," ");
		}
		
	}
	if(kwargs){
		struct fbgc_object * endobj = fbgc_map_object_lookup_str(kwargs,keywords[0]);
		if(endobj != NULL){
			printf_fbgc_object(endobj);
			return __fbgc_nil;
		}
		
	}
	fprintf(stdout,"\n");

	return __fbgc_nil;
}


static struct fbgc_object * fbgc_io_read(struct fbgc_cfun_arg * arg){

	if(parse_tuple_content(arg,"!s") == -1)
        return NULL;
	
	printf_fbgc_object(arg->arg[0]);
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