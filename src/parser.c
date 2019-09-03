#include "fbgc.h"



#define is_pushable_in_main(x)(!is_fbgc_PARA(x) && x!= END )


uint8_t operator_precedence(fbgc_token T){
	
	switch(T){

		case IDENTIFIER: return 210;
		case PLUSPLUS: case MINUSMINUS: case UMINUS: case UPLUS: return 200;
		case SLASH: case STAR: case PERCENT: case CARET: case STARSTAR: case SLASHSLASH: return 180;
		case PLUS: case MINUS: case EXCLAMATION: return 160;
		case L_SHIFT: case R_SHIFT: return 140;
		case LOWER: case GREATER: case LO_EQ: case  GR_EQ: return 120;
		case EQ_EQ: case NOT_EQ: return 130;
		case AMPERSAND: return 120;
		case PIPE: return 110;
		case COLON: return 105;

		case ASSIGN: case PLUS_ASSIGN: case MINUS_ASSIGN: case STAR_ASSIGN: case SLASH_ASSIGN:
		return 100;
		//case ASSIGN_GLOBAL: case ASSIGN_LOCAL: return 100;

		case COMMA: return 80;

		case LPARA: case LBRACE: case LBRACK: return 10;

		//case IDENTIFIER: 
		case CFUN: return 7;		
		case RPARA: case RBRACE: case RBRACK: return 4;	
		case RETURN: return 3;
		case NEWLINE: return 2;
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
			default:
				result = 1;
			break;
		}
	}
	else if(obj_type == LPARA || obj_type == LBRACK || obj_type == IF || obj_type == END || obj_type == ELIF){
		return 0;
	}
	else
		//precedence of the operators have to change according to their positions
		result = (operator_precedence(stack_top) >= operator_precedence(obj_type));
	#ifdef PARSER_DEBUG
	cprintf(111,"Object type comparison stack top:[%s] >= obj[%s] : %d\n",object_name_array[stack_top],object_name_array[obj_type],result);
	#endif
	return result;
}


