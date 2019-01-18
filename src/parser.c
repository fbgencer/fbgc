#include "fbgc.h"



uint8_t operator_precedence(fbgc_token T){
	
	/*
	if(T == INC || T == DECR || T == UMINUS || T == UPLUS  ) return 10;
	else if(T == DIV  || T == MULT || T == MOD || T==POWER1 || T == POWER2 || T==DIVDIV){return 9;}
	else if(T == PLUS || T == MINUS || T== NOT_OP ) return 8;
	else if(T == LSHIFT || T == RSHIFT ) return 7;
	else if(T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ) return 6;
	else if(T == EQ_EQ || T == NOT_EQ   || T == IS_EQ) return 5; //ise degisebilr dene 
	else if(T == AND_OP ) return 4;
	else if(T == OR_OP) return 3;
	else if(T == ASSIGN ||T == PLUS_ASSIGN || T == MIN_ASSIGN || 
		T == MULT_ASSIGN || T == DIV_ASSIGN	) return 2;
	else if(T == COMMA  || T == LPARA || T == RPARA) return 1;
  	else return 0; 	
  	*/
	switch(T){
		case INC: case DECR: case UMINUS: case UPLUS: return 10;
		case DIV: case MULT: case MOD: case POWER1: case POWER2: case DIVDIV: return 9;
		case PLUS: case MINUS: case NOT_OP: return 8;
		case LSHIFT: case RSHIFT: return 7;
		case LOWER: case GREATER: case LO_EQ: case  GR_EQ: return 6;
		case  EQ_EQ: case NOT_EQ: case IS_EQ: return 5;
		case AND_OP: return 4;
		case OR_OP: return 3;
		case ASSIGN: case PLUS_ASSIGN: case MIN_ASSIGN: case MULT_ASSIGN: case DIV_ASSIGN: return 2;
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
	
	cprintf(111,"\t\t\t\t==[PARSER]==\n");

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll(head_obj);
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = head; //note that iter_prev->next is iter, always this is the case!
	struct fbgc_object * op_stack_head = new_fbgc_object(0);//make just 0 we are using for just head
	struct fbgc_object * gm_stack_head = new_fbgc_object(0); //grammar stack head

	uint8_t gm_error = 1;

	for(int i = 0; i<3000 && gm_error != 0 && iter != head->tail ; i++){

		cprintf(010,"----------------------[%d] = {%s}-----------------------\n",i,object_name_array[iter->type]);
		if(is_fbgc_NUMBER(iter->type)){
			//this is number do not touch
			iter_prev = iter;
			grammar_push(&gm_stack_head,NUMBER);
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

					/*if(op_stack_top(op_stack_head)->type) {
						basic_operator_handler(parser_stack2.back());
					}*/

					gm_error = grammar_seek_right(&gm_stack_head,op_stack_top(op_stack_head)->type);
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
			
			gm_error = grammar_seek_left(&gm_stack_head,iter->type);
			op_stack_head = op_stack_push(op_stack_head,iter);
			//print_fbgc_object(iter);
		}
		else if(iter->type == LPARA){
			iter_prev->next = iter->next;
			op_stack_head = op_stack_push(op_stack_head,iter);
			//iter = iter_prev->next;
			//print_fbgc_object(iter);
		}
		else if(iter->type == RPARA){

			iter_prev->next = iter->next;
			while(op_stack_top(op_stack_head)->type != LPARA){
				//Insert top op to the list  
				gm_error =grammar_seek_right(&gm_stack_head,op_stack_top(op_stack_head)->type);
				iter_prev->next = op_stack_top(op_stack_head);
				//Pop top from stack
				op_stack_head = op_stack_pop(op_stack_head);
				//connect list again
				iter_prev->next->next = iter->next; 
				//make the iter_prev proper
				iter_prev = iter_prev->next;
				
				//}
			}
			//op_stack_head = op_stack_push(op_stack_head,iter);
				
			gm_error = grammar_seek_left(&gm_stack_head,RPARA);

			if(op_stack_top(op_stack_head)->type == LPARA){
				//cprintf(111,"Hit the left para! content:%d i:%d\n",head_int->content,i);
				//now we have free paranthesis object by hand and also change the size of the main list
				head->size-=2;
				struct fbgc_object * dummy_for_lpara = op_stack_top(op_stack_head);
				op_stack_head = op_stack_pop(op_stack_head);
				free_fbgc_object(dummy_for_lpara);
				free_fbgc_object(iter);
			}
			
		}


		iter = iter_prev->next;
		print_fbgc_ll_object(head);
		op_stack_print(op_stack_head);
		grammar_stack_print(gm_stack_head);

	}
	
	if(!op_stack_empty(op_stack_head) && gm_error != 0){
		cprintf(011,"Op stack is not empty!\n");
		//now iter shows the tail of the main list 
		while(!op_stack_empty(op_stack_head)){
			gm_error = grammar_seek_right(&gm_stack_head,op_stack_top(op_stack_head)->type);
			iter_prev->next = op_stack_top(op_stack_head);
			op_stack_head = op_stack_pop(op_stack_head);
			iter_prev = iter_prev->next;
		}
		iter_prev->next = iter;	
	}

	free_fbgc_object(op_stack_head);
	grammar_free(gm_stack_head);
	return (struct fbgc_object*) head;
}
