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

struct fbgc_object * fbgc_binary_starstar(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok){ 
		switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object( pow(convert_fbgc_object_to_int(a),convert_fbgc_object_to_int(b)) );
		default:
			return NULL;
	}
}	
struct fbgc_object * fbgc_binary_slashslash(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok){

 	// a // b  = 1/(1/a+1/b) =  a*b/(a+b) 

	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_double_object( 1.0/(1.0/convert_fbgc_object_to_double(a) + 1.0/convert_fbgc_object_to_double(b)) );
		default:
			return NULL;
	}


}
struct fbgc_object * fbgc_binary_loeq(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) <= convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}

struct fbgc_object * fbgc_binary_greq(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) >= convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}
struct fbgc_object * fbgc_binary_eqeq(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 

	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) == convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}
struct fbgc_object * fbgc_binary_noteq(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) != convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}


struct fbgc_object * fbgc_binary_rshift(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) >> convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}

struct fbgc_object * fbgc_binary_lshift(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) << convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}


struct fbgc_object * fbgc_binary_caret(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 

	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object( pow(convert_fbgc_object_to_int(a),convert_fbgc_object_to_int(b)) );
		default:
			return NULL;
	}

}
struct fbgc_object * fbgc_binary_percent(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) % convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}


struct fbgc_object * fbgc_binary_lower(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
		switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) < convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}



struct fbgc_object * fbgc_binary_grater(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 

	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) > convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}


struct fbgc_object * fbgc_binary_pipe(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok ){ 
	
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) | convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}
}


struct fbgc_object * fbgc_binary_ampersand(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok){ 
	
	//cprintf(110,"[%s](%s,%s)->%s\n",__FUNCTION__,object_name_array[a->type],object_name_array[b->type],object_name_array[tok_ab]);
	switch(main_tok){
		case INT:
			//int has lower precedence for the other
			//both a and b must be int 
			return new_fbgc_int_object(convert_fbgc_object_to_int(a) & convert_fbgc_object_to_int(b) );
		default:
			return NULL;
	}

}


struct fbgc_object * fbgc_binary_plus(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok){

	//cprintf(110,"[%s](%s,%s)->%s\n",__FUNCTION__,object_name_array[a->type],object_name_array[b->type],object_name_array[tok_ab]);

	switch(main_tok){
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


struct fbgc_object * fbgc_binary_minus(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok){
	

	switch(main_tok){
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
		
		default:
			cprintf(100,"Error at binary minus\n");
		return NULL;	
	}
}

struct fbgc_object * fbgc_binary_star(struct fbgc_object * a, struct fbgc_object * b,fbgc_token main_tok){
	

	switch(main_tok){
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
		
		default:
			cprintf(100,"Error at binary multiply \n");
		return NULL;	
	}
}


struct fbgc_object * fbgc_binary_slash(struct fbgc_object * a, struct fbgc_object * b, fbgc_token main_tok){
	



	switch(main_tok){
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
		
		default:
			cprintf(100,"Error at binary divide\n");
		return NULL;	
	}
}



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


//Assignment

// a=b
void fbgc_assignment_assign(struct fbgc_object * a, struct fbgc_object *b , fbgc_token main_tok ){
	
	//assign_var_to_fbgc_ref_object(a,get_var_from_fbgc_ref_object(b));

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





struct fbgc_object * (*fbgc_binary_op[BINARY_FUNCTION_NO])(struct fbgc_object *, struct fbgc_object *, fbgc_token ) =
{
	fbgc_binary_starstar,
	fbgc_binary_slashslash,
	fbgc_binary_loeq,
	fbgc_binary_greq,
	fbgc_binary_eqeq,
	fbgc_binary_noteq,
	fbgc_binary_rshift,
	fbgc_binary_lshift,
	fbgc_binary_caret,
	fbgc_binary_percent,
	fbgc_binary_lower,
	fbgc_binary_grater,
	fbgc_binary_pipe,
	fbgc_binary_ampersand,
	fbgc_binary_slash,
	fbgc_binary_star,
	fbgc_binary_minus,
	fbgc_binary_plus
};

