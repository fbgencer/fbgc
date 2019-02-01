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
	string + double = ?


*/

struct fbgc_object * fbgc_binary_add(struct fbgc_object * a, struct fbgc_object * b){


	if(a == NULL || b == NULL) return NULL;

	fbgc_token tok_ab = 
	(get_fbgc_object_type(a) > get_fbgc_object_type(b)) ? 
	get_fbgc_object_type(a) : 
	get_fbgc_object_type(b) ;

	switch(tok_ab){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return add_fbgc_int_object(a,b);
		case DOUBLE:
			//before sending add function convert a and b as doubles
		 	//no need to check double, function that we call is going to check
			return add_fbgc_double_object(a,b);
		case COMPLEX: return NULL;
		case STRING: return NULL;
		default:
			cprintf(100,"Error at binary add\n");
		return NULL;	
	}
}


struct fbgc_object * fbgc_binary_minus(struct fbgc_object * a, struct fbgc_object * b){
	
	if(a == NULL || b == NULL) return NULL;

	fbgc_token tok_ab = 
	(get_fbgc_object_type(a) > get_fbgc_object_type(b)) ? 
	get_fbgc_object_type(a) : 
	get_fbgc_object_type(b) ;

	switch(tok_ab){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return subtract_fbgc_int_object(a,b);
		case DOUBLE:
			//before sending add function convert a and b as doubles
		 	//no need to check double, function that we call is going to check
			return subtract_fbgc_double_object(a,b);
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case VAR: return NULL;
		default:
			cprintf(100,"Error at binary minus\n");
		return NULL;	
	}
}

struct fbgc_object * fbgc_binary_star(struct fbgc_object * a, struct fbgc_object * b){
	
	if(a == NULL || b == NULL) return NULL;

	fbgc_token tok_ab = 
	(get_fbgc_object_type(a) > get_fbgc_object_type(b)) ? 
	get_fbgc_object_type(a) : 
	get_fbgc_object_type(b) ;

	switch(tok_ab){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return multiply_fbgc_int_object(a,b);
		case DOUBLE:
			//before sending add function convert a and b as doubles
		 	//no need to check double, function that we call is going to check
			return multiply_fbgc_double_object(a,b);
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case VAR: return NULL;
		default:
			cprintf(100,"Error at binary multiply \n");
		return NULL;	
	}
}


struct fbgc_object * fbgc_binary_slash(struct fbgc_object * a, struct fbgc_object * b){
	
	
	if(a == NULL || b == NULL) return NULL;

	fbgc_token tok_ab = 
	(get_fbgc_object_type(a) > get_fbgc_object_type(b)) ? 
	get_fbgc_object_type(a) : 
	get_fbgc_object_type(b) ;

	switch(tok_ab){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return divide_fbgc_int_object(a,b);
		case DOUBLE:
			//before sending add function convert a and b as doubles
		 	//no need to check double, function that we call is going to check
			return divide_fbgc_double_object(a,b);
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case VAR: return NULL;
		default:
			cprintf(100,"Error at binary divide\n");
		return NULL;	
	}
}



struct fbgc_object * (*fbgc_binary_op[])(struct fbgc_object *, struct fbgc_object *) =
{
	fbgc_binary_add,
	fbgc_binary_minus,
	fbgc_binary_star,
	fbgc_binary_slash
};



//Assignment

// a=b
void fbgc_assignment_assign(struct fbgc_object * a, struct fbgc_object * b ){
	cprintf(001,"Trying to assign!\n");
	if(get_fbgc_object_type(b) != REFERENCE){
		a = assign_var_to_fbgc_ref_object(a,b);
	}
	else{
		cprintf(001,"Working on x = y\n");
		a = assign_var_to_fbgc_ref_object(a,get_var_from_fbgc_ref_object(b));
	}

}


void (*fbgc_assignment_op[])(struct fbgc_object *, struct fbgc_object *) =
{
	fbgc_assignment_assign,
};