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
	unary_op: (binary_op | '(' )* ('+' | '-') 
	

	:> if operator is & it becomes AND, if '+' it becomes plus etc.
	binary_op: (atom | expression) ('+' | '-' | '*' | '/' | '&' | '|' | '==')

	atom: INT|DOUBLE|STR|WORD 

	
*/




void grammar_push(struct fbgc_object ** gm_head,fbgc_token type){
	//grammar head is already created

	if((*gm_head)->next == NULL || (*gm_head)->next->type != VALID){
		struct fbgc_object *temp = new_fbgc_object(type);
		temp->next = (*gm_head)->next;
		(*gm_head)->next = temp;
		cprintf(111,"Grammar pushed :%s\n",object_name_array[type]);
	}
	
	if((*gm_head)->next->type == VALID){
		(*gm_head)->next->type = type;
		cprintf(111,"Grammar just changed to :%s\n",object_name_array[type]);
	}

}


uint8_t grammar_seek_left(struct fbgc_object ** grammar_stack,fbgc_token op ){

	return 1;
	fbgc_token top = (*grammar_stack)->next->type;

	switch(op){
		case PLUS: 
		case MINUS:
		case MULT:
		case DIV:
		case RPARA:

			if(top == IDENTIFIER || top == EXPRESSION || top == NUMBER){
				cprintf(111,"Valid!\n");
				(*grammar_stack)->next->type = VALID;
				return 1;
			}
			
			cprintf(111,"\nThe left side of %s is not valid operand!\n",object_name_array[op]);
			return 0;
			
		case ASSIGN:
			if(top == IDENTIFIER){
				(*grammar_stack)->next->type = VALID;
				return 1;
			}
			cprintf(111,"\nThe left side of assignment operator must be identifier!\n");
			return 0;			
		default: return 0;
	}

}


uint8_t grammar_seek_right(struct fbgc_object ** grammar_stack,fbgc_token op ){
	return 1;
	fbgc_token top = (*grammar_stack)->next->type;
	//cprintf(001,"Coming! [%s]\n",object_name_array[op]);

	switch(op){
		case PLUS: 
		case MINUS:
		case MULT:
		case DIV:
			if(top == IDENTIFIER || top == EXPRESSION || top == NUMBER){
				cprintf(111,"Correct type ! now operator can be pushed\n");
				(*grammar_stack)->next->type = EXPRESSION;
				return 1;
			}else {
				cprintf(111,"\nThe left side of the operator is not valid operand!\n");
				return 0;
			}
			break;
		case ASSIGN:
			if(top == IDENTIFIER){
				(*grammar_stack)->next->type = ASSIGNMENT;
				return 1;
			}else {
				cprintf(111,"\nThe left side of assignment operator must be identifier!\n");
				return 0;
			}
			break;				
		default: break;
	}
}

void grammar_stack_print(struct fbgc_object * grammar_stack){
	struct fbgc_object * iter = grammar_stack->next;
	
	cprintf(001,"[G]->");

	while(iter != NULL){
		cprintf(001,"[%s]->",object_name_array[iter->type]);
		iter = iter->next;
	}
	cprintf(100,"NULL\n");

}


void grammar_free(struct fbgc_object * grammar_stack){
	struct fbgc_object * iter = grammar_stack->next, *temp = iter;
	
	while(iter != NULL){
		temp = iter->next;
		free_fbgc_object(iter);	
		iter = temp;	
	}
	free_fbgc_object(grammar_stack);

}
