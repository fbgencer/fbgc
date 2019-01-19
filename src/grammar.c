#include "fbgc.h"

// input is stack 

	//Now seek the left of the operator, means the top of the grammar stack
	//Assume that input is 1+2*(3)
	// Then we have three list or stacks Main(M), Operator(O) and Grammar(G)
/*

	x = {1,2,3} << 'fbg'
	x = 1; x <<= 'fbg'; x: {'fbg',1}
	x = 1; x >>= 'fbg'; x: {1,'fbg'}

	x = 'fbg' >> x : {'fbg',1}
	x = 'fbg' << x : {'fbg',1}
	x = x << 'fbg' : {1,'fbg'}
	x = x >> 'fbg' : {1,'fbg'}

*/

/*	
	Syntax->> left_tok TOKEN right_tok
	left_tok means when token pushed into a stack(main or operator), it needs to see that left_tok 
	into the grammar stack, for example 1+3 is a valid entry meanwhile 1-*4 is not, 
	For 1+3 example, stacks look following
	M:1		'+'	 	1,	 	'3'		1,3									1,3,+
	O:		--->  	+, 		---> 	+		Now '+' looks its right --> 	
	G:atom		  	plus,			atom,								expression
	
	:>Usage
	grammer_tokens: uppercase like IF, ELIF 
	normal tokens : uppercase inside quotation like 'double', 'int'


	matrix: '[' balanced_list ']'
	balanced_list: unbalanced_list (expression | atom)
	unbalanced_list: (expression | atom) ','
	quick_list: (expression | atom) ':' 

	
	IF: 'if' para_comparison 
	ELIF: IF (statement) 'elif' para_comparison    
	ELSE: (IF | ELIF) (statement) 'else'
	WHILE: 'while' para_comparison
	UNTIL: 'until' para_comparison
 	END:  (IF | ELIF | ELSE | WHILE | UNTIL) (statement) END
	
	
	fun_def: WORD LP (balanced_word_list | empty) RP LBR statement RBR


	statement: para_comparison | para_expression | expression | assignment
	para_expression: '(' expression ')'
	para_comparison: '(' comparison_expr ')'

	
	assignment: assignment_op expression
	assignment_op: identifier ('=' | '+=' | '-=' | '*=' | '/=' | '<<=' | '>>=') 


	expression: arithmetic_expr | comparison_expr | unary_expr | (binary_op atom) | (unary_op atom)
	comparison_expr: ('&' | '|' | '==' | '>' | '<' | '>=' | '<=') (expression | atom)
	arithmetic_expr: ('+' | '-' | '*' | '/') (expression | atom)
	unary_expr: unary_op (expression | atom)
	unary_op: (binary_op | '(' )* ('+' | '-' | ++ | --) 
	

	:> if operator is & it becomes AND, if '+' it becomes plus etc.
	binary_op: (atom | expression) ('+' | '-' | '*' | '/' | '&' | '|' | '==')

	SYNTAX :> 
		return : stack incoming_obj 

	atom: INT|DOUBLE|STR|WORD 	
*/



uint8_t grammar_seek_left(struct fbgc_object * gm_head,struct fbgc_object * obj ){

	fbgc_token left = get_fbgc_object_type(top_fbgc_ll_object(gm_head));
	fbgc_token obj_type = get_fbgc_object_type(obj);
	cprintf(111,"Left :[%s], obj_type:[%s]\n",object_name_array[left],object_name_array[obj_type]);

	//two number cannot be in the state side by side, left is the last pushed in the array
	
	if(!is_fbgc_NUMBER(left) && is_fbgc_NUMBER(obj_type)){
		if(!is_fbgc_BINARY_OPERATOR(left) && !is_fbgc_UNARY_OPERATOR(left)) gm_head = push_front_fbgc_ll_object(gm_head,new_fbgc_object(NUMBER));
		else top_fbgc_ll_object(gm_head)->type = NUMBER;
	}
	else if( (is_fbgc_BINARY_OPERATOR(left) || left == LPARA || left == UNKNOWN) && is_fbgc_UNARY_OPERATOR(obj_type)){
		if(obj_type == PLUS) obj->type = UPLUS;
		else if(obj_type == MINUS) obj->type = UMINUS;	
		top_fbgc_ll_object(gm_head)->type = obj->type;

	}
	else if(is_fbgc_BINARY_OPERATOR(obj_type)){
		if(is_fbgc_NUMBER(left) || left == EXPRESSION || left == IDENTIFIER){
			top_fbgc_ll_object(gm_head)->type = obj_type;
		}
		else {
			cprintf(111,"\nThe left side of %s is not valid operand!\n",object_name_array[obj->type]);
			return 0;
		}
	}
	else if(obj_type == LPARA && left != LPARA){
		gm_head = push_front_fbgc_ll_object(gm_head,new_fbgc_object(LPARA));
	}
	else if(obj_type == RPARA){
		if(left == EXPRESSION || is_fbgc_NUMBER(left)) top_fbgc_ll_object(gm_head)->type = CLOSED_EXPRESSION;
		else {
			cprintf(111,"\nThe left side of %s is not valid operand!\n",object_name_array[obj->type]);
			return 0;
		}
	}
	return 1;
}


uint8_t grammar_seek_right(struct fbgc_object * gm_head,struct fbgc_object * obj){

	fbgc_token right = get_fbgc_object_type(top_fbgc_ll_object(gm_head));
	fbgc_token obj_type = get_fbgc_object_type(obj);
	cprintf(111,"Right :[%s], obj_type:[%s]\n",object_name_array[right],object_name_array[obj_type]);

	//two number cannot be in the state side by side, top is the last pushed in the array
	
	if((is_fbgc_BINARY_OPERATOR(obj_type) || is_fbgc_UNARY_OPERATOR(obj_type)) && (right == EXPRESSION || right == NUMBER || right == IDENTIFIER)){
			top_fbgc_ll_object(gm_head)->type = EXPRESSION;
	}
	/*else if( (is_fbgc_BINARY_OPERATOR(top) || top == LPARA || top == UNKNOWN) && is_fbgc_UNARY_OPERATOR(obj_type)){
		if(obj_type == PLUS) obj->type = UPLUS;
		else if(obj_type == MINUS) obj->type = UMINUS;	
		top_fbgc_ll_object(gm_head)->type = obj_type;

	}
	else if(is_fbgc_BINARY_OPERATOR(obj_type)){
		if(is_fbgc_NUMBER(top) || top == EXPRESSION || top == IDENTIFIER){
			top_fbgc_ll_object(gm_head)->type = obj_type;
		}
		else {
			cprintf(111,"\nThe left side of %s is not valid operand!\n",object_name_array[obj->type]);
		}
	}
	else if(obj_type == LPARA && top != LPARA){
		gm_head = push_front_fbgc_ll_object(gm_head,new_fbgc_object(LPARA));
	}*/
}
