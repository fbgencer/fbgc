#include "../src/fbgc.h"
#include "fbgc_math.h"
#include <math.h>

/*
#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/




struct fbgc_object * one_arg_math(struct fbgc_object ** arg,int argc, double (*fun) (double)){
	double dbarg = convert_fbgc_object_to_double( arg[0] );
	return new_fbgc_double_object((*fun)(dbarg));
}


#define new_fbgc_one_arg_math(fun_str,func_ptr)\
const struct fbgc_cfunction fbgc_##func_ptr##_struct = {fun_str,fbgc_##func_ptr};\
extern struct fbgc_object * fbgc_##func_ptr(struct fbgc_object ** arg, int argc) {\
return (argc == 1) ? one_arg_math(arg,argc,func_ptr) : NULL ;}\

new_fbgc_one_arg_math("sin",sin);
new_fbgc_one_arg_math("cos",cos);
new_fbgc_one_arg_math("tan",tan);

new_fbgc_cfunction(fbgc_sqrt,"sqrt")
{
	double dbarg = convert_fbgc_object_to_double( arg[0] );
	if(dbarg < 0) return new_fbgc_complex_object(0,sqrt(-dbarg));
	return new_fbgc_double_object(sqrt(dbarg));
}



/*
const struct fbgc_cfunction fbgc_math_initializer_struct = {"math",fbgc_math_initializer};
extern struct fbgc_object * fbgc_math_initializer (struct fbgc_object * cm)*/
new_fbgc_cfunction(fbgc_math_initializer,"math")
{
	return arg;
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
		&fbgc_sqrt_struct,
		NULL
	}
};
