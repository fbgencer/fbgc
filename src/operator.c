#include "fbgc.h"
	

//#define INT 2 // int class
//#define DOUBLE 3 // double class
//#define COMPLEX 4 // complex class
//#define STRING 5 // string class
//#define MATRIX 6 // matrix class
//#define TUPLE 7 // tuple class



struct fbgc_object * (*fbgc_binary_op[7]) (struct fbgc_object *, struct fbgc_object *, fbgc_token ) =
{	
	binary_op_fbgc_logic_object,
	binary_op_fbgc_int_object,
	binary_op_fbgc_double_object,
	binary_op_fbgc_complex_object,
	binary_op_fbgc_str_object,
	binary_op_fbgc_matrix_object,
	binary_op_fbgc_tuple_object,

};


struct fbgc_object * safe_call_fbgc_binary_op(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main, fbgc_token op){
	switch(main){
		case LOGIC: return binary_op_fbgc_logic_object(a,b,op);
		case INT: return binary_op_fbgc_int_object(a,b,op);
		case DOUBLE: return binary_op_fbgc_double_object(a,b,op);
		case COMPLEX: return binary_op_fbgc_complex_object(a,b,op);
		case STRING: return binary_op_fbgc_str_object(a,b,op);
		case TUPLE: ;//return binary_op_fbgc_matrix_object(a,b,op);
		case MATRIX: return NULL;
	}
	return NULL;
}
