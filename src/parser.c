#include "fbgc.h"

uint8_t operator_precedence(fbgc_token T){
	
	switch(T){

		case INC: case DECR: case UMINUS: case UPLUS: return 200;
		case SLASH: case STAR: case MOD: case CARET: case STARSTAR: case SLASHSLASH: return 180;
		case PLUS: case MINUS: case NOT_OP: return 160;
		case LSHIFT: case RSHIFT: return 140;
		case LOWER: case GREATER: case LO_EQ: case  GR_EQ: return 120;
		case EQ_EQ: case NOT_EQ: case IS_EQ: return 130;
		case AND_OP: return 120;
		case OR_OP: return 110;
		case ASSIGN: case PLUS_ASSIGN: case MINUS_ASSIGN: case STAR_ASSIGN: case SLASH_ASSIGN: return 100;
		case COMMA: return 80;

		case LPARA: case LBRACE: case LBRACK: return 5;

		case REFERENCE: return 3;		
		case RPARA: case RBRACE: case RBRACK: return 2;	
		case SEMICOLON: return 1;

		//case BEGIN:case IF_BEGIN:  case ELSE_BEGIN: case ELIF: case ELSE: return 1;
		//case END: return 0;
		default: return 0;	
	}
}

uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
	// precedence outputs
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	uint8_t result = 0;

	if(stack_top == obj_type){
		switch(stack_top){
			case LPARA:
			case LBRACK:
			case ASSIGN:
			case COMMA:
			result = 0;
			break;
		}
	}
	else if(obj_type == LPARA || obj_type == IF || obj_type == END || obj_type == ELIF){
		return 0;
	}
	else
		//precedence of the operators have to change according to their positions
		result = (operator_precedence(stack_top) >= operator_precedence(obj_type));



	cprintf(111,"Object type comparison stack top:[%s] >= obj[%s] : %d\n",object_name_array[stack_top],object_name_array[obj_type],result);

	return result;
}


#define is_pushable_in_main(x)(!is_fbgc_PARA(x) && x!= END )


struct fbgc_object * handle_before_paranthesis(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	
	uint8_t gm_error = 1;

	switch(get_fbgc_object_type(top_fbgc_ll_object(op))){
		case REFERENCE:
			cprintf(100,"Operator stack top REFERENCE, this is a function call template!\n");

				gm_seek_right(gm,top_fbgc_ll_object(op));

				struct fbgc_object * iter = iter_prev->next;
				cprintf(011,"in func Iter:[%s]\n",
							object_name_array[iter->type]);

				//Insert top op to the list  
				iter_prev->next = top_fbgc_ll_object(op);
				op = pop_front_fbgc_ll_object(op);
				print_fbgc_ll_object(op,"Op stack:");
				iter_prev = iter_prev->next;
				iter_prev->next = new_fbgc_object(FUN_CALL);
				iter_prev = iter_prev->next;
				iter_prev->next = iter;
				

				return iter_prev;
		case IF:
			cprintf(100,"Operator stack top IF, this is an if template!\n");

			 
			gm_seek_right(gm,top_fbgc_ll_object(op));
			
			cast_fbgc_object_as_if(top_fbgc_ll_object(op))->content = iter_prev;


			return iter_prev;
		case ELIF:
			cprintf(100,"Operator stack top ELIF, this is an if template!\n");

			gm_seek_right(gm,top_fbgc_ll_object(op));
			
			cast_fbgc_object_as_if(top_fbgc_ll_object(op))->content = iter_prev;


			return iter_prev;			

		case WHILE:
			cprintf(100,"Operator stack top WHILE, this is an whle template!\n");

			gm_seek_right(gm,top_fbgc_ll_object(op));
			
			cast_fbgc_object_as_if(top_fbgc_ll_object(op))->content = iter_prev;


			return iter_prev;
		default: 
			cprintf(100,"Operator stack top undefined, return old!\n");
		return iter_prev;
	}

}


