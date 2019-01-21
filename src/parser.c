#include "fbgc.h"



uint8_t operator_precedence(fbgc_token T){
	
	switch(T){
		case INC: case DECR: case UMINUS: case UPLUS: return 10;
		case SLASH: case STAR: case MOD: case CARET: case STARSTAR: case SLASHSLASH: return 9;
		case PLUS: case MINUS: case NOT_OP: return 8;
		case LSHIFT: case RSHIFT: return 7;
		case LOWER: case GREATER: case LO_EQ: case  GR_EQ: return 6;
		case  EQ_EQ: case NOT_EQ: case IS_EQ: return 5;
		case AND_OP: return 4;
		case OR_OP: return 3;
		case ASSIGN: case PLUS_ASSIGN: case MINUS_ASSIGN: case STAR_ASSIGN: case SLASH_ASSIGN: return 2;
		case COMMA: case LPARA: case RPARA: return 1;
		default: return 0;	
	}
}


/*

*/

struct
fbgc_object * op_stack_push(struct fbgc_object * head,struct fbgc_object * obj){
	//[head]-> [op1]-> NULL;
	//[head]-> [op2]->[op1]->NULL;
	//[head]-> [op3]->[op2]->[op1]->NULL;	

	//printf("\033[1;32m[Op Stack : Pushed:%s]\033[0m\n",object_name_array[obj->type]); 
	obj->next = head->next;
	head->next = obj;	
	
	
	return (struct fbgc_object*) head;
}
#define op_stack_top(self)(self->next)
#define op_stack_empty(self)(self->next == NULL)
struct
fbgc_object * op_stack_pop(struct fbgc_object * head){
	//[head]-> [op1]-> NULL;
	//[head]-> [op2]->[op1]->NULL;
	//[head]-> [op3]->[op2]->[op1]	
	//Assume we want to pop op3, just derefence it do not free!

	//printf("\033[1;32m[Op Stack : Popped:%s]\033[0m\n",object_name_array[head->next->type]); 
	head->next = head->next->next;
	return (struct fbgc_object*) head;
}

void op_stack_print(struct fbgc_object *head){
	struct fbgc_object * iter = head->next;
	cprintf(010,"[O]->");
	
	while(iter != NULL){
		cprintf(010,"[%s]->",object_name_array[iter->type]);
		iter = iter->next;
	}
	cprintf(100,"NULL\n");

}


struct 
fbgc_object * parser(struct fbgc_object * head_obj){
	
	cprintf(111,"--------------[parser_begin]-------------\n");

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll(head_obj);
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = head; //note that iter_prev->next is iter, always this is the case!
	struct fbgc_object * op_stack_head = new_fbgc_object(UNKNOWN);//make just 0 we are using for just head
	struct fbgc_object * gm_stack_head = new_fbgc_ll_object();
	gm_stack_head = push_back_fbgc_ll_object(gm_stack_head,new_fbgc_object(LINKED_LIST));
	uint8_t gm_error = 1;

	for(int i = 0; i<30 && gm_error != 0 && iter != head->tail ; i++){

		cprintf(010,"----------------------[%d] = {%s}-----------------------\n",i,object_name_array[iter->type]);
		if(is_fbgc_ATOM(iter->type)){
			//this is number do not touch
			iter_prev = iter;
			//grammar_push(&gm_stack_head,iter);
			grammar_seek_left(gm_stack_head,iter);
		}
		else if(iter->type == WORD){
			//this is word do not touch
			iter_prev = iter;
			//grammar_push(&gm_stack_head,iter);		
		}
		else if(is_fbgc_OPERATOR(iter->type)){
			
			//take the op object from main list and connect previous one to the next one 
			//[H]->[2]->[+]->[3] => [H]->[2]->[3], now iter shows the operator iter->next is [3] but we will change that too 
			//     p^	i^					
			iter_prev->next = iter->next;
			// if(iter_prev == NULL) iter_prev = iter;
			// else iter_prev->next = iter->next;

			if( !op_stack_empty(op_stack_head) && operator_precedence(op_stack_top(op_stack_head)->type) >= operator_precedence(iter->type)){
				do{

					gm_error = grammar_seek_right(gm_stack_head,op_stack_top(op_stack_head));
					//Insert top op to the list  
					iter_prev->next = op_stack_top(op_stack_head);
					//Pop top from stack
					op_stack_head = op_stack_pop(op_stack_head);
					//connect list again
					iter_prev->next->next = iter->next; 
					//make the iter_prev proper
					iter_prev = iter_prev->next;	

				}while( !op_stack_empty(op_stack_head) && operator_precedence(op_stack_top(op_stack_head)->type) >= operator_precedence(iter->type));
			}
			
			gm_error = grammar_seek_left(gm_stack_head,iter);
			op_stack_head = op_stack_push(op_stack_head,iter);
			//print_fbgc_object(iter);
		}
		else if(iter->type == LPARA){
			grammar_seek_left(gm_stack_head,iter);
			iter_prev->next = iter->next;
			op_stack_head = op_stack_push(op_stack_head,iter);
			

		}
		else if(iter->type == RPARA){

			
			//if(!gm_error) goto END_OF_THE_PARSER;

			iter_prev->next = iter->next;
			while( op_stack_top(op_stack_head) != NULL && op_stack_top(op_stack_head)->type != LPARA){
				//Insert top op to the list  
				gm_error =grammar_seek_right(gm_stack_head,op_stack_top(op_stack_head));
				iter_prev->next = op_stack_top(op_stack_head);
				//Pop top from stack
				op_stack_head = op_stack_pop(op_stack_head);
				//connect list again
				iter_prev->next->next = iter->next; 
				//make the iter_prev proper
				iter_prev = iter_prev->next;
			}
			//op_stack_head = op_stack_push(op_stack_head,iter);
				
			gm_error = grammar_seek_left(gm_stack_head,iter);

			if(op_stack_top(op_stack_head) != NULL && op_stack_top(op_stack_head)->type == LPARA){
				//cprintf(111,"Hit the left para! content:%d i:%d\n",head_int->content,i);
				//now we have free paranthesis object by hand and also change the size of the main list
				head->size-=2;
				struct fbgc_object * dummy_for_lpara = op_stack_top(op_stack_head);
				op_stack_head = op_stack_pop(op_stack_head);
				free_fbgc_object(dummy_for_lpara);
				free_fbgc_object(iter);
			}
			else {
				goto END_OF_THE_PARSER;

			}
			
		}


		iter = iter_prev->next;
		print_fbgc_ll_object(head_obj,"M");
		op_stack_print(op_stack_head);
		print_fbgc_ll_object(gm_stack_head,"GM");

	}
	
	if(!op_stack_empty(op_stack_head) && gm_error != 0){
		cprintf(011,"Op stack is not empty!\n");
		//now iter shows the tail of the main list 
		while(!op_stack_empty(op_stack_head)){
			gm_error = grammar_seek_right(gm_stack_head,op_stack_top(op_stack_head));
			iter_prev->next = op_stack_top(op_stack_head);
			op_stack_head = op_stack_pop(op_stack_head);
			iter_prev = iter_prev->next;
		}
		iter_prev->next = iter;	

		print_fbgc_ll_object(head_obj,"M");
		op_stack_print(op_stack_head);
		print_fbgc_ll_object(gm_stack_head,"GM");		
	}

	END_OF_THE_PARSER:

	free_fbgc_object(op_stack_head);
	free_fbgc_ll_object(gm_stack_head);
	
	cprintf(111,"--------------[parser_end]-------------\n");
	return (struct fbgc_object*) head;
}
