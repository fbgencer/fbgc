#include "fbgc.h"

// input is stack 

	//Now seek the left of the operator, means the top of the grammar stack
	//Assume that input is 1+2*(3)
	// Then we have three list or stacks Main(M), Operator(O) and Grammar(G)


void seek_left(struct fbgc_object ** grammar_stack,fbgc_token op ){

	fbgc_token top = (*grammar_stack)->next->type;

	switch(op){
		case PLUS: 
		case MINUS:
		case MULT:
		case DIV:
		case POWER1:
			if(top == IDENTIFIER || top == EXPRESSION || top == NUMBER){
				(*grammar_stack)->next->type = UNKNOWN;
			}
			break;
		default: break;
	}

}