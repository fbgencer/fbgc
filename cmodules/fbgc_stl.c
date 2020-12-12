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

#include "fbgc_stl.h"



static int8_t _verify_one_arg(struct fbgc_cfun_arg * arg,struct fbgc_object ** obj){
	return parse_tuple_content(arg,"!o",obj) != -1;
}


static
struct fbgc_object * fbgc_stl_assert(struct fbgc_cfun_arg * arg){

	if(parse_tuple_content(arg,"!b|bs|i|is|d|ds") == -1){
		return NULL;
	}


	if(!convert_fbgc_object_to_logic(arg->arg[0])){
		if(arg->argc>1){
			FBGC_LOGE("%s\n",content_fbgc_str_object(arg->arg[1]) );
		}
		assert(0);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_load(struct fbgc_cfun_arg * arg){
	//There are 3 states
	// load('module_name') : just returns module : state 0
	// load('module_name','*') : loads all in the field object, state : 1
	// load('module_name','fun1','fun2','funN') : loads desired functions : state : 2
	//XX last one cannot add desired variables, we need to fix it


	
	if(parse_tuple_content(arg,"!s+") == -1){
		return NULL;
	}

	struct fbgc_object ** args = arg->arg;
	uint8_t argc = arg->argc;

	struct fbgc_object * res = NULL;
	const char * libname = content_fbgc_str_object(args[0]);

	if(argc == 1){
			res = fbgc_load_module(libname,NULL,0);
			if(res == NULL){
				//seek in files
				res = fbgc_load_file(libname,NULL,0);
			}
		return res;
	}
		
	if(!my_strcmp(content_fbgc_str_object(args[1]),"*")){
		res = fbgc_load_module(libname,NULL,1);
		if(res == NULL){
			res = fbgc_load_file(libname,NULL,1);
		}	
		return res;
	}

	//fbgc_load_module_specific(const char * module_name, const char * fun_name){
	while(--argc){
		res = fbgc_load_module(libname,content_fbgc_str_object(args[argc]),2);
		if(res == NULL){
			res = fbgc_load_file(libname,content_fbgc_str_object(args[argc]),2);
		}	
	}
	
	return res;
	//load_module_in_field_object(main_field,&fbgc_math_module);
}


static
struct fbgc_object * fbgc_stl_len(struct fbgc_cfun_arg * arg){

	struct fbgc_object * o = NULL;
	if(_verify_one_arg(arg,&o)){
		return abs_operator_fbgc_object(o);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_id(struct fbgc_cfun_arg * arg){
	struct fbgc_object * o = NULL;
	if(_verify_one_arg(arg,&o)){
		fprintf(stdout,"%p\n",o);
		return fbgc_nil_object;
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_type(struct fbgc_cfun_arg * arg){
	struct fbgc_object * o = NULL;
	if(_verify_one_arg(arg,&o)){
		return new_fbgc_int_object(o->type);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_int(struct fbgc_cfun_arg * arg){
	struct fbgc_object * o = NULL;
	if(_verify_one_arg(arg,&o)){
		int d = convert_fbgc_object_to_int(o);
		return new_fbgc_int_object(d);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_double(struct fbgc_cfun_arg * arg){
	struct fbgc_object * o = NULL;
	if(_verify_one_arg(arg,&o)){
		double d = convert_fbgc_object_to_double(o);
		return new_fbgc_double_object(d);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_tuple(struct fbgc_cfun_arg * arg){

	struct fbgc_object * obj;
	int8_t level = parse_tuple_content(arg,"i|s|r",&obj);
	if(level == -1){
		return NULL;
	}

	switch(level){
		case 0:{
			//obj is int
			return new_fbgc_tuple_object(cast_fbgc_object_as_int(obj)->content);
		}
		case 1:{
			//obj is str
			struct fbgc_object * t =  new_fbgc_tuple_object( length_fbgc_str_object(obj) );
			for(int i = 0; i < length_fbgc_str_object(obj) ; ++i )
				content_fbgc_tuple_object(t)[i] = get_object_in_fbgc_str_object(obj,i,1);
			size_fbgc_tuple_object(t) = length_fbgc_str_object(obj);				
			return t;			
		}
		case 2:{
			assert(0); //not tested
			//obj is range
			struct fbgc_range_object * ran = (struct fbgc_range_object*) obj;
			int start = cast_fbgc_object_as_int(ran->start)->content;
			int end = cast_fbgc_object_as_int(ran->end)->content;
			int step = ran->step == NULL ? 1 : cast_fbgc_object_as_int(ran->step)->content;

			int sz = (double)(end-start)/(step * 1.0);

			struct fbgc_object * tp = new_fbgc_tuple_object(sz);
			for(int i = start; i<end; i+=step){
				set_object_in_fbgc_tuple_object(tp,new_fbgc_int_object(i), i);
			}

			size_fbgc_tuple_object(tp) = sz;

			return tp;
		}
	}
	return NULL;
}


static
struct fbgc_object * check_type(struct fbgc_cfun_arg * arg, fbgc_token t){
	uint8_t res = arg->argc;
	uint8_t argc = arg->argc;
	while(argc--){
		res &= (get_fbgc_object_type(arg->arg[argc]) == t); 
	}
	return new_fbgc_logic_object(res);
}

static
struct fbgc_object * fbgc_stl_is_int(struct fbgc_cfun_arg * arg){
	return check_type(arg,INT);
}
static
struct fbgc_object * fbgc_stl_is_double(struct fbgc_cfun_arg * arg){
	return check_type(arg,DOUBLE);
}
static
struct fbgc_object * fbgc_stl_is_complex(struct fbgc_cfun_arg * arg){
	return check_type(arg,COMPLEX);
}
static
struct fbgc_object * fbgc_stl_is_string(struct fbgc_cfun_arg * arg){
	return check_type(arg,STRING);
}
static
struct fbgc_object * fbgc_stl_is_tuple(struct fbgc_cfun_arg * arg){
	return check_type(arg,TUPLE);
}
static
struct fbgc_object * fbgc_stl_is_matrix(struct fbgc_cfun_arg * arg){
	return check_type(arg,MATRIX);
}



static struct fbgc_object_method _fbgc_stl_methods = {
	.len = 14,
	.method = {
		{.name = "assert", .function = &fbgc_stl_assert},
		{.name = "len", .function = &fbgc_stl_len},
		{.name = "id", .function = &fbgc_stl_id},
		{.name = "type", .function = &fbgc_stl_type},
		{.name = "int", .function = &fbgc_stl_int},
		{.name = "double", .function = &fbgc_stl_double},
		{.name = "tuple", .function = &fbgc_stl_tuple},
		//{.name = "matrix", .function = &fbgc_stl_matrix},
		{.name = "is_int", .function = &fbgc_stl_is_int},
		{.name = "is_double", .function = &fbgc_stl_is_double},
		{.name = "is_string", .function = &fbgc_stl_is_string},
		{.name = "is_complex", .function = &fbgc_stl_is_complex},
		{.name = "is_tuple", .function = &fbgc_stl_is_tuple},
		{.name = "is_matrix", .function = &fbgc_stl_is_matrix},
		{.name = "load", .function = &fbgc_stl_load},
	}
};

const struct fbgc_object_property_holder _fbgc_stl_property_holder = {
	.bits = 
	_BIT_METHODS |
	_BIT_NAME 
	,
	.properties ={
		{.methods = &_fbgc_stl_methods},
		{.name = "stl"},
	}
};