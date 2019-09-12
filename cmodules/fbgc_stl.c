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

	struct fbgc_object ** contents = tuple_object_content(arg);
	
	if(size_fbgc_tuple_object(arg) == 1){
		int sz = 0;

		switch(contents[0]->type){
			case STRING:
			{
				return new_fbgc_int_object(cast_fbgc_object_as_str(contents[0])->len);
			}				
			case TUPLE:
			{

				return new_fbgc_int_object( size_fbgc_tuple_object(contents[0]) );
			}
			
			case MATRIX:
			{
				struct fbgc_object * sz_tuple = new_fbgc_tuple_object(2);
				struct fbgc_object ** aa = tuple_object_content(sz_tuple);
				aa[0] = new_fbgc_int_object(cast_fbgc_object_as_matrix(contents[0])->row);
				aa[1] = new_fbgc_int_object(cast_fbgc_object_as_matrix(contents[0])->column);
				size_fbgc_tuple_object(sz_tuple) = 2;
				return sz_tuple;
			}
			default:
			{
				cprintf(100,"Not an iterable object type %s\n",object_name_array[contents[0]->type]);
				return NULL;
			}	
		}
	}
	else cprintf(100,"<len> takes only 1 argument!\n");


	return NULL;
}



new_fbgc_cfunction(fbgc_matrix,"matrix"){

	return NULL;
}




const struct fbgc_cfunction fbgc_stl_initializer_struct = {"stl",fbgc_stl_initializer};
extern struct fbgc_object * fbgc_stl_initializer (struct fbgc_object * arg){
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
 
