#include "fbgc.h"



uint8_t operator_precedence(fbgc_token T){
	
	switch(T){

		case IF:case LOAD: case CFUN: return 25;
		case INC: case DECR: case UMINUS: case UPLUS: return 20;
		case SLASH: case STAR: case MOD: case CARET: case STARSTAR: case SLASHSLASH: return 18;
		case PLUS: case MINUS: case NOT_OP: return 16;
		case LSHIFT: case RSHIFT: return 14;
		case LOWER: case GREATER: case LO_EQ: case  GR_EQ: return 12;
		case EQ_EQ: case NOT_EQ: case IS_EQ: return 13;
		case AND_OP: return 12;
		case OR_OP: return 11;
		case ASSIGN: case PLUS_ASSIGN: case MINUS_ASSIGN: case STAR_ASSIGN: case SLASH_ASSIGN: return 10;
		case COMMA: return 8;
		case LPARA: return 6;
		case RPARA: return 5;
		case LBRACK: case SUBSCRIPT:return 4;
		case RBRACK: return 3;
		case SEMICOLON: return 2;
		case BEGIN:case IF_BEGIN:  case ELSE_BEGIN: case ELIF: case ELSE: return 1;
		case END: return 0;
		default: return 0;	
	}
}

uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
	// precedence outputs
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	if(stack_top == obj_type){
		switch(stack_top){
			case LPARA:
			case LBRACK:
			case ASSIGN:
			case COMMA:
			case IF_BEGIN:
			return 0;
		}
	}
	else if(obj_type == LPARA || obj_type == LBRACK) return 0; //work about this
	//else if(obj_type == IF) return 0;
	//else if(obj_type == IF_BEGIN) return 0;
	//precedence of the operators have to change according to their positions
	return (operator_precedence(stack_top) >= operator_precedence(obj_type));
}


#define is_pushable_in_main(x)(!is_fbgc_PARA(x) && \
x!= END && x!=IF_BEGIN && x!=ELSE_BEGIN && x!= SEMICOLON && x!= LOAD && x!= SUBSCRIPT)