uint8_t parser(struct fbgc_object ** field_obj){
	#ifdef PARSER_DEBUG
	cprintf(111,"==========[PARSER]==========\n");
	#endif

//print_fbgc_memory_block();

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * head_obj =  cast_fbgc_object_as_field(*field_obj)->head;
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head; //note that iter_prev->next is iter, always this is the case!
	struct fbgc_object * op_stack_head = new_fbgc_ll_object(STACK);
	struct fbgc_grammar gm = {.flag = 0, .top =  NEWLINE};
	
	uint8_t gm_error = 1;


	push_back_fbgc_ll_object(head_obj,new_fbgc_object(SEMICOLON));


	for(int i = 0;  (iter != head->tail); i++){

		#ifdef PARSER_DEBUG
		cprintf(010,"----------------------[%d] = {%s:",i,object_name_array[iter->type]);
		print_fbgc_object(iter);
		cprintf(010,"}-----------------------\n");
		#endif

 
		if(is_fbgc_ATOM(iter->type)){
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);
		}
		else if(iter->type == REFERENCE){
			gm_error = gm_seek_left(&gm,iter);

			if(iter->next->type == LPARA){
				iter_prev->next = iter->next;
				op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);

			}
			else {
				iter_prev = iter;
			}	
		}
		else if(iter->type == WHILE){
			gm_error = gm_seek_left(&gm,iter);	

			struct fbgc_object * jump_obj = new_fbgc_ref_object();
			jump_obj->type = JUMP;		
			cast_fbgc_object_as_ref(jump_obj)->content = iter_prev;
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,jump_obj);
			iter_prev->next = iter->next;	
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);	
			//iter_prev->next = iter->next;	

		}		
		else if(iter->type == END){
			//gm_error = gm_seek_left(&gm,iter);
			cprintf(010,"############## BEFORE END ###############\n");
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op_stack_head,"O");
			cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			cprintf(010,"###############$$$$################\n");


			if(top_fbgc_ll_object(op_stack_head)->type == IF_BEGIN){
				//now insert if in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				if_obj->next = cast_fbgc_object_as_if(if_obj)->content->next;
				cast_fbgc_object_as_if(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_if(if_obj)->content = iter_prev;
			}
			else if(top_fbgc_ll_object(op_stack_head)->type == ELIF_BEGIN){
				//now insert if in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				if_obj->next = cast_fbgc_object_as_if(if_obj)->content->next;
				cast_fbgc_object_as_if(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_if(if_obj)->content = iter_prev;
			}
			else if(top_fbgc_ll_object(op_stack_head)->type == WHILE_BEGIN){
				//now insert while in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				if_obj->next = cast_fbgc_object_as_if(if_obj)->content->next;
				cast_fbgc_object_as_if(if_obj)->content->next = if_obj;

				print_fbgc_ll_object(head_obj,"M:::");

				struct fbgc_object * jump_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				jump_obj->next = iter_prev->next;
				iter_prev->next = jump_obj;
				iter_prev = jump_obj;
				
				print_fbgc_ll_object(head_obj,"M2:::");

				cast_fbgc_object_as_if(if_obj)->content = jump_obj;
				
				//iter_prev = jump_obj->next;
			}

			while(top_fbgc_ll_object(op_stack_head) != NULL && top_fbgc_ll_object(op_stack_head)->type == JUMP){
				struct fbgc_object * jump_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				jump_obj->next = cast_fbgc_object_as_if(jump_obj)->content->next;
				cast_fbgc_object_as_if(jump_obj)->content->next = jump_obj;

				cast_fbgc_object_as_if(jump_obj)->content = iter_prev;	

			
			}

			iter_prev->next = iter->next;

			cprintf(010,"############## AFTER END ###############\n");
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op_stack_head,"O");
			cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			cprintf(010,"###############$$$$################\n");

		}
		else if(iter->type == ELIF){


			gm_error = gm_seek_right(&gm,top_fbgc_ll_object(op_stack_head));
			gm_error = gm_seek_left(&gm,iter);	

			if(top_fbgc_ll_object(op_stack_head)->type == IF_BEGIN || top_fbgc_ll_object(op_stack_head)->type == ELIF_BEGIN){

				//now insert if in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);
				if_obj->next = cast_fbgc_object_as_if(if_obj)->content->next;
				cast_fbgc_object_as_if(if_obj)->content->next = if_obj;

				struct fbgc_object * jump_obj = new_fbgc_ref_object();
				jump_obj->type = JUMP;

				cast_fbgc_object_as_ref(jump_obj)->content = iter_prev;

				cast_fbgc_object_as_if(if_obj)->content = jump_obj;

				op_stack_head = push_front_fbgc_ll_object(op_stack_head,jump_obj);
				
				iter_prev->next = iter->next;	
				op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);				
			}	

			//iter_prev->next = iter->next;		
		}
		else if(iter->type == ELSE){

			gm_error = gm_seek_right(&gm,top_fbgc_ll_object(op_stack_head));

			gm_error = gm_seek_left(&gm,iter);	

			if(top_fbgc_ll_object(op_stack_head)->type == IF_BEGIN || top_fbgc_ll_object(op_stack_head)->type == ELIF_BEGIN){

				//now insert if in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);
				if_obj->next = cast_fbgc_object_as_if(if_obj)->content->next;
				cast_fbgc_object_as_if(if_obj)->content->next = if_obj;

				struct fbgc_object * jump_obj = new_fbgc_ref_object();
				jump_obj->type = JUMP;

				cast_fbgc_object_as_ref(jump_obj)->content = iter_prev;
				op_stack_head = push_front_fbgc_ll_object(op_stack_head,jump_obj);

				cast_fbgc_object_as_if(if_obj)->content = jump_obj;

			}	

			iter_prev->next = iter->next;		
		}

		else if(iter->type == BREAK){
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);

			struct fbgc_object * op_top = top_fbgc_ll_object(op_stack_head);

			for(;;){

				if(op_top == NULL){
					cprintf(100,"Break without loop! \n");
					goto END_OF_THE_PARSER;
				}

				if(op_top->type == WHILE_BEGIN){
					cast_fbgc_object_as_ref(iter)->content = op_top;
					break;
				}
				op_top = op_top->next;
			}			
		}
		else if(iter->type == CONT){
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);

			struct fbgc_object * op_top = top_fbgc_ll_object(op_stack_head);

			for(;;){

				if(op_top == NULL){
					cprintf(100,"Break without loop! \n");
					goto END_OF_THE_PARSER;
				}

				if(op_top->type == WHILE_BEGIN){
					//here we assumed there will be always jump object after while
					op_top = op_top->next;
					
					cast_fbgc_object_as_ref(iter)->content = op_top;
					break;
				}
				op_top = op_top->next;
			}			
		}
		else if(is_fbgc_OPERATOR(iter->type) || iter->type == IF || iter->type == ELSE){
			
			//take the op object from main list and connect previous one to the next one 
			//[H]->[2]->[+]->[3] => [H]->[2]->[3], now iter holds the operator, iter->next is [3] but we will change that too 
			//     p^	i^					
			iter_prev->next = iter->next;
			
			while( !is_empty_fbgc_ll_object(op_stack_head) && compare_operators(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)),iter->type) ){

				cprintf(100,"Inside while!\n");

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
					
				}
				else{
					//not pushables in main, like paranthesis 
					if(iter->type == RPARA && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LPARA){
						//balanced paranthesis now search other objects in the stack top is lpara but what is the previous ? 

						cprintf(110,"=== Rpara and Lpara ===\n");

						delete_front_fbgc_ll_object(op_stack_head);
						gm_error = gm_seek_left(&gm,iter);

						if(gm.top == BUILD_TUPLE){
							if(iter_prev->type == COMMA) iter_prev->type = INT; 
							else cprintf(100,"Building tuple comma error!\n");
						}
						//nuple or subscript
						
						if(gm.top != MONUPLE){
							head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(gm.top));
							iter_prev = iter_prev->next;
						}	


						if(top_fbgc_ll_object(op_stack_head) == NULL) break;
						cprintf(111,"After handling paranthesis, operator stack top :");
						cprintf(011,"Op Top :[%s], Iter_prev:[%s]\n",object_name_array[get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))],
							object_name_array[iter_prev->type]);


						iter_prev = handle_before_paranthesis(iter_prev,op_stack_head,&gm);
						//iter_prev = iter;

						cprintf(010,"##############After handle paranthesis###############\n");
						print_fbgc_ll_object(head_obj,"M");
						print_fbgc_ll_object(op_stack_head,"O");
						cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
						cprintf(010,"###############$$$$################\n");

						//iter = iter->next;

						break;


					}
					else {
						cprintf(111,"GOTO END_OF_THE_PARSER Syntax Error :");
						cprintf(011,"Op Top :[%s], Iter:[%s]\n",object_name_array[get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))],object_name_array[iter->type]);
						goto END_OF_THE_PARSER;
					}

				}

					#ifdef PARSER_DEBUG
						cprintf(010,"##############WHile###############\n");
						print_fbgc_ll_object(head_obj,"M");
						print_fbgc_ll_object(op_stack_head,"O");
						cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
						cprintf(010,"###############$$$$################\n");
					#endif

				if(!gm_error) goto END_OF_THE_PARSER;
			}
			#ifdef PARSER_DEBUG
				print_fbgc_ll_object(head_obj,"*M");
				print_fbgc_ll_object(op_stack_head,"*O");
				cprintf(101,"[*GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			#endif

			if(iter->type != RPARA)
			gm_error = gm_seek_left(&gm,iter);
			

			if(iter->type == RPARA || iter->type == SEMICOLON){
			}
			else if(iter->type == COMMA){

				if(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == COMMA){
					cast_fbgc_object_as_int(top_fbgc_ll_object(op_stack_head))->content++;
					//free_fbgc_object(iter);
					head->size--;	
				}
				else{
					//now delete iterator,make new int object and hold comma as an int object but changing its type!
					//free_fbgc_object(iter);
					struct fbgc_object * comma_as_int = new_fbgc_int_object(2);//2 elements for 1 comma
					comma_as_int->type = COMMA;
					op_stack_head = push_front_fbgc_ll_object(op_stack_head,comma_as_int);
				}
			}
			else {
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
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif

	return gm_error;
}
