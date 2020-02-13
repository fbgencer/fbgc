#include "../src/fbgc.h"
#include "fbgc_math.h"
#include <math.h>

/*
#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/


/*
>> z = 1+2j
z =  1 + 2i
>> cos(z)
ans =  2.0327 - 3.0519i
>> (exp(1j*z)+exp(-1j*z))/2
ans =  2.0327 - 3.0519i
>>
*/

struct raw_complex c_exp(struct raw_complex z){
	//complex exponantiel
	struct raw_complex res;
	double r = exp(z.real);
	res.real = r*cos(z.imag);
	res.imag = r*sin(z.imag);
	return res;
}

struct raw_complex c_cos(struct raw_complex z){
	//complex cos
	struct raw_complex u1,u2;

	u1.real = -z.imag;
	u1.imag = z.real;

	u2.real = z.imag;
	u2.imag = -z.real;
	
	u1 = c_exp(u1);
	u2 = c_exp(u2);

	u1.real = (u1.real+u2.real)/2;
	u1.imag = (u1.imag+u2.imag)/2;

	return u1;
}

struct fbgc_object * one_arg_math(struct fbgc_object ** arg,int argc, double (*fun) (double),
	struct raw_complex(*zfun) (struct raw_complex))
{
	switch(arg[0]->type)
	{
		case INT:
		case DOUBLE:
		{
			double dbarg = convert_fbgc_object_to_double( arg[0] );
			return new_fbgc_double_object((*fun)(dbarg));
		}
		case COMPLEX:
		{
			struct raw_complex z = convert_fbgc_object_to_complex(arg[0]);
			z = (*zfun)(z);
			return new_fbgc_complex_object(z.real,z.imag);
		}
		case TUPLE:
		{
			size_t sz = size_fbgc_tuple_object(arg[0]);
			struct fbgc_object * res_tp = new_fbgc_tuple_object( sz );
			size_fbgc_tuple_object(res_tp) = sz;
			struct fbgc_object ** tp_content = tuple_object_content(arg[0]);

			for(size_t i = 0; i <  sz; ++i ){
				//call recursively
				struct fbgc_object * result = one_arg_math(tp_content+i,1,fun,zfun);
				assert(result != NULL);
				set_object_in_fbgc_tuple_object(res_tp,result, i);
			}
			return res_tp;
		}
		case MATRIX:
		{
			struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) arg[0];
			struct fbgc_object * res_m = new_fbgc_matrix_object(m->sub_type,m->row,m->column,UNINITIALIZED_MATRIX);

			size_t rc = m->row*m->column;

			struct raw_complex z;

			for(size_t i = 0; i<m->row; ++i){
				for(size_t j = 0; j<m->column; ++j){
					size_t index = i * m->column + j;
					size_t c_index = rc + index;

					z.real = *(content_fbgc_matrix_object(m)+index);
					if(m->sub_type == COMPLEX){
						z.imag = *(content_fbgc_matrix_object(m)+c_index);
						struct raw_complex result = (*zfun)(z);
						*(content_fbgc_matrix_object(res_m)+index) = result.real;
						*(content_fbgc_matrix_object(res_m)+c_index) = result.imag;
					}
					else *(content_fbgc_matrix_object(res_m)+index) = (*fun)(z.real);
				}
			}
			return res_m;			
		}
		default:
			return NULL;
	}
}


#define new_fbgc_one_arg_math(fun_str,func_ptr,complex_func_ptr)\
const struct fbgc_cfunction fbgc_##func_ptr##_struct = {fun_str,fbgc_##func_ptr};\
extern struct fbgc_object * fbgc_##func_ptr(struct fbgc_object ** arg, int argc) {\
return (argc == 1) ? one_arg_math(arg,argc,func_ptr,complex_func_ptr) : NULL ;}\


new_fbgc_one_arg_math("sin",sin,NULL);
new_fbgc_one_arg_math("cos",cos,c_cos);
new_fbgc_one_arg_math("tan",tan,NULL);
new_fbgc_one_arg_math("exp",exp,c_exp);
new_fbgc_one_arg_math("sqrt",sqrt,NULL);

/*
const struct fbgc_cfunction fbgc_math_initializer_struct = {"math",fbgc_math_initializer};
extern struct fbgc_object * fbgc_math_initializer (struct fbgc_object * cm)*/
new_fbgc_cfunction(fbgc_math_initializer,"math")
{	
	return NULL;
}


new_fbgc_cfunction(fbgc_randint,"randint")
{

	srand((int) clock() );

	int min = 0;
	int max = 0;

	if(argc == 1){	
		max = convert_fbgc_object_to_int(arg[0]);
	}
	else if(argc == 2){
		min = convert_fbgc_object_to_int(arg[0]);
		max = convert_fbgc_object_to_int(arg[1]);
	}
	else{
		cprintf(100,"randint requires one or two arg\n"); 
		return NULL;
	}

	return new_fbgc_int_object(rand() % max + min );
}

new_fbgc_cfunction(fbgc_random,"random")
{
	srand((int) clock() );

	double min = 0;
	double max = 1.0;

	if(argc == 1){	
		max = convert_fbgc_object_to_double(arg[0]);
	}
	else if(argc == 2){
		min = convert_fbgc_object_to_double(arg[0]);
		max = convert_fbgc_object_to_double(arg[1]);
	}
	else if(argc > 2) return NULL;

	double range = (max - min); 
    double div = RAND_MAX / range;

	return new_fbgc_double_object( min + rand() /( div ) );
}

new_fbgc_cfunction(fbgc_rand,"rand")
{
	cprintf(111,"calling me!\n");
	srand((int) clock() );
	size_t row,col;
	if(argc == 1){	
		row = col = convert_fbgc_object_to_int(arg[0]);
	}
	else if(argc == 2){
		row = convert_fbgc_object_to_int(arg[0]);
		col = convert_fbgc_object_to_int(arg[1]);
	}
	else return NULL;

	struct fbgc_object * m = new_fbgc_matrix_object(DOUBLE,row,col,UNINITIALIZED_MATRIX);
	double * mc = content_fbgc_matrix_object(m);
	for(size_t i = 0; i<row; ++i){
		for(size_t j = 0; j<col; ++j){
			mc[i*col+j] = rand() /( (double)RAND_MAX );
		}
	}	
	return m;
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
		&fbgc_random_struct,
		&fbgc_randint_struct,
		&fbgc_rand_struct,		
		NULL
	}
};
