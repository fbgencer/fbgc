#include "fbgc.h"
	

//#define INT 2 // int class
//#define DOUBLE 3 // double class
//#define COMPLEX 4 // complex class
//#define STRING 5 // string class
//#define MATRIX 6 // matrix class
//#define TUPLE 7 // tuple class



struct fbgc_object * (*fbgc_operator[7]) (struct fbgc_object *, struct fbgc_object *, fbgc_token) =
{	
	operator_fbgc_logic_object,
	operator_fbgc_int_object,
	operator_fbgc_double_object,
	operator_fbgc_complex_object,
	operator_fbgc_str_object,
	operator_fbgc_matrix_object,
	operator_fbgc_tuple_object,
};


struct fbgc_object * safe_call_fbgc_operator(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main, fbgc_token op){
	switch(main){
		case LOGIC: return operator_fbgc_logic_object(a,b,op);
		case INT: return operator_fbgc_int_object(a,b,op);
		case DOUBLE: return operator_fbgc_double_object(a,b,op);
		case COMPLEX: return operator_fbgc_complex_object(a,b,op);
		case STRING: return operator_fbgc_str_object(a,b,op);
		case TUPLE: return operator_fbgc_matrix_object(a,b,op);
		case MATRIX: return operator_fbgc_matrix_object(a,b,op);
	}
	return NULL;
}
