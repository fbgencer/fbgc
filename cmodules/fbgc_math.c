#include "../src/fbgc.h"
#include "fbgc_math.h"
#include <math.h>

/*
#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/




struct fbgc_object * one_arg_math(struct fbgc_object ** arg,int argc, double (*fun) (double)){
	switch(arg[0]->type)
	{
		case INT:
		case DOUBLE:
		{
			double dbarg = convert_fbgc_object_to_double( arg[0] );
			return new_fbgc_double_object((*fun)(dbarg));
		}
		case TUPLE:
		{
			size_t sz = size_fbgc_tuple_object(arg[0]);
			struct fbgc_object * res_tp = new_fbgc_tuple_object( sz );
			size_fbgc_tuple_object(res_tp) = sz;
			for(int i = 0; i <  sz; ++i ){
				double dbarg = convert_fbgc_object_to_double( get_object_in_fbgc_tuple_object(arg[0],i) );				
				set_object_in_fbgc_tuple_object(res_tp,new_fbgc_double_object((*fun)(dbarg)), i);
			}
			return res_tp;
		}
		case MATRIX:
		{
			struct fbgc_matrix_object * m = (struct fbgc_matrix_object *)arg[0];
			size_t r = m->row;
			size_t c = m->column;
			struct fbgc_object * res_m = new_fbgc_matrix_object(r,c,UNINITIALIZED_MATRIX);
			double * mc = content_fbgc_matrix_object(m);
			double * resc = content_fbgc_matrix_object(res_m);
			for(size_t i = 0; i<r; ++i){
				for(size_t j = 0; j<c; ++j){
					size_t index = i*c+j;
					resc[index] = (*fun)(mc[index]);
				}
			}
			return res_m;			
		}
		default:
			return NULL;
	}
}


#define new_fbgc_one_arg_math(fun_str,func_ptr)\
const struct fbgc_cfunction fbgc_##func_ptr##_struct = {fun_str,fbgc_##func_ptr};\
extern struct fbgc_object * fbgc_##func_ptr(struct fbgc_object ** arg, int argc) {\
return (argc == 1) ? one_arg_math(arg,argc,func_ptr) : NULL ;}\

new_fbgc_one_arg_math("sin",sin);
new_fbgc_one_arg_math("cos",cos);
new_fbgc_one_arg_math("tan",tan);
new_fbgc_one_arg_math("exp",exp);
new_fbgc_one_arg_math("sqrt",sqrt);

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
		&fbgc_exp_struct,
		&fbgc_sqrt_struct,
		NULL
	}
};
