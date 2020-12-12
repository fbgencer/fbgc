// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc_io.h"



static struct fbgc_object * fbgc_io_print(struct fbgc_cfun_arg * arg){
	
	static const char * keywords [] = {"end","sep",NULL};
	struct fbgc_object * end = NULL, * sep = NULL;

	if(parse_keywordargs_content(arg,keywords,"oo",&end,&sep) == -1){
		return NULL;
	}

	for(uint8_t i = 0; i<arg->argc; ++i){
		 
		printf_fbgc_object(arg->arg[i]);
		
		//Change this printing seperator part
		if(i != arg->argc-1){
			if(sep)
				printf_fbgc_object(sep);
			else
				fprintf(stdout," ");
		}
	}
	if(end)
		printf_fbgc_object(end);
	else
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

static struct fbgc_object_method _fbgc_io_methods = {
	.len = 2,
	.method = {
		{.name = "print", .function = &fbgc_io_print},
		{.name = "read", .function = &fbgc_io_read},
	}
};

const struct fbgc_object_property_holder _fbgc_io_property_holder = {
	.bits = 
	_BIT_NAME |
	_BIT_METHODS
	,
	.properties ={
		{.methods = &_fbgc_io_methods},
		{.name = "io"}
	}
};
