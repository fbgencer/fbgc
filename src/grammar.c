#include "fbgc.h"

// input is stack 

	//Now seek the left of the operator, means the top of the grammar stack
	//Assume that input is 1+2*(3)
	// Then we have three list or stacks Main(M), Operator(O) and Grammar(G)


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

	fbgc_token top = (*grammar_stack)->next->type;

	switch(op){
		case PLUS: 
		case MINUS:
		case MULT:
		case DIV:
		case RPARA:
			//assert(top != IDENTIFIER && top != EXPRESSION && top != NUMBER);

			if(top == IDENTIFIER || top == EXPRESSION || top == NUMBER){
				cprintf(111,"Valid!\n");
				(*grammar_stack)->next->type = VALID;
				return 1;
			}
			
			cprintf(111,"\nThe left side of the operator is not valid operand!\n");
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
