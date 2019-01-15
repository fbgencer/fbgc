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
	cprintf(111,"[O]->");
	
	while(iter != NULL){
		cprintf(101,"[%s]->",object_name_array[iter->type]);
		iter = iter->next;
	}
	cprintf(100,"NULL\n");

}


struct 
fbgc_object * parser(struct fbgc_object * head,struct fbgc_object * tail){
	
	cprintf(111,"==[PARSER]==\n");

	struct fbgc_int_object * head_int = (struct fbgc_int_object*) head;
	struct fbgc_object * iter = head_int->base.next;
	struct fbgc_object * iter_prev = head; //note that iter_prev->next is iter, always this is the case!
	struct fbgc_object * op_stack_head = new_fbgc_object(0);//make just 0 we are using for just head
	

	for(int i = 0; i<3000 && iter != tail ; i++){


		
		//cprintf(010,"Input >>> [%d] = {%s}\n",i,object_name_array[iter->type]);
		if(is_fbgc_NUMBER(iter->type)){
			//this is number do not touch
			iter_prev = iter;
			//iter = iter->next;
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
			
			op_stack_head = op_stack_push(op_stack_head,iter);
			//iter = iter_prev->next;
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
			
			if(op_stack_top(op_stack_head)->type == LPARA){
				//cprintf(111,"Hit the left para! content:%d i:%d\n",head_int->content,i);
				//now we have free paranthesis object by hand and also change the size of the main list
				head_int->content -=2;
				struct fbgc_object * dummy_for_lpara = op_stack_top(op_stack_head);
				op_stack_head = op_stack_pop(op_stack_head);
				free_fbgc_object(dummy_for_lpara);
				free_fbgc_object(iter);

			}
			
		}
		iter = iter_prev->next;
		//op_stack_print(op_stack_head);
		//print_fbgc_object_ll(head);
	}
	
	if(!op_stack_empty(op_stack_head)){
		cprintf(011,"Op stack is not empty!\n");
		//now iter shows the tail of the main list 
		while(!op_stack_empty(op_stack_head)){
			iter_prev->next = op_stack_top(op_stack_head);
			op_stack_head = op_stack_pop(op_stack_head);
			iter_prev = iter_prev->next;
		}
		iter_prev->next = iter;	
	}

	free_fbgc_object(op_stack_head);
	return (struct fbgc_object*) head;
}
/*
		else if(isOP(lextok[i])){
			printf("OPERATOR : {%s}\n",TYPES_ARRAY[lextok[i]].c_str());

			//UNARYLERI DUZELT!!!!!!!!!!!!!!!!!
			//unary durumları
				//x=-5+3, x=-(-(-2)) , 
				// arkası operator|para|iter=sıfır önu kesinlikle sayı,var olmalı
				//unary -> (num|var|word|RPARA) unary (num|var|word|LPARA)
			if(lextok[i] == MINUS || lextok[i] == PLUS){
				if( (i == 0||isOP(lextok[i-1])||lextok[i-1] == LPARA) && 
					(isNUM(lextok[i+1])|| isWORD(lextok[i+1])|| isVAR(lextok[i+1])||
					lextok[i+1] == RPARA || lextok[i+1] == LPARA)
					
				  ){ 
				  		printf("UNARY SITUATION OCCURED\n");
						lextok[i] += 1;
						printf("OPERATOR : {%s}\n",TYPES_ARRAY[lextok[i]].c_str());  
						//if current token minus, minus+1 = unary minus is arranged.
						//Same as for plus. No need to know plus or minus.
					} 
			}

			//If operator is not unary type operator
			//if operator at the end of the stack2 is higher precedence or equal when compare
			//to current one then it must push onto the main stack. 
			if(parser_stack2.size() != 0 && opcompare(parser_stack2.back(),lextok[i])>=1){
				do{
				
					if(isBASIC_OPERATOR(parser_stack2.back())) {
						basic_operator_handler(parser_stack2.back());
						parser_stack2.pop_back();	
					}
					else {
						parser_stack.push_back(parser_stack2.back()); 
						parser_stack2.pop_back();
					}

				}while(opcompare(parser_stack2.back(),lextok[i])>=1);
			}

			parser_stack2.push_back(lextok[i]);
}
*/