uint8_t parser(struct fbgc_object ** field_obj){
	#ifdef PARSER_DEBUG
	cprintf(111,"--------------[parser_begin]-------------\n");
	#endif

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * head_obj =  cast_fbgc_object_as_field(*field_obj)->head;
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head; //note that iter_prev->next is iter, always this is the case!
	struct fbgc_object * op_stack_head = new_fbgc_ll_object(STACK);
	struct fbgc_grammar gm = {.flag = 0, .top = START};
	
	uint8_t gm_error = 1;



	for(int i = 0; i<500 && (iter != head->tail); i++){

		#ifdef PARSER_DEBUG
		cprintf(010,"----------------------[%d] = {%s:",i,object_name_array[iter->type]);
		print_fbgc_object(iter);
		cprintf(010,"}-----------------------\n");
		#endif


		if(is_fbgc_ATOM(iter->type) || iter->type == REFERENCE){
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);
		}
		else if(is_fbgc_OPERATOR(iter->type) || is_fbgc_FUNCTIONABLE(iter->type) 
			|| iter->type == IF_BEGIN || iter->type == ELSE_BEGIN ||
			iter->type == BEGIN || iter->type == END || iter->type == LOAD || iter->type == CFUN){
			
			//take the op object from main list and connect previous one to the next one 
			//[H]->[2]->[+]->[3] => [H]->[2]->[3], now iter shows the operator iter->next is [3] but we will change that too 
			//     p^	i^					
			iter_prev->next = iter->next;
			
			while( !is_empty_fbgc_ll_object(op_stack_head) && compare_operators(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)),iter->type) ){

				gm_error = gm_seek_right(&gm,top_fbgc_ll_object(op_stack_head));
				
				if(is_pushable_in_main(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)))){		
					//Insert top op to the list  
					iter_prev->next = top_fbgc_ll_object(op_stack_head);
					//Pop top from stack
					op_stack_head = pop_front_fbgc_ll_object(op_stack_head);
					//connect list again
					iter_prev->next->next = iter->next; 
					//make the iter_prev proper
					iter_prev = iter_prev->next;
					head->size++;	
				}
				else{
					//not pushables in main, like paranthesis 
					if(iter->type == RPARA && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LPARA){
						//balanced paranthesis, 
						delete_front_fbgc_ll_object(op_stack_head);
						break;
					}
					else if(iter->type == RBRACK && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LBRACK){
						//balanced brackets
						cprintf(111,"here\n");
						delete_front_fbgc_ll_object(op_stack_head);
						break;
					}
					else if(iter->type == RBRACK && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == SUBSCRIPT){
						//balanced brackets
						delete_front_fbgc_ll_object(op_stack_head);
						break;
					}					
					else if(iter->type == RBRACE && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LBRACE){
						//balanced brackets
						delete_front_fbgc_ll_object(op_stack_head);
						break;
					}					
					else if(iter->type == ELIF && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == IF_BEGIN){
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(JUMP));
						iter_prev = iter_prev->next;
						delete_front_fbgc_ll_object(op_stack_head);
						//cprintf(111,"LAN\n");
						break;
					}	
					else if(iter->type == ELSE && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == IF_BEGIN){
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(JUMP));
						iter_prev = iter_prev->next;
						top_fbgc_ll_object(op_stack_head)->type = ELSE_BEGIN;
						//cprintf(111,"LAN2\n");
						break;
					}									
					else if(iter->type == END && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == IF_BEGIN){
						//cprintf(111,"LAN\n");
						break;
					}
					else if(iter->type == END && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == ELSE_BEGIN){
						//cprintf(111,"LAN\n");
						break;
					}
					else if(iter->type == IF_BEGIN && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == ELSE_BEGIN){
						//cprintf(111,"LAN\n");
						break;
					}
					else if(iter->type == SEMICOLON && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LOAD ){
						cprintf(011,"seek_library_and_add!\n");
						//seek_library_and_add(iter);
						break;
					}
					else if(iter->type == SEMICOLON && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LBRACK ){
						break;
					}										
					else {
						cprintf(111,"GOTO END_OF_THE_PARSER Syntax Error :");
						cprintf(011,"Op Top :[%s], Iter:[%s]\n",object_name_array[get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))],object_name_array[iter->type]);
						//delete_front_fbgc_ll_object(op_stack_head);
						free_fbgc_object(iter);
						goto END_OF_THE_PARSER;
					}
				}	
			}
			#ifdef PARSER_DEBUG
				print_fbgc_ll_object(head_obj,"*M");
				print_fbgc_ll_object(op_stack_head,"*O");
				cprintf(101,"[*GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			#endif

			gm_error = gm_seek_left(&gm,iter);

			/*if(iter->type == RPARA){
				if(is_fbgc_FUNCTIONABLE(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)))) {
					if(	get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))== IF || 
						get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))== ELIF)
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(IF_BEGIN));
					else
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(BEGIN));
					cprintf(110,"[%s] object is inserted into main list\n",object_name_array[get_fbgc_object_type(iter_prev->next)]);
				}	
				if(iter_prev->type == COMMA) iter_prev->type = INT;
				free_fbgc_object(iter);
				head->size--;	
			}*/
			if(iter->type == RBRACK || iter->type == RPARA){
				if(gm.top == RAW_MATRIX || gm.top == RAW_TUPLE){
					if(iter_prev->type != COMMA){
						//cprintf(111,"Previous is not comma!\n");
						struct fbgc_object * comma_as_int = new_fbgc_int_object(1);
						comma_as_int->type = INT;					
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,comma_as_int);
						iter_prev = iter_prev->next;
					} else iter_prev->type = INT;
				}
				//nuple or subscript
				if(gm.top != MONUPLE){
					head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(gm.top));
					iter_prev = iter_prev->next;
				}
				free_fbgc_object(iter);
				head->size--;	
			}
			else if(iter->type == END){
				if( get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == BEGIN || 
					get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == IF_BEGIN ||
					get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == ELSE_BEGIN) {
					delete_front_fbgc_ll_object(op_stack_head);
					head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,iter);
					//cprintf(011,"END object is inserted into main list\n");
					iter_prev = iter;
				}
			}
			else if(iter->type == COMMA){
				if(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == COMMA){
					cast_fbgc_object_as_int(top_fbgc_ll_object(op_stack_head))->content++;
					free_fbgc_object(iter);
					head->size--;	
				}
				else{
					//now delete iterator,make new int object and hold comma as an int object but changing its type!
					free_fbgc_object(iter);
					struct fbgc_object * comma_as_int = new_fbgc_int_object(2);//2 elements for 1 comma
					comma_as_int->type = COMMA;
					op_stack_head = push_front_fbgc_ll_object(op_stack_head,comma_as_int);
				}
			}
			else if(iter->type == SEMICOLON){
				if(is_grammar_LBRACK_flag_open(gm.flag)){
					if(iter_prev->type != COMMA){
						//cprintf(111,"Previous is not comma!\n");
						struct fbgc_object * comma_as_int = new_fbgc_int_object(1);
						comma_as_int->type = INT;					
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,comma_as_int);
						iter_prev = iter_prev->next;
					} else iter_prev->type = INT;

					head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(ROW));
					iter_prev = iter_prev->next;					
				}
				
				free_fbgc_object(iter);
				head->size--;		
						
			}
			else if(iter->type == ELSE){
				head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,iter);
				iter_prev = iter;
			}			
			else{
				op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);
				head->size--;
			}
		}
		else{
			cprintf(100,"Error else in parser\n");
			break;	
		}	

		if(!gm_error) goto END_OF_THE_PARSER;

		iter = iter_prev->next;
		
		#ifdef PARSER_DEBUG
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op_stack_head,"O");
			cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
		#endif



	}

	END_OF_THE_PARSER:

	//make the linked list connection proper
	head->tail->next = iter_prev;

	free_fbgc_ll_object(op_stack_head);
	
	#ifdef PARSER_DEBUG
	cprintf(111,"--------------[parser_end]-------------\n");
	#endif

	return gm_error;
}
