

#include "../src/fbgc.h"
#include "fbgc_math.h"
#include <tgmath.h>
#include <complex.h>
//#include <gsl/gsl_matrix.h>
//#include <gsl/gsl_blas.h>
//#include <gsl/gsl_complex_math.h>

//#include <cblas.h>

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

#ifdef __C99_COMPLEXH__
	


#endif


struct raw_complex c_exp(struct raw_complex z){
	//complex exponantiel

	//<! \todo We need to write a new complex library and also provide c99 complex math interface so we can call those functions with just one casting
	/*
	struct raw_complex mm = z;
	double complex * xx = (double complex*)&mm;

	//((double *)&xx)[0] = z.real;
	//((double *)&xx)[1] = z.imag;
	*xx = cexp(*xx); // Euler's formula
	
	printf("res %g%+gj\n", creal(*xx), cimag(*xx));*/

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

struct fbgc_object * one_arg_math(struct fbgc_object ** arg,int argc, double (*fun) (double),struct raw_complex(*zfun) (struct raw_complex)){

	struct fbgc_object * obj;

	if(parse_tuple_content(arg,argc,"i|d|j|t|m",&obj) == -1)
		return NULL;

	switch(obj->type){
		case INT:
		case DOUBLE:
		{
			double dbarg = convert_fbgc_object_to_double(obj);
			return new_fbgc_double_object((*fun)(dbarg));
		}
		case COMPLEX:
		{
			struct raw_complex z = convert_fbgc_object_to_complex(obj);
			z = (*zfun)(z);
			return new_fbgc_complex_object(z.real,z.imag);
		}
		case TUPLE:
		{
			size_t sz = size_fbgc_tuple_object(obj);
			struct fbgc_object * res_tp = new_fbgc_tuple_object( sz );
			size_fbgc_tuple_object(res_tp) = sz;
			while(sz--){
				content_fbgc_tuple_object(res_tp)[sz] = 
					one_arg_math(content_fbgc_tuple_object(obj)+sz,1,fun,zfun);
			}
			return res_tp;
		}
		case MATRIX:
		{
			struct fbgc_matrix_object * m = (struct fbgc_matrix_object *) obj;
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


#define new_fbgc_one_arg_math(func_ptr,complex_func_ptr)\
static struct fbgc_object * fbgc_math_##func_ptr(struct fbgc_object ** arg, int argc){\
return one_arg_math(arg,argc,func_ptr,complex_func_ptr);}\


new_fbgc_one_arg_math(cos,c_cos);
new_fbgc_one_arg_math(sin,NULL);
new_fbgc_one_arg_math(tan,NULL);
new_fbgc_one_arg_math(acos,NULL);
new_fbgc_one_arg_math(asin,NULL);
new_fbgc_one_arg_math(atan,NULL);

new_fbgc_one_arg_math(cosh,NULL);
new_fbgc_one_arg_math(sinh,NULL);
new_fbgc_one_arg_math(tanh,NULL);
new_fbgc_one_arg_math(acosh,NULL);
new_fbgc_one_arg_math(asinh,NULL);
new_fbgc_one_arg_math(atanh,NULL);


new_fbgc_one_arg_math(exp,c_exp);
new_fbgc_one_arg_math(sqrt,NULL);


// new_fbgc_cfunction(fbgc_gsl,"gsl")
// {

//   	/*	int i=0;
//   	double A[6] = {1.0,2.0,
//   					 1.0,-3.0,
//   					 4.0,-1.0};//3x2  
//   	double B[6] = { 1.0,2.0,1.0,
//   					 -3.0,4.0,-1.0};//2x3
//   	double C[9] = {0};//3x3
//  	size_t m = 3;
//  	size_t k = 2;
//  	size_t n = 3; 
//   	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,m,n,k,1,A, k, B, n,0,C,n);

//   	for(i=0; i<9; i++)
//     	printf("%lf ", C[i]);
//   	printf("\n");

//   	return NULL;*/


// 	struct fbgc_matrix_object * m1 = (struct fbgc_matrix_object *) arg[0];
// 	struct fbgc_matrix_object * m2 = (struct fbgc_matrix_object *) arg[1];
// 	if(m1->column != m2->row) return NULL;
// 	fbgc_token sub_type = MAX(m1->sub_type,m2->sub_type);
// 	struct fbgc_matrix_object * res = (struct fbgc_matrix_object *) new_fbgc_matrix_object(sub_type, m1->row, m2->column, UNINITIALIZED_MATRIX);
		
//  	//cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,m, n, k, alpha, A, k, B, n, beta, C, n); 
//   	//A : mxk, B : kxn, C : mxn
//   	//alpha : used to scale the product of matrices A and B, beta : used to scale matrix C
//  	size_t m = m1->row;
//  	size_t k = m1->column;
//  	size_t n = m2->column;

// 	double * A = content_fbgc_matrix_object(m1);
// 	double * B = content_fbgc_matrix_object(m2);
// 	double * C = content_fbgc_matrix_object(res);

// 	//both of them are double no need to make extra work
// 	if(sub_type == DOUBLE){
//  		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,m,n,k,1 ,A,k,B,n,0,C,n);
// 	}
// 	//one of the matrices could be double so we need check and convert to complex before using zgemm
//  	else if(sub_type == COMPLEX){
//  		double alpha[] = {1,0};
//  		double beta [] = {0,0};
//  		cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,m,n,k,alpha,A,k,B,n,beta,C,n);
//  	}

//  	return (struct fbgc_object *) res;




// 	if(argc == 2 && arg[0]->type == MATRIX && arg[1]->type == MATRIX){


// 		struct fbgc_matrix_object * m1 = (struct fbgc_matrix_object *) arg[0];
// 		struct fbgc_matrix_object * m2 = (struct fbgc_matrix_object *) arg[1];

// 		if(m1->column != m2->row) return NULL;
		

// 		fbgc_token sub_type = MAX(m1->sub_type,m2->sub_type);


// 		struct fbgc_matrix_object * res = (struct fbgc_matrix_object *) new_fbgc_matrix_object(sub_type, m1->row, m2->column, UNINITIALIZED_MATRIX);
		
// 		gsl_matrix v1 = { .size1=m1->row, .size2=m1->column, .data = content_fbgc_matrix_object(m1), .tda = m1->column};
// 		gsl_matrix v2 = { .size1=m2->row, .size2=m2->column, .data = content_fbgc_matrix_object(m2), .tda = m2->column};
// 		gsl_matrix v3 = { .size1=m1->row, .size2=m2->column, .data = content_fbgc_matrix_object(res), .tda = m2->column};

// 		if(sub_type == COMPLEX){

  			
//   			//gsl_matrix_complex_view vm1 = gsl_matrix_complex_view_array(content_fbgc_matrix_object(m1), m1->row, m1->column);
//  			//gsl_matrix_complex_view vm2 = gsl_matrix_complex_view_array(content_fbgc_matrix_object(m2), m2->row, m2->column);
//   			//gsl_matrix_complex_view vres = gsl_matrix_complex_view_array(content_fbgc_matrix_object(res), m1->row, m2->column);
// 			gsl_blas_zgemm (CblasNoTrans, CblasNoTrans, GSL_COMPLEX_ONE, &v1, &v2, GSL_COMPLEX_ZERO, &v3);
// 		}
// 		else if(sub_type == DOUBLE){
// 		//	gsl_matrix_complex v1 = { .size1=m1->row, .size2=m1->column, .data = content_fbgc_matrix_object(m1), .tda = m1->column};
//   		//	gsl_matrix_complex v2 = { .size1=m2->row, .size2=m2->column, .data = content_fbgc_matrix_object(m2), .tda = m2->column};
//   		//	gsl_matrix_complex v3 = { .size1 = m1->row, .size2=m2->column, .data = content_fbgc_matrix_object(res), .tda = m2->column};
//   			gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, &v1, &v2, 0, &v3);
// 		}

// 		return (struct fbgc_object *) res;

// 	}
		
// /*
// 	//x = [1+2j 3+4j; 2.2+3j 5.5-8.8j];
// 	double x[] = {  1,2,        3,4,
// 						2.2,3,    5.5,-8.8};

// 	//y = [1+2.3j 30+4.2j; 0.22+1j 5-8j];						
// 	double y[] = {  1,2.3,    30,4.2, 
// 					0.22,1,   5,-8};

// 	//2x2 matrices 8 doubles including real+imag
// 	double z[8] = {0};

//   	gsl_matrix_complex_view X = gsl_matrix_complex_view_array(x, 2, 2);
//  	gsl_matrix_complex_view Y = gsl_matrix_complex_view_array(y, 2, 2);
//   	gsl_matrix_complex_view Z = gsl_matrix_complex_view_array(z, 2, 2);

// 	gsl_blas_zgemm (CblasNoTrans, CblasNoTrans,
// 	              GSL_COMPLEX_ONE, &X.matrix, &Y.matrix,
// 	              GSL_COMPLEX_ZERO, &Z.matrix);

// 	for(int i = 0; i<8; i+=2){
// 		gsl_complex cm = {z[i],z[i+1]};
// 		printf ("[%d,%d] = %g%+gj\n",i,i+1,cm.dat[0], cm.dat[1]);
// 	}


// */

// 	return NULL;
// }

// new_fbgc_cfunction(fbgc_randint,"randint")
// {

// 	srand((int) clock() );

// 	int min = 0;
// 	int max = 0;

// 	if(argc == 1){	
// 		max = convert_fbgc_object_to_int(arg[0]);
// 	}
// 	else if(argc == 2){
// 		min = convert_fbgc_object_to_int(arg[0]);
// 		max = convert_fbgc_object_to_int(arg[1]);
// 	}
// 	else{
// 		cprintf(100,"randint requires one or two arg\n"); 
// 		return NULL;
// 	}

// 	return new_fbgc_int_object(rand() % max + min );
// }

// new_fbgc_cfunction(fbgc_random,"random")
// {
// 	srand((int) clock() );

// 	double min = 0;
// 	double max = 1.0;

// 	if(argc == 1){	
// 		max = convert_fbgc_object_to_double(arg[0]);
// 	}
// 	else if(argc == 2){
// 		min = convert_fbgc_object_to_double(arg[0]);
// 		max = convert_fbgc_object_to_double(arg[1]);
// 	}
// 	else if(argc > 2) return NULL;

// 	double range = (max - min); 
//     double div = RAND_MAX / range;

// 	return new_fbgc_double_object( min + rand() /( div ) );
// }

// new_fbgc_cfunction(fbgc_rand,"rand")
// {
// 	srand((int) clock() );
// 	size_t row,col;

// 	if(argc == 1){	
// 		row = col = convert_fbgc_object_to_int(arg[0]);
// 	}
// 	else if(argc == 2){
// 		row = convert_fbgc_object_to_int(arg[0]);
// 		col = convert_fbgc_object_to_int(arg[1]);
// 	}
// 	else return NULL;

// 	struct fbgc_object * m = new_fbgc_matrix_object(DOUBLE,row,col,UNINITIALIZED_MATRIX);
// 	double * mc = content_fbgc_matrix_object(m);
// 	size_t rc = row*col;
// 	while(rc)
// 		mc[--rc] = rand()/( (double)RAND_MAX );
		
	
// 	return m;
// }

// new_fbgc_cfunction(fbgc_randj,"randj")
// {
// 	srand((int) clock() );
// 	size_t row,col;
// 	if(argc == 1){	
// 		row = col = convert_fbgc_object_to_int(arg[0]);
// 	}
// 	else if(argc == 2){
// 		row = convert_fbgc_object_to_int(arg[0]);
// 		col = convert_fbgc_object_to_int(arg[1]);
// 	}
// 	else return NULL;

// 	struct fbgc_object * m = new_fbgc_matrix_object(COMPLEX,row,col,UNINITIALIZED_MATRIX);
// 	double * mc = content_fbgc_matrix_object(m);
// 	size_t rc = 2*row*col;
// 	while(rc)
// 		mc[--rc] = rand()/( (double)RAND_MAX );

// 	return m;
// }


// //Work on this, is it possible to cast ?
// const struct fbgc_cmodule fbgc_math_module = 
// {
// 	//.name = "math",
// 	.initializer = &fbgc_math_initializer_struct,
// 	.functions = (const struct fbgc_cfunction*[])
// 	{	
// 		&fbgc_sin_struct,
// 		&fbgc_cos_struct,
// 		&fbgc_tan_struct,
// 		&fbgc_exp_struct,
// 		&fbgc_sqrt_struct,
// 		&fbgc_gsl_struct,
// 		&fbgc_random_struct,
// 		&fbgc_randint_struct,
// 		&fbgc_rand_struct,
// 		&fbgc_randj_struct,		
// 		NULL
// 	}
// };


static
struct fbgc_object * fbgc_math_init(struct fbgc_object ** arg, int argc){
	//cprintf(111,"Called math initializer!\n");
	add_variable_in_field_object(arg[0],"pi",new_fbgc_double_object(FBGC_MATH_PI));
	add_variable_in_field_object(arg[0],"e",new_fbgc_double_object(FBGC_MATH_E));

	return NULL;
}

const struct fbgc_cmodule fbgc_math_module = 
{
	.initializer = {.name = "math", .function = &fbgc_math_init},
	.functions = {
		{.name = "sin", .function = &fbgc_math_sin},
		{.name = "cos", .function = &fbgc_math_cos},
		{.name = "tan", .function = &fbgc_math_tan},
		{.name = "exp", .function = &fbgc_math_exp},
		{NULL,NULL},
	}
};