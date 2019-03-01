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

struct fbgc_object * fbgc_binary_plus(struct fbgc_object * a, struct fbgc_object * b){

	if(a == NULL || b == NULL) return NULL;

	fbgc_token tok_ab = 
	(get_fbgc_object_type(a) > get_fbgc_object_type(b)) ? 
	get_fbgc_object_type(a) : 
	get_fbgc_object_type(b) ;

	//cprintf(110,"[%s](%s,%s)->%s\n",__FUNCTION__,object_name_array[a->type],object_name_array[b->type],object_name_array[tok_ab]);

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
		case TUPLE: return add_fbgc_tuple_object(a,b);
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






//Assignment

// a=b
void fbgc_assignment_assign(struct fbgc_object * a, struct fbgc_object * b ){
	
	assign_var_to_fbgc_ref_object(a,get_var_from_fbgc_ref_object(b));

	/*if(get_fbgc_object_type(a) == TUPLE && get_fbgc_object_type(b) == TUPLE){
		if(cast_fbgc_object_as_tuple(a)->size == cast_fbgc_object_as_tuple(b)->size){
			for(int i = 0; i<cast_fbgc_object_as_tuple(a)->size; i++){
				fbgc_assignment_assign(table,cast_fbgc_object_as_tuple(a)->contents[i],cast_fbgc_object_as_tuple(b)->contents[i]);
			}
			//cast_fbgc_object_as_tuple(a)->size = 0;			

		}
	}
	else if(get_fbgc_object_type(b) != REFERENCE){
		//cprintf(001,"Working on x = constant\n");
		a = assign_var_to_fbgc_ref_object(table,a,b);
	}
	else{
		//cprintf(001,"Working on x = y\n");
		//struct fbgc_object 
		a = assign_var_to_fbgc_ref_object(table,a,get_var_from_fbgc_ref_object(b));
	}*/

}



struct fbgc_object * (*fbgc_binary_op[4])(struct fbgc_object *, struct fbgc_object *) =
{
	fbgc_binary_plus,
	fbgc_binary_minus,
	fbgc_binary_star,
	fbgc_binary_slash
};
void (*fbgc_assignment_op[1])(struct fbgc_object *, struct fbgc_object *) =
{
	fbgc_assignment_assign,
};


struct fbgc_object * call_get_item_with_index(struct fbgc_object * var,struct fbgc_object *index){
	cprintf(111,"index:%s,vaR:%s\n",object_type_as_str(index),object_type_as_str(var));

	if(get_fbgc_object_type(var) != TUPLE && get_fbgc_object_type(var) != MATRIX) return NULL;
	if(get_fbgc_object_type(index) != INT ) return NULL;
	
	switch(get_fbgc_object_type(var)){
		case TUPLE:
			return get_object_in_fbgc_tuple_object(var,cast_fbgc_object_as_int(index)->content);
		default:
			return NULL;
	}


}