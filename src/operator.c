#include "fbgc.h"
	
/*
	int + int  = int
	double + int = double
	int + double = double
	double + double = double

	int + complex = complex
	double + complex = complex
	complex + int = complex
	complex + double = complex
		
	//concatenation
	string + string = string
	string + int  = string
	string + double = string
*/

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
	NULL,
	binary_op_fbgc_str_object,
	NULL,
	NULL,

};