void handle_function_args(struct fbgc_object * fun_obj,struct fbgc_object * arg_end){
	/*
	//	arg_start is always next pt of fun_obj
	//	arg_start holds parsed arg expression beginning
	//	example: fun(x,y,z) will be parsed x,y,z,3,build_tuple,
	//							  arg_start^      arg_end^	
		
		There could be default assignment situations, we need to handle assignemnt expr and other type of args
	*/	
	#ifdef PARSER_DEBUG
	cprintf(111,"Function args handler\n");
	#endif

	cast_fbgc_object_as_fun(fun_obj)->no_arg = size_fbgc_tuple_object(cast_fbgc_object_as_fun(fun_obj)->code); 
	#ifdef PARSER_DEBUG
	cprintf(111,"Arg no : %d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);
	#endif
	fun_obj->next = arg_end->next;

	
	/*struct fbgc_object * arg_start = fun_obj->next;
	do{
		cprintf(111,"\nFun def while current type:\n");
		print_fbgc_object(arg_start);
		switch(arg_start->type){
			case LOAD_GLOBAL:
			{	

				//fun_local = push_back_fbgc_tuple_object(fun_local,cast_fbgc_object_as_ref(arg_start)->content);
				//cast_fbgc_object_as_ref(arg_start)->content = get_top_in_fbgc_tuple_object(fun_local);

				//cprintf(100,"fun local top %p ref cont %p \n",cast_fbgc_object_as_ref(arg_start)->content,get_top_in_fbgc_tuple_object(fun_local));
				//print_fbgc_object(cast_fbgc_object_as_ref(arg_start)->content);
				break;
			}
			case NUPLE:

			break;
			case BUILD_TUPLE:
			break;
			
			default:
			break;
		}

		arg_start = arg_start->next;	
	}while(arg_start != arg_end->next);*/

}

struct fbgc_object * handle_before_paranthesis(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	
	uint8_t gm_error = 1;

	if(iter_prev->type == COMMA) iter_prev->type = INT; 
	else {
		struct fbgc_object * ito = new_fbgc_int_object(1);
		if(gm->top == NUPLE) cast_fbgc_object_as_int(ito)->content = 0;
		ito->next = iter_prev->next;
		iter_prev->next = ito;
		iter_prev = ito;
	}

	#ifdef PARSER_DEBUG
	cprintf(111,">>gm.top %s\n",object_name_array[gm->top]);
	#endif
	/*if(gm.top == BUILD_TUPLE){
		if(iter_prev->type == COMMA) iter_prev->type = INT; 
		else cprintf(100,"Building tuple comma error!\n");
	}
	
	head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(gm.top));
	iter_prev = iter_prev->next;
	*/


	switch(get_fbgc_object_type(top_fbgc_ll_object(op))){
		case IDENTIFIER:
		case CFUN:
			cprintf(100,"Operator stack top NAME, this is a function call template!\n");

				gm_seek_right(gm,top_fbgc_ll_object(op));

				struct fbgc_object * iter = iter_prev->next;
				#ifdef PARSER_DEBUG
				cprintf(011,"in func gm.top :[%s]\n",
							object_name_array[gm->top]);
				#endif

				//Insert top op to the list  
				iter_prev->next = top_fbgc_ll_object(op);
				op = pop_front_fbgc_ll_object(op);
				iter_prev = iter_prev->next;
				iter_prev->next = new_fbgc_object(FUN_CALL);
				iter_prev = iter_prev->next;
				iter_prev->next = iter;
				

				return iter_prev;
		case IF:	cprintf(100,"Operator stack top IF, this is an if template!\n");
			gm_seek_right(gm,top_fbgc_ll_object(op));
			cast_fbgc_object_as_jumper(top_fbgc_ll_object(op))->content = iter_prev;
			return iter_prev;
		case ELIF:	cprintf(100,"Operator stack top ELIF, this is an if template!\n");
			gm_seek_right(gm,top_fbgc_ll_object(op));
			cast_fbgc_object_as_jumper(top_fbgc_ll_object(op))->content = iter_prev;
			return iter_prev;			
		case WHILE:	cprintf(100,"Operator stack top WHILE, this is an whle template!\n");
			gm_seek_right(gm,top_fbgc_ll_object(op));
			cast_fbgc_object_as_jumper(top_fbgc_ll_object(op))->content = iter_prev;
			return iter_prev;
		case FOR:	cprintf(100,"Operator stack top FOR, this is an for template!\n");
			gm_seek_right(gm,top_fbgc_ll_object(op));
			cast_fbgc_object_as_jumper(top_fbgc_ll_object(op))->content = iter_prev;
			return iter_prev;
		case FUN_MAKE:{
			cprintf(100,"Operator stack top FUN, this is an fun template!\n");
			//fun_make content holds function object, parse the arguments
			gm_seek_right(gm,top_fbgc_ll_object(op));
			
			struct fbgc_object * top = top_fbgc_ll_object(op);
			struct fbgc_object * fun_obj =  cast_fbgc_object_as_jumper(top_fbgc_ll_object(op))->content; 

			#ifdef PARSER_DEBUG
			cprintf(111,"\n\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
			cprintf(111,"Function making : arg start:"); print_fbgc_object(fun_obj->next);
			cprintf(111,"\nArg end:"); print_fbgc_object(iter_prev);
			#endif

			handle_function_args(fun_obj,iter_prev);

			//cprintf(111,"iter_prev \n"); print_fbgc_object(iter_prev);

			//cprintf(111,"fun next %s\n",object_name_array[fun_obj->next->type]);
			//cprintf(111,"iter_prev next %s\n",object_name_array[iter_prev->next->type]);
			iter_prev = fun_obj;
			#ifdef PARSER_DEBUG
			cprintf(111,"\n\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
			#endif

			return iter_prev;	
		}
		case LOAD:	cprintf(100,"Operator stack top LOAD, this is an load module template!\n");
			struct fbgc_object * top = top_fbgc_ll_object(op);


			return iter_prev;	
		default: 
			cprintf(100,"Operator stack top undefined, return old!\n");
		return iter_prev;
	}

}

struct fbgc_object * handle_before_brackets(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	#ifdef PARSER_DEBUG
	cprintf(111,">>%s<<",__FUNCTION__);
	#endif
	uint8_t gm_error = 1;


	

	if(iter_prev->type == COMMA) iter_prev->type = INT; 
	else {
		struct fbgc_object * ito = new_fbgc_int_object(1);
		ito->next = iter_prev->next;
		iter_prev->next = ito;
		iter_prev = ito;
	}

	if(top_fbgc_ll_object(op) != NULL && get_fbgc_object_type(top_fbgc_ll_object(op)) == IDENTIFIER)
	{
		cprintf(100,"Operator stack top load_local or global, this is a subscript call template!\n");

		gm_seek_right(gm,top_fbgc_ll_object(op));
		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		set_id_flag_SUBSCRIPT(top_fbgc_ll_object(op));
		iter_prev->next = top_fbgc_ll_object(op);
		op = pop_front_fbgc_ll_object(op);
		iter_prev = iter_prev->next;
		iter_prev->next = iter;

	}
	else{
		struct fbgc_object * iter = iter_prev->next;
		iter_prev->next = new_fbgc_object(gm->top);
		iter_prev = iter_prev->next;

		iter_prev->next = iter;
	}
	

	return iter_prev;
}


struct fbgc_object * new_cfun_object_from_str(struct fbgc_object * field_obj,const char * str){

	struct fbgc_ll_object * ll = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(field_obj)->modules );
	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)ll->base.next;
	while(cm!= NULL && (struct fbgc_object * )cm != ll->tail){
		const struct fbgc_cfunction * cc = cm->module->functions[0];
		//cprintf(111,"Functions:\n");
		for (int i = 1; cc!=NULL; ++i){
			int size_str = strlen(str);
			int size_fun_name = strlen(cc->name);
			if(size_str == size_fun_name && !memcmp(str,cc->name,size_str)){
				cprintf(010,"\n**Function [%s] is founded in module [%s]**\n",cc->name,cm->module->name);
				return new_fbgc_cfun_object(cc->function);
			} 
			//cprintf(101,"{%s}\n",cc->name);
			cc = cm->module->functions[i];
		}
		cm = (struct fbgc_cmodule_object * )cm->base.next;
	}

	cprintf(111,"Not a cfunction!\n");
	return NULL;
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
	
	struct fbgc_object * current_scope = *field_obj;

	uint8_t gm_error = 1;

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
		else if(iter->type == IDENTIFIER){
			gm_error = gm_seek_left(&gm,iter);

			cprintf(111,"current_scope :[%s]\n",object_name_array[current_scope->type]);

			struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,cast_fbgc_object_as_int(iter)->content);
			cprintf(100,">>>:"); print_fbgc_object(cstr_obj);

			struct fbgc_object * cf =  new_cfun_object_from_str(*field_obj,&cast_fbgc_object_as_cstr(cstr_obj)->content);

			if(cf != NULL){
				//remove identifier object, out cfun object into stack
				cf->next = iter->next;
				iter = cf;
			}
			else if(current_scope->type == FIELD){

				struct fbgc_object * local_array = cast_fbgc_object_as_field(current_scope)->locals;
				struct fbgc_identifier * temp_id; 
				int where = -1;//index_fbgc_array_object(local_tuple,name_obj);

				for(int i = 0; i<size_fbgc_array_object(local_array); i++){
					temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
					if(temp_id->name == cstr_obj) where = i; 
				}

				if(where == -1) {
					cprintf(111,"couldn't find in locals/field obj..\n");
					struct fbgc_identifier id;		
					id.name = cstr_obj; id.content = NULL;
					local_array = push_back_fbgc_array_object(local_array,&id);
					where = size_fbgc_array_object(local_array)-1;
					cast_fbgc_object_as_field(current_scope)->locals = local_array;
				}
				else cprintf(111,"Found at %d!",where);
				set_id_flag_GLOBAL(iter);
				cast_fbgc_object_as_id_opcode(iter)->loc = where;
				cprintf(111,"\n+++++++++++++++++++++++++++++++++++\n");
			}
			
			else if(current_scope->type == FUN){				
				struct fbgc_object * local_tuple = cast_fbgc_object_as_fun(current_scope)->code;
				int where = index_fbgc_tuple_object(local_tuple,cstr_obj);
				//iter->type = LOAD_LOCAL ;
				set_id_flag_LOCAL(iter);
				if(where == -1) {
					cprintf(111,"iter %s iter-nxt %s\n",object_name_array[iter->type],object_name_array[iter->next->type]);
					//-1 arg means func definition hasnt been done yet! So we are reading arguments
					if(iter->next->type == ASSIGN  || cast_fbgc_object_as_fun(current_scope)->no_arg == -1){
						cprintf(111,"couldn't find in locals of function obj..\n");
						local_tuple = push_back_fbgc_tuple_object(local_tuple,cstr_obj);
						where = size_fbgc_tuple_object(local_tuple)-1;
						cast_fbgc_object_as_fun(current_scope)->code = local_tuple;
						cprintf(100,"fun local tuple:["); print_fbgc_object(local_tuple); cprintf(100,"]\n");
					}
					else {
						local_tuple = cast_fbgc_object_as_field(*field_obj)->locals;
						struct fbgc_identifier * temp_id; 
						for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
							temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_tuple,i);
							if(temp_id->name == cstr_obj) where = i; 
						}						
						assert(where != -1);
						//cprintf(100,"field local tuple:["); print_fbgc_object(local_tuple); cprintf(100,"]\n");
						
						//iter->type = LOAD_GLOBAL;
						set_id_flag_GLOBAL(iter);
					}
				}
				
				//cast_fbgc_object_as_int(iter)->content = where;
				cast_fbgc_object_as_id_opcode(iter)->loc = where;
				cprintf(111,"\n+++++++++++++++++++++++++++++++++++\n");
			}

			iter_prev->next = iter->next;
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);

		}

		else if(iter->type == WHILE){
			gm_error = gm_seek_left(&gm,iter);	

			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);
			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,jump_obj);
			iter_prev->next = iter->next;	
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);	
			//iter_prev->next = iter->next;	

		}
		else if(iter->type == FOR){
			gm_error = gm_seek_left(&gm,iter);	

			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);	
			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,jump_obj);
			iter_prev->next = iter->next;	
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);	
			//iter_prev->next = iter->next;	
		}
		else if(iter->type == FUN_MAKE){
			//gm_error = gm_seek_left(&gm,iter);	
			struct fbgc_object * fun_obj = new_fbgc_fun_object();	
			fun_obj->next = iter->next;
			iter_prev->next = fun_obj;
			iter_prev = fun_obj;
			cast_fbgc_object_as_jumper(iter)->content = fun_obj;
			op_stack_head = push_front_fbgc_ll_object(op_stack_head,iter);

			current_scope = fun_obj;
			cast_fbgc_object_as_fun(fun_obj)->code = new_fbgc_tuple_object(0);
						
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

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_jumper(if_obj)->content = iter_prev;
			}
			else if(top_fbgc_ll_object(op_stack_head)->type == ELIF_BEGIN){
				//now insert if in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_jumper(if_obj)->content = iter_prev;
			}
			else if(top_fbgc_ll_object(op_stack_head)->type == WHILE_BEGIN || top_fbgc_ll_object(op_stack_head)->type == FOR_BEGIN){
				//now insert while in its place,
				struct fbgc_object * if_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				print_fbgc_ll_object(head_obj,"M:::");

				struct fbgc_object * jump_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				jump_obj->next = iter_prev->next;
				iter_prev->next = jump_obj;
				iter_prev = jump_obj;
				
				print_fbgc_ll_object(head_obj,"M2:::");

				cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;
				
				//iter_prev = jump_obj->next;
			}
			else if(top_fbgc_ll_object(op_stack_head)->type == FUN_MAKE){
				struct fbgc_object * fun_obj = cast_fbgc_object_as_jumper(top_fbgc_ll_object(op_stack_head))->content;
				cprintf(111,"iter_prev:%s\n",object_name_array[iter_prev->type]);
				cprintf(111,"iter_prev->next:%s\n",object_name_array[iter_prev->next->type]);
				cprintf(111,"iter:%s\n",object_name_array[iter->type]);

				cast_fbgc_object_as_fun(fun_obj)->no_locals = size_fbgc_tuple_object(cast_fbgc_object_as_fun(fun_obj)->code); 
				cast_fbgc_object_as_fun(fun_obj)->code = fun_obj->next;

				//fun_obj->next = iter->next;
				iter_prev->next = fun_obj;
				iter_prev = fun_obj;


				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);
				current_scope = *field_obj;
			}

			while(top_fbgc_ll_object(op_stack_head) != NULL && top_fbgc_ll_object(op_stack_head)->type == JUMP){
				struct fbgc_object * jump_obj = top_fbgc_ll_object(op_stack_head);
				op_stack_head = pop_front_fbgc_ll_object(op_stack_head);

				jump_obj->next = cast_fbgc_object_as_jumper(jump_obj)->content->next;
				cast_fbgc_object_as_jumper(jump_obj)->content->next = jump_obj;

				cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;	
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
				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);

				cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;

				cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;

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
				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);

				cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;
				op_stack_head = push_front_fbgc_ll_object(op_stack_head,jump_obj);

				cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;

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
					cast_fbgc_object_as_jumper(iter)->content = op_top;
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
					
					cast_fbgc_object_as_jumper(iter)->content = op_top;
					break;
				}
				op_top = op_top->next;
			}			
		}

		else if(is_fbgc_ASSIGNMENT_OPERATOR(iter->type)){
			cprintf(111,"I am here experimental\n");
			
			assert(top_fbgc_ll_object(op_stack_head)->type == IDENTIFIER);

			top_fbgc_ll_object(op_stack_head)->type = iter->type;

			/*switch(get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))){
				case LOAD_GLOBAL:
					top_fbgc_ll_object(op_stack_head)->type = ASSIGN_GLOBAL;
				break;
				case LOAD_LOCAL:
					top_fbgc_ll_object(op_stack_head)->type = ASSIGN_LOCAL;
				break;
				case LOAD_SUBSCRIPT:
					top_fbgc_ll_object(op_stack_head)->type = ASSIGN_SUBSCRIPT;
				break;	
				default:
					return 0;			
			}*/

		iter_prev->next = iter->next;
	
			//return 0;
		}
		else if(is_fbgc_OPERATOR(iter->type) || is_fbgc_PARA(iter->type) || iter->type == IF || iter->type == RETURN || iter->type == NEWLINE){
			
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



						/*if(top_fbgc_ll_object(op_stack_head) == NULL) break;
						cprintf(111,"After handling paranthesis, operator stack top :");
						cprintf(011,"Op Top :[%s], Iter_prev:[%s]\n",object_name_array[get_fbgc_object_type(top_fbgc_ll_object(op_stack_head))],
							object_name_array[iter_prev->type]);*/


						iter_prev = handle_before_paranthesis(iter_prev,op_stack_head,&gm);

						cprintf(010,"##############After handle paranthesis###############\n");
						print_fbgc_ll_object(head_obj,"M");
						print_fbgc_ll_object(op_stack_head,"O");
						cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
						cprintf(010,"###############$$$$################\n");

						break;
					}
					else if(iter->type == RBRACK && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LBRACK){
						cprintf(110,"=== RBRACK and LBRACK ===\n");

						delete_front_fbgc_ll_object(op_stack_head);
						gm_error = gm_seek_left(&gm,iter);

						if(gm.top != BUILD_MATRIX && gm.top != MONATRIX) cprintf(100,"\n\n ERROR BRACK \n\n");

						/*if(gm.top == BUILD_MATRIX){
							if(iter_prev->type == COMMA) iter_prev->type = INT; 
							else {
								struct fbgc_object * ito = new_fbgc_int_object(1);
								ito->next = iter_prev->next;
								iter_prev->next = ito;
								iter_prev = ito;
							}
						}
						head_obj = insert_next_fbgc_ll_object(head_obj,iter_prev,new_fbgc_object(gm.top));
						iter_prev = iter_prev->next;
						*/
						
						iter_prev = handle_before_brackets(iter_prev,op_stack_head,&gm);


						break;
					}
					else if(iter->type == SEMICOLON && get_fbgc_object_type(top_fbgc_ll_object(op_stack_head)) == LBRACK){

						cprintf(110,"\tBRACK AND SEMICOLON\n");
						if(iter_prev->type == COMMA) iter_prev->type = INT; 
						else {
							struct fbgc_object * ito = new_fbgc_int_object(1);
							ito->next = iter_prev->next;
							iter_prev->next = ito;
							iter_prev = ito;
						}

						struct fbgc_object * rowo = new_fbgc_object(ROW);
						rowo->next = iter_prev->next;
						iter_prev->next = rowo;
						iter_prev = rowo;
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

				//if(!gm_error) goto END_OF_THE_PARSER;
			}
			#ifdef PARSER_DEBUG
				print_fbgc_ll_object(head_obj,"*M");
				print_fbgc_ll_object(op_stack_head,"*O");
				cprintf(101,"[*GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			#endif

			if(iter->type != RPARA)	gm_error = gm_seek_left(&gm,iter);
			

			if(iter->type == RPARA || iter->type == RBRACK|| iter->type == SEMICOLON|| iter->type == NEWLINE || iter->type == ROW){
			}
			else if(iter->type == COMMA){

				assert(top_fbgc_ll_object(op_stack_head) != NULL);

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
			cprintf(100,"Error else in parser incoming obj %s|%d\n",object_name_array[iter->type],iter->type);
			break;	
		}	

		//if(!gm_error) goto END_OF_THE_PARSER;
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
	#ifdef PARSER_DEBUG
	cprintf(111,"Locals:");
	//:>print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	cprintf(111,"\n\n\n\n");
	return gm_error;

}
