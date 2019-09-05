#include "../src/fbgc.h"
#include "fbgc_math.h"
#include <math.h>

/*
#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/




struct fbgc_object * one_arg_math(struct fbgc_object * arg, double (*fun) (double)){
	assert(size_fbgc_tuple_object(arg) != 0);// return NULL;
	
	struct fbgc_object ** contents = tuple_object_content(arg);
	double dbarg = convert_fbgc_object_to_double( contents[0] );
	return new_fbgc_double_object((*fun)(dbarg));
}


#define new_fbgc_one_arg_math(fun_str,func_ptr)\
const struct fbgc_cfunction fbgc_##func_ptr##_struct  = {fun_str,fbgc_##func_ptr};\
extern struct fbgc_object * fbgc_##func_ptr(struct fbgc_object * arg) {return one_arg_math(arg,func_ptr);}\



new_fbgc_one_arg_math("sin",sin);
new_fbgc_one_arg_math("cos",cos);
new_fbgc_one_arg_math("tan",tan);

/*
extern struct fbgc_object * fbgc_pi(struct fbgc_object * arg){
	printf("I am a pi functionn\n");

	return new_fbgc_double_object(3.141592653589793);
}

*/

const struct fbgc_cfunction fbgc_math_initializer_struct = {"math",fbgc_math_initializer};
extern struct fbgc_object * fbgc_math_initializer (struct fbgc_object * cm){

	cprintf(111,"AAAA type is %s\n",object_name_array[cm->type]);

	//cm->variables = new_fbgc_array_object(0,sizeof(struct ));

	return cm;
}



//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_math_module = 
{
	//.name = "math",
	.initializer = &fbgc_math_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{	
		&fbgc_sin_struct,
		&fbgc_cos_struct,
		&fbgc_tan_struct,
		NULL
	}
};
