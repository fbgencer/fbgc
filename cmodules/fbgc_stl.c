#include "../src/fbgc.h"
#include "fbgc_stl.h"


static int8_t _verify_one_arg(struct fbgc_object ** arg, int argc){
	return parse_tuple_content(arg,argc,"!o") != -1;
}



static
struct fbgc_object * fbgc_stl_assert(struct fbgc_object ** arg, int argc){

	if(parse_tuple_content(arg,argc,"!b|bs") == -1){
		return NULL;
	}


	if(!cast_fbgc_object_as_logic(arg[0])->content){
		if(argc>1){
			FBGC_LOGE("%s\n",content_fbgc_str_object(arg[1]) );
		}
		assert(0);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_load(struct fbgc_object ** arg, int argc){
	//There are 3 states
	// load('module_name') : just returns module : state 0
	// load('module_name','*') : loads all in the field object, state : 1
	// load('module_name','fun1','fun2','funN') : loads desired functions : state : 2
	//XX last one cannot add desired variables, we need to fix it

	
	if(parse_tuple_content(arg,argc,"!s+") == -1){
		return NULL;
	}

	struct fbgc_object * res = NULL;
	const char * libname = content_fbgc_str_object(arg[0]);

	if(argc == 1){
			res = fbgc_load_module(libname,NULL,0);
			if(res == NULL){
				//seek in files
				res = fbgc_load_file(libname,NULL,0);
			}
		return res;
	}
		
	if(!my_strcmp(content_fbgc_str_object(arg[1]),"*")){
		res = fbgc_load_module(libname,NULL,1);
		if(res == NULL){
			res = fbgc_load_file(libname,NULL,1);
		}	
		return res;
	}

	//fbgc_load_module_specific(const char * module_name, const char * fun_name){
	while(--argc){
		res = fbgc_load_module(libname,content_fbgc_str_object(arg[argc]),2);
		if(res == NULL){
			res = fbgc_load_file(libname,content_fbgc_str_object(arg[argc]),2);
		}	
	}
	
	return res;
	//load_module_in_field_object(main_field,&fbgc_math_module);
}


static
struct fbgc_object * fbgc_stl_len(struct fbgc_object ** arg, int argc){

	if(_verify_one_arg(arg,argc)){
		return abs_operator_fbgc_object(arg[0]);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_id(struct fbgc_object ** arg, int argc){
	if(_verify_one_arg(arg,argc)){
		fprintf(stdout,"%p\n",arg[0]);
		return fbgc_nil_object;
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_type(struct fbgc_object ** arg, int argc){
	if(_verify_one_arg(arg,argc)){
		return new_fbgc_int_object(arg[0]->type);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_int(struct fbgc_object ** arg, int argc){
	if(_verify_one_arg(arg,argc)){
		int d = convert_fbgc_object_to_int(arg[0]);
		return new_fbgc_int_object(d);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_double(struct fbgc_object ** arg, int argc){
	if(_verify_one_arg(arg,argc)){
		double d = convert_fbgc_object_to_double(arg[0]);
		return new_fbgc_double_object(d);
	}
	return NULL;
}

static
struct fbgc_object * fbgc_stl_tuple(struct fbgc_object ** arg, int argc){

	struct fbgc_object * obj;
	int8_t level = parse_tuple_content(arg,argc,"i|s|r",&obj);
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


struct fbgc_object * check_type(struct fbgc_object ** arg,int argc, fbgc_token t){
	uint8_t res = argc;
	while(argc--){
		res &= (get_fbgc_object_type(arg[argc]) == t); 
	}
	return new_fbgc_logic_object(res);
}

static
struct fbgc_object * fbgc_stl_is_int(struct fbgc_object ** arg, int argc){
	return check_type(arg,argc,INT);
}
static
struct fbgc_object * fbgc_stl_is_double(struct fbgc_object ** arg, int argc){
	return check_type(arg,argc,DOUBLE);
}
static
struct fbgc_object * fbgc_stl_is_complex(struct fbgc_object ** arg, int argc){
	return check_type(arg,argc,COMPLEX);
}
static
struct fbgc_object * fbgc_stl_is_string(struct fbgc_object ** arg, int argc){
	return check_type(arg,argc,STRING);
}
static
struct fbgc_object * fbgc_stl_is_tuple(struct fbgc_object ** arg, int argc){
	return check_type(arg,argc,TUPLE);
}
static
struct fbgc_object * fbgc_stl_is_matrix(struct fbgc_object ** arg, int argc){
	return check_type(arg,argc,MATRIX);
}

/*
new_fbgc_cfunction(fbgc_matrix,"matrix"){
	
	if(argc == 3){
		size_t r = convert_fbgc_object_to_int(arg[0]);
		size_t c = convert_fbgc_object_to_int(arg[1]);
		return new_fbgc_matrix_object(DOUBLE,r,c,convert_fbgc_object_to_int(arg[2]));
	}
	else cprintf(100,"<matrix> takes only 3 argument, %d given !\n",argc);
	return NULL;
}


new_fbgc_cfunction(fbgc_mem,"mem"){
	if(argc == 0){
		print_fbgc_memory_block();
		return __fbgc_nil;
	}
	else cprintf(100,"<mem> takes only 0 argument, %d given !\n",argc);
	return NULL;
}


new_fbgc_cfunction(fbgc_locals,"locals"){
	if(argc == 0){
		print_field_object_locals(current_field);
		return __fbgc_nil;
	}
	else cprintf(100,"<locals> takes only 0 argument, %d given !\n",argc);
	return NULL;
}
*/

const struct fbgc_cmodule fbgc_stl_module = 
{
	.initializer = {.name = "stl", .function = NULL},
	.functions = {
		
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
		{NULL,NULL},
	}
};



// //Work on this, is it possible to cast ?
// const struct fbgc_cmodule fbgc_stl_module = 
// {
// 	.initializer = &fbgc_stl_initializer_struct,
// 	.functions = (const struct fbgc_cfunction*[])
// 	{
// 		&fbgc_tuple_struct,
// 		&fbgc_matrix_struct,
// 		&fbgc_type_struct,
// 		&fbgc_mem_struct,
// 		&fbgc_locals_struct,
// 		NULL
// 	}
// };

