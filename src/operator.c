#include "fbgc.h"
	

struct fbgc_object * is_fbgc_object_true(struct fbgc_object * a){

	fbgc_token tok_ab = get_fbgc_object_type(a);

	switch(tok_ab){
		case TRUE: 
			return new_fbgc_object(TRUE);
		case FALSE:
			return new_fbgc_object(FALSE);
		default:
			cprintf(100,"Error at true comparison \n");
		return NULL;	
	}	
}


//#define INT 2 // int class
//#define DOUBLE 3 // double class
//#define COMPLEX 4 // complex class
//#define STRING 5 // string class
//#define TUPLE 6 // tuple class
//#define MATRIX 7 // matrix class


struct fbgc_object * (*fbgc_binary_op[6]) (struct fbgc_object *, struct fbgc_object *, fbgc_token ) =
{	
	binary_op_fbgc_int_object,
	binary_op_fbgc_double_object,
	binary_op_fbgc_complex_object,
	binary_op_fbgc_str_object,
	NULL,
	NULL,

};


struct fbgc_object * safe_call_fbgc_binary_op(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main, fbgc_token op){
	switch(main){
		case INT: return binary_op_fbgc_int_object(a,b,op);
		case DOUBLE: return binary_op_fbgc_double_object(a,b,op);
		case COMPLEX: return binary_op_fbgc_complex_object(a,b,op);
		case STRING: return binary_op_fbgc_str_object(a,b,op);
		case TUPLE: ;//return binary_op_fbgc_matrix_object(a,b,op);
		case MATRIX: return NULL;
	}
	return NULL;
}
