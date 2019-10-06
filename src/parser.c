#include "fbgc.h"



#define is_pushable_in_main(x)(!is_fbgc_PARA(x) && x != LEN)

/*


*/




/*
uint8_t operator_precedence(fbgc_token T){
	
	switch(T){

		case IDENTIFIER: return 50;
		case PLUSPLUS: case MINUSMINUS: case UMINUS: case UPLUS: return 48;
		case SLASH: case STAR: case PERCENT: case CARET: case STARSTAR:
		case SLASHSLASH: return 46;
		
		case PLUS: case MINUS: case EXCLAMATION: case TILDE: return 44;
		case L_SHIFT: case R_SHIFT: return 42;
		case LOWER: case GREATER: case LO_EQ: case  GR_EQ: return 40;
		case EQ_EQ: case NOT_EQ: return 38;
		case AMPERSAND: return 36;
		case PIPE: case LEN: return 34;
		case COLON: return 32;

		case  ASSIGN:
		case  R_SHIFT_ASSIGN:
		case  L_SHIFT_ASSIGN:
		case  STARSTAR_ASSIGN:
		case  SLASHSLASH_ASSIGN:
		case  PLUS_ASSIGN:
		case  MINUS_ASSIGN:
		case  STAR_ASSIGN:
		case  SLASH_ASSIGN:
		case  CARET_ASSIGN:
		case  PERCENT_ASSIGN:
		return 30;

		case COMMA: return 20;

		case LPARA: case LBRACE: case LBRACK: return 10;
		case CFUN: return 7;		
		case RPARA: case RBRACE: case RBRACK: return 6;	
		case RETURN: return 3;
		case NEWLINE: return 2;
		case SEMICOLON: return 1;
		default: return 0;	
	}
}*/

#define RIGHT_ASSOC 0b10000000
#define LEFT_ASSOC 0	
const fbgc_token const precedence_table[] =
{
	0,//IF
	3,//RETURN
	2,//NEWLINE
	RIGHT_ASSOC | 10,//LPARA
	6,//RPARA
	RIGHT_ASSOC | 10,//LBRACK
	6,//RBRACK
	RIGHT_ASSOC | 10,//LBRACE
	6,//RBRACE
	RIGHT_ASSOC | 48,//UMINUS
	RIGHT_ASSOC | 48,//UPLUS
	RIGHT_ASSOC | 44,//TILDE
	32,//COLON
	RIGHT_ASSOC | 20,//COMMA
	52,//DOT
	1,//SEMICOLON
	42,//R_SHIFT
	42,//L_SHIFT
	RIGHT_ASSOC | 46,//STARSTAR
	RIGHT_ASSOC | 46,//SLASHSLASH
	44,//PLUS
	44,//MINUS
	46,//STAR
	46,//SLASH
	RIGHT_ASSOC | 46,//CARET
	46,//PERCENT
	40,//LO_EQ
	40,//GR_EQ
	38,//EQ_EQ
	38,//NOT_EQ
	40,//LOWER
	40,//GREATER
	41,//PIPE
	36,//AMPERSAND
	40,//EXCLAMATION
	RIGHT_ASSOC | 30,//ASSIGN
	RIGHT_ASSOC | 30,//R_SHIFT_ASSIGN
	RIGHT_ASSOC | 30,//L_SHIFT_ASSIGN
	RIGHT_ASSOC | 30,//STARSTAR_ASSIGN
	RIGHT_ASSOC | 30,//SLASHSLASH_ASSIGN
	RIGHT_ASSOC | 30,//PLUS_ASSIGN
	RIGHT_ASSOC | 30,//MINUS_ASSIGN
	RIGHT_ASSOC | 30,//STAR_ASSIGN
	RIGHT_ASSOC | 30,//SLASH_ASSIGN
	RIGHT_ASSOC | 30,//CARET_ASSIGN
	RIGHT_ASSOC | 30,//PERCENT_ASSIGN
	41,//LEN
};

uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
	// precedence outputs
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	uint8_t result = 0;
	#ifdef PARSER_DEBUG
	cprintf(100,"Object type comparison stack top:[%s] >= obj[%s] ",object_name_array[stack_top],object_name_array[obj_type]);
	#endif

	/*if(stack_top == obj_type){
		switch(stack_top){
			case LPARA:
			case LBRACK:
			case ASSIGN:
			case COMMA:
			case STARSTAR:
			case CARET:
				result = 0;
				break;
			default:
				result = 1;
			break;
		}
	}
	else */
	if(obj_type == NEWLINE && stack_top == COMMA) result = 0;
	else if(obj_type == LPARA || obj_type == LBRACK) result =  0;
	else if(stack_top == IDENTIFIER) result = 1;

	else if(obj_type >= IF && obj_type <= LEN && stack_top >= IF && stack_top <= LEN ){
		//precedence of the operators have to change according to their positions
		//result = (operator_precedence(stack_top) >= operator_precedence(obj_type));
		stack_top = precedence_table[stack_top - IF];
		obj_type = precedence_table[obj_type - IF];
		result = (0x7F & stack_top) > (0x7F & obj_type); // is there another way to perform this calculation ?? 
		if(result == 0 && obj_type == stack_top){
			result = !(0x80 & obj_type); 
		}
	}

	#ifdef PARSER_DEBUG
	cprintf(100,": %d\n",result && 1);
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
	
}

struct fbgc_object * handle_before_paranthesis(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	
	uint8_t gm_error = 1;


	fbgc_token top_type = get_fbgc_object_type(TOP_LL(op));

	#ifdef PARSER_DEBUG
	cprintf(111,"In %s op top [%s]\n",__FUNCTION__,object_name_array[top_type]);
	cprintf(111,">>gm.top %s\n",object_name_array[gm->top]);
	#endif

	//know that iter_prev is iter_prev_prev!!

 
	if(top_type == IDENTIFIER || top_type == CFUN){
		#ifdef PARSER_DEBUG
		cprintf(100,"Operator stack top ID or CFUN, this is a function call template!\n");
		print_fbgc_object(iter_prev);
		
		#endif

		if(iter_prev->type == COMMA) iter_prev->type = INT; 
		else {
			struct fbgc_object * ito = new_fbgc_int_object(gm->top != NUPLE);
			ito->next = iter_prev->next;
			iter_prev->next = ito;
			iter_prev = ito;
		}

		gm_seek_right(gm,TOP_LL(op));
		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		iter_prev->next = TOP_LL(op);
		POP_LL(op);
		
		if(iter_prev->next->type != CFUN && is_id_flag_MEMBER(iter_prev->next) ){
			//method call
			iter_prev->next->type = METHOD_CALL;
			iter_prev = iter_prev->next;
		}else{
			//function call

			iter_prev = iter_prev->next;
			iter_prev->next = new_fbgc_object(FUN_CALL);
			iter_prev = iter_prev->next;
		}

		if(is_empty_fbgc_ll_object(op) && TOP_LL(op)->type != ASSIGN && top_type != CFUN ){
			iter_prev->next = new_fbgc_object(POP_TOP);
			iter_prev = iter_prev->next;			
		}

		iter_prev->next = iter;
		

		/*if(iter_prev->type == COMMA) iter_prev->type = INT; 
		else {
			//if gm->top == NUPLE put zero so we won't push anything from stack
			struct fbgc_object * ito = 
			derive_from_new_int_object(INT, (gm->top != NUPLE) );
			ito->next = iter_prev->next;
			iter_prev->next = ito;
			iter_prev = ito;
		}

		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		iter_prev->next = TOP_LL(op);
		op = POP_LL(op);
		iter_prev = iter_prev->next;
		iter_prev->next = iter;
		iter_prev = iter_prev->next;*/
		//iter_prev->next = iter;
		
	}
	else if( top_type == FUN_MAKE){
		#ifdef PARSER_DEBUG
		cprintf(100,"Operator stack top FUN, this is an fun template!\n");
		#endif
		//fun_make content holds function object, parse the arguments
		gm_seek_right(gm,TOP_LL(op));
		

		struct fbgc_object * top = TOP_LL(op);
		struct fbgc_object * fun_obj =  cast_fbgc_object_as_jumper(TOP_LL(op))->content; 

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

	}
	else if(top_type == FOR){
		gm_seek_right(gm,TOP_LL(op));
		#ifdef PARSER_DEBUG
		cprintf(111,"This was for, not it is [%s]\n",object_name_array[TOP_LL(op)->type]);
		cprintf(100,"C :%s\n",object_name_array[cast_fbgc_object_as_jumper(TOP_LL(op))->content->type]);
		cprintf(100,"iter_prev %s\n",object_name_array[iter_prev->type]);
		#endif
		struct fbgc_object * d = cast_fbgc_object_as_jumper(TOP_LL(op))->content;
		while(d->next != iter_prev)
			d = d->next;

		struct fbgc_object * i =  new_fbgc_int_object(-1);
		i->next = d->next;
		d->next = i;
		cast_fbgc_object_as_jumper(TOP_LL(op))->content = i;
	}	
	else{

		if(gm->top != MONUPLE){
			
			if(iter_prev->type == COMMA){
				iter_prev->type = BUILD_TUPLE;
			} 
			else {
				//Actually this is always zero ? Check this later
				struct fbgc_object * ito = 
				derive_from_new_int_object(BUILD_TUPLE,(gm->top != NUPLE));
				ito->next = iter_prev->next;
				iter_prev->next = ito;
				iter_prev = ito;
			}	
			//If nothing than it's a tuple!
		}
		//else iter_prev = iter_prev->next;


		if(top_type == IF || top_type == ELIF || top_type == WHILE){
			#ifdef PARSER_DEBUG
			cprintf(100,"Operator stack top %s, this is an if template!\n",object_name_array[top_type]);
			#endif

			gm_seek_right(gm,TOP_LL(op));
			cast_fbgc_object_as_jumper(TOP_LL(op))->content = iter_prev;
			#ifdef PARSER_DEBUG
			cprintf(111,"if shows %s\n",object_name_array[iter_prev->next->type]);	
			#endif		
		}
	}

	return iter_prev;

}

struct fbgc_object * handle_before_brackets(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	#ifdef PARSER_DEBUG
	cprintf(111,">>%s<<\n",__FUNCTION__);
	cprintf(111,"iter_prev %s\n",object_name_array[iter_prev->type]);
	#endif
	uint8_t gm_error = 1;

	if(iter_prev->type == COMMA) iter_prev->type = INT; 
	else {
		struct fbgc_object * ito = new_fbgc_int_object(1);
		ito->next = iter_prev->next;
		iter_prev->next = ito;
		iter_prev = ito;
	}

	#ifdef PARSER_DEBUG
	if(TOP_LL(op) != NULL){
		cprintf(110,"############## IN bracket closing ###############\n");
		print_fbgc_ll_object(op,"O");
		cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm->top],gm->flag);
		cprintf(110,"###############$$$$################\n");		
	}
	#endif

	if(TOP_LL(op) != NULL &&  get_fbgc_object_type(TOP_LL(op)) == IDENTIFIER)
	{
		#ifdef PARSER_DEBUG
		cprintf(100,"Operator stack top load_local or global, this is a subscript call template!\n");
		#endif

		gm_seek_right(gm,TOP_LL(op));
		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		set_id_flag_SUBSCRIPT(TOP_LL(op));
	}
	else{

		iter_prev->type = BUILD_MATRIX;
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
			//optimize strlen part
			if(!my_strcmp(str,cc->name) ){
				#ifdef PARSER_DEBUG
				cprintf(010,"\n**Function [%s] matched with str [%s]\n",cc->name,str);
				#endif
				return new_fbgc_cfun_object(cc->function);
			} 
			//cprintf(101,"{%s}\n",cc->name);
			cc = cm->module->functions[i];
		}
		cm = (struct fbgc_cmodule_object * )cm->base.next;
	}
	#ifdef PARSER_DEBUG
	cprintf(111,"Not a cfunction!\n");
	#endif
	return NULL;
}



uint8_t parser(struct fbgc_object ** field_obj){
	#ifdef PARSER_DEBUG
	cprintf(111,"==========[PARSER]==========\n");
	#endif

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * head_obj =  cast_fbgc_object_as_field(*field_obj)->head;
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head; //note that iter_prev->next is iter, always this is the case!
	struct fbgc_object * op = new_fbgc_ll_object();
	struct fbgc_grammar gm = {.flag = 0, .top =  NEWLINE};
	
	struct fbgc_object * current_scope = *field_obj;

	uint8_t gm_error = 1;

	for(int i = 0;  (iter != head->tail); i++){

	#ifdef PARSER_DEBUG
	cprintf(010,"----------------------[%d] = {%s:",i,object_name_array[iter->type]);
	print_fbgc_object(iter);
	cprintf(010,"}-----------------------\n");
	#endif
	
 	switch(iter->type){
 		case LOGIC:
 		case INT:
 		case DOUBLE:
 		case COMPLEX:
 		case STRING:
 		{
 			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);	
 			break;
 		}
 		case IDENTIFIER:
 		{
			gm_error = gm_seek_left(&gm,iter);

			#ifdef PARSER_DEBUG
			cprintf(111,"current_scope :[%s]\n",object_name_array[current_scope->type]);
			#endif

			struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,cast_fbgc_object_as_id_opcode(iter)->loc);
			#ifdef PARSER_DEBUG
			cprintf(100,">>>:"); print_fbgc_object(cstr_obj);
			cprintf(100,"\n");
			#endif
			
			struct fbgc_object * cf = NULL;

			if(TOP_LL(op) != NULL && TOP_LL(op)->type == DOT){
				//so this is just a member selection
				POP_LL(op);

				/*struct fbgc_object * cstr = new_fbgc_cstr_object(&cast_fbgc_object_as_cstr(cstr_obj)->content); 
				cstr->next = iter->next;
				iter_prev->next = cstr;
				iter_prev=cstr;*/
				cast_fbgc_object_as_id_opcode(iter)->member_name = cstr_obj;
				set_id_flag_MEMBER(iter);
			}
			else if((cf = new_cfun_object_from_str(*field_obj,&cast_fbgc_object_as_cstr(cstr_obj)->content)) != NULL){
				//remove identifier object, put cfun object into stack
				cf->next = iter->next;
				iter = cf;
			}
			else if(current_scope->type == FIELD){

				struct fbgc_object * local_array = cast_fbgc_object_as_field(current_scope)->locals;
				struct fbgc_identifier * temp_id; 
				int where = -1;

				for(int i = 0; i<size_fbgc_array_object(local_array); i++){
					temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
					if(temp_id->name == cstr_obj) where = i; 
				}

				if(where == -1) {
					#ifdef PARSER_DEBUG
					cprintf(111,"couldn't find in locals/field obj| creating variable inside global field\n");
					#endif
					struct fbgc_identifier id;		
					id.name = cstr_obj; id.content = NULL;
					local_array = push_back_fbgc_array_object(local_array,&id);
					where = size_fbgc_array_object(local_array)-1;
					cast_fbgc_object_as_field(current_scope)->locals = local_array;
					
					#ifdef PARSER_DEBUG
					cprintf(111,"We put at where %d\n",where);
					#endif
				}
				else{
					#ifdef PARSER_DEBUG
					cprintf(111,"Found at %d!",where);
					#endif
				}
				set_id_flag_GLOBAL(iter);
				cast_fbgc_object_as_id_opcode(iter)->loc = where;
				#ifdef PARSER_DEBUG
				cprintf(111,"\n+++++++++++++++++++++++++++++++++++\n");
				#endif
			}
			
			else if(current_scope->type == FUN){				
				struct fbgc_object * local_tuple = cast_fbgc_object_as_fun(current_scope)->code;
				int where = index_fbgc_tuple_object(local_tuple,cstr_obj);
				//iter->type = LOAD_LOCAL ;
				set_id_flag_LOCAL(iter);
				if(where == -1) {
					#ifdef PARSER_DEBUG
					cprintf(111,"iter %s iter-nxt %s\n",object_name_array[iter->type],object_name_array[iter->next->type]);
					#endif
					//-1 arg means func definition hasnt been done yet! So we are reading arguments
					if(iter->next->type == ASSIGN  || cast_fbgc_object_as_fun(current_scope)->no_arg == -1){
						#ifdef PARSER_DEBUG
						cprintf(111,"couldn't find in locals of function obj..\n");
						#endif
						local_tuple = push_back_fbgc_tuple_object(local_tuple,cstr_obj);
						where = size_fbgc_tuple_object(local_tuple)-1;
						cast_fbgc_object_as_fun(current_scope)->code = local_tuple;
						#ifdef PARSER_DEBUG
						cprintf(100,"fun local tuple:["); print_fbgc_object(local_tuple); cprintf(100,"]\n");
						#endif
					}
					else {
						local_tuple = cast_fbgc_object_as_field(*field_obj)->locals;
						struct fbgc_identifier * temp_id; 
						for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
							temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_tuple,i);
							if(temp_id->name == cstr_obj) where = i; 
						}						
						
						if(where == -1){
							cprintf(100,"%s is not defined before\n",&cast_fbgc_object_as_cstr(cstr_obj)->content);
							assert(0);
						}
						//cprintf(100,"field local tuple:["); print_fbgc_object(local_tuple); cprintf(100,"]\n");
						
						//iter->type = LOAD_GLOBAL;
						set_id_flag_GLOBAL(iter);
					}
				}
				
				//cast_fbgc_object_as_int(iter)->content = where;
				cast_fbgc_object_as_id_opcode(iter)->loc = where;
				#ifdef PARSER_DEBUG
				cprintf(111,"\n+++++++++++++++++++++++++++++++++++\n");
				#endif
			}

			iter_prev->next = iter->next;
			push_front_fbgc_ll_object(op,iter);
 			break;
 		}
		case END:
		{
			//gm_error = gm_seek_left(&gm,iter);
			#ifdef PARSER_DEBUG
			cprintf(010,"############## BEFORE END ###############\n");
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op,"O");
			cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			cprintf(010,"###############$$$$################\n");
			#endif


			if(TOP_LL(op)->type == IF_BEGIN){
				//now insert if in its place,
				struct fbgc_object * if_obj = TOP_LL(op);
				POP_LL(op);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_jumper(if_obj)->content = iter_prev;
			}
			else if(TOP_LL(op)->type == ELIF_BEGIN){
				//now insert if in its place,
				struct fbgc_object * if_obj = TOP_LL(op);
				POP_LL(op);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_jumper(if_obj)->content = iter_prev;
			}
			else if(TOP_LL(op)->type == WHILE_BEGIN){
				//now insert while in its place,
				struct fbgc_object * if_obj = TOP_LL(op);
				POP_LL(op);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				struct fbgc_object * jump_obj = TOP_LL(op);
				POP_LL(op);

				jump_obj->next = iter_prev->next;
				iter_prev->next = jump_obj;
				iter_prev = jump_obj;
				
				cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;
			}
			else if(TOP_LL(op)->type == FOR_BEGIN){
				//now insert while in its place,
				struct fbgc_object * if_obj = TOP_LL(op);
				POP_LL(op);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;



				struct fbgc_object * jump_obj = TOP_LL(op);
				POP_LL(op);

				cast_fbgc_object_as_jumper(jump_obj)->content = 
				cast_fbgc_object_as_jumper(if_obj)->content;	

				jump_obj->next = iter_prev->next;
				iter_prev->next = jump_obj;
				iter_prev = jump_obj;
				
				cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;

			}
			else if(TOP_LL(op)->type == FUN_MAKE){
				assert(iter_prev->type != FUN); 
				struct fbgc_object * fun_obj = cast_fbgc_object_as_jumper(TOP_LL(op))->content;
				
				#ifdef PARSER_DEBUG
				cprintf(111,"iter_prev:%s\n",object_name_array[iter_prev->type]);
				cprintf(111,"iter_prev->next:%s\n",object_name_array[iter_prev->next->type]);
				cprintf(111,"iter:%s\n",object_name_array[iter->type]);
				#endif

				cast_fbgc_object_as_fun(fun_obj)->no_locals = size_fbgc_tuple_object(cast_fbgc_object_as_fun(fun_obj)->code); 
				cast_fbgc_object_as_fun(fun_obj)->code = fun_obj->next;

				//This is commented to just save the day..
				//if(iter_prev->type != RETURN){
					//add NIL object 
					iter_prev->next = new_fbgc_object(NIL);
					iter_prev = iter_prev->next;
					iter_prev->next = new_fbgc_object(RETURN);
					iter_prev = iter_prev->next;
				//}

				iter_prev->next = fun_obj; //last token in function code shows func object
				iter_prev = fun_obj;


				POP_LL(op);
				current_scope = *field_obj;
			}
			//else assert(0);

			while(TOP_LL(op) != NULL && TOP_LL(op)->type == JUMP){
				struct fbgc_object * jump_obj = TOP_LL(op);
				POP_LL(op);

				jump_obj->next = cast_fbgc_object_as_jumper(jump_obj)->content->next;
				cast_fbgc_object_as_jumper(jump_obj)->content->next = jump_obj;

				cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;	
			}

			iter_prev->next = iter->next;

			#ifdef PARSER_DEBUG
			cprintf(010,"############## AFTER END ###############\n");
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op,"O");
			cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
			cprintf(010,"###############$$$$################\n");
			#endif

			break;			
		}
		case FUN_MAKE:
		{
			//gm_error = gm_seek_left(&gm,iter);	
			struct fbgc_object * fun_obj = new_fbgc_fun_object();	
			fun_obj->next = iter->next;
			iter_prev->next = fun_obj;
			iter_prev = fun_obj;
			cast_fbgc_object_as_jumper(iter)->content = fun_obj;
			push_front_fbgc_ll_object(op,iter);

			current_scope = fun_obj;
			cast_fbgc_object_as_fun(fun_obj)->code = new_fbgc_tuple_object(0);	
			break;		
		}
		case ELIF:
		{
			gm_error = gm_seek_right(&gm,TOP_LL(op));
			gm_error = gm_seek_left(&gm,iter);	

			assert(TOP_LL(op) != NULL && 
				(TOP_LL(op)->type == IF_BEGIN || TOP_LL(op)->type == ELIF_BEGIN));

			//now insert if in its place,
			struct fbgc_object * if_obj = TOP_LL(op);
			POP_LL(op);
			if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
			cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);

			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;

			cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;

			push_front_fbgc_ll_object(op,jump_obj);
			iter_prev->next = iter->next;	
			push_front_fbgc_ll_object(op,iter);				

			break;
		}
		case ELSE:
		{

			gm_error = gm_seek_right(&gm,TOP_LL(op));
			gm_error = gm_seek_left(&gm,iter);	

			assert(TOP_LL(op)->type == IF_BEGIN || TOP_LL(op)->type == ELIF_BEGIN);

			//now insert if in its place,
			struct fbgc_object * if_obj = TOP_LL(op);
			POP_LL(op);
			if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
			cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);

			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;
			push_front_fbgc_ll_object(op,jump_obj);

			cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;

			

			iter_prev->next = iter->next;				
			break;
		}
		case WHILE:
		{
			gm_error = gm_seek_left(&gm,iter);	



			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);
			if(TOP_LL(op) != NULL && TOP_LL(op)->type == WHILE_BEGIN){
				//This is a branch optimization to solve an issue between two while loops
				//if we have a situation
				// while(a)
				//	while(b)
				//jump object of second while holds 'a' to jump because a->next is b
				//however, second while loop will be inserted between 'a' and 'b' so
				//it will jump to while in the interpreter
				//now we can directly put the jump branch at while object
				// new configuration will look like this, first jump object jumps to the next of while which is 'b'
				// a,while,b,while,jump,jump 
				//		   ^---------|   |
				// ^---------------------|

				cast_fbgc_object_as_jumper(jump_obj)->content = TOP_LL(op);
			}
			else if(TOP_LL(op) != NULL && TOP_LL(op)->type == FOR_BEGIN){
				cprintf(111,"korkmustum gercekten amcik\n");
				assert(0);
			}
			else
				cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;

			push_front_fbgc_ll_object(op,jump_obj);
			iter_prev->next = iter->next;	
			push_front_fbgc_ll_object(op,iter);	
			//iter_prev->next = iter->next;	
			break;
		}
		case FOR:
		{
			gm_error = gm_seek_left(&gm,iter);	


			cast_fbgc_object_as_jumper(iter)->content = iter_prev;

			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);
			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;
			push_front_fbgc_ll_object(op,jump_obj);
			iter_prev->next = iter->next;	
			push_front_fbgc_ll_object(op,iter);				
			break;
		}			
		case BREAK:
		{
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);

			struct fbgc_object * op_top = TOP_LL(op);

			for(;;){
				assert(op_top != NULL);
				if(op_top->type == WHILE_BEGIN || op_top->type == FOR_BEGIN){
					cast_fbgc_object_as_jumper(iter)->content = op_top;
					break;
				}

				op_top = op_top->next;
			}
			break;				
		}
		case CONT:
		{
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);

			struct fbgc_object * op_top = TOP_LL(op);

			for(;;){

				assert(op_top != NULL);
				if(op_top->type == WHILE_BEGIN || op_top->type == FOR_BEGIN){
					//here we assumed there will be always jump object after while in the stack
					op_top = op_top->next;
					
					cast_fbgc_object_as_jumper(iter)->content = op_top;
					break;
				}
				op_top = op_top->next;
			}
			break;		
		} 		
		case IF:
		case RETURN:
		case NEWLINE:
		case LPARA:
		case RPARA:
		case LBRACK:
		case RBRACK:
		case LBRACE:
		case RBRACE:
		case UMINUS:
		case UPLUS:
		case TILDE:
		case COLON:
		case COMMA:
		case DOT:
		case SEMICOLON:
		case R_SHIFT:
		case L_SHIFT:
		case STARSTAR:
		case SLASHSLASH:
		case PLUS:
		case MINUS:
		case STAR:
		case SLASH:
		case CARET:
		case PERCENT:
		case LO_EQ:
		case GR_EQ:
		case EQ_EQ:
		case NOT_EQ:
		case LOWER:
		case GREATER:
		case PIPE:
		case AMPERSAND:
		case EXCLAMATION:
		{
			//take the op object from main list and connect previous one to the next one 
			//[H]->[2]->[+]->[3] => [H]->[2]->[3], now iter holds the operator, iter->next is [3] but we will change that too 
			//     p^	i^					
			iter_prev->next = iter->next;
			//struct fbgc_object * iterp2 = iter_prev;
			
			while( !is_empty_fbgc_ll_object(op) && compare_operators(get_fbgc_object_type(TOP_LL(op)),iter->type) ){

				gm_error = gm_seek_right(&gm,TOP_LL(op));
				
				if(is_pushable_in_main(get_fbgc_object_type(TOP_LL(op)))){		
					//Insert top op to the list  
					iter_prev->next = TOP_LL(op);
					//Pop top from stack
					POP_LL(op);
					//connect list again
					iter_prev->next->next = iter->next; 
					
					//make the iter_prev proper
					iter_prev = iter_prev->next;
					
				}
				else{
					//not pushables in main, like paranthesis 
					if(iter->type == RPARA && get_fbgc_object_type(TOP_LL(op)) == LPARA){
						//balanced paranthesis now search other objects in the stack top is lpara but what is the previous ? 

						#ifdef PARSER_DEBUG
						cprintf(110,"=== Rpara and Lpara ===\n");
						//cprintf(110,"iterp2 : "); print_fbgc_object(iterp2); cprintf(111,"\n");
						#endif

						//popping the lpara
						delete_front_fbgc_ll_object(op);
						gm_error = gm_seek_left(&gm,iter);

						#ifdef PARSER_DEBUG
						cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
						#endif
						//if(TOP_LL(op) == NULL) break;
						//cprintf(111,"After handling paranthesis, operator stack top :");
						//cprintf(011,"Op Top :[%s], Iter_prev:[%s]\n",object_name_array[get_fbgc_object_type(TOP_LL(op))],
						//	object_name_array[iter_prev->type]);


						iter_prev = handle_before_paranthesis(iter_prev,op,&gm);
						//iter_prev = iterp2->next;

						#ifdef PARSER_DEBUG
						cprintf(010,"##############After handle paranthesis###############\n");
						print_fbgc_ll_object(head_obj,"M");
						print_fbgc_ll_object(op,"O");
						cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
						cprintf(010,"###############$$$$################\n");
						#endif

						break;
					}
					else if(iter->type == RBRACK && get_fbgc_object_type(TOP_LL(op)) == LBRACK){
						#ifdef PARSER_DEBUG
						cprintf(110,"=== RBRACK and LBRACK ===\n");
						#endif

						delete_front_fbgc_ll_object(op);
						gm_error = gm_seek_left(&gm,iter);

						#ifdef PARSER_DEBUG
						if(gm.top != BUILD_MATRIX && gm.top != MONATRIX) 
							cprintf(100,"\n\n ERROR BRACK \n\n");
						#endif

						iter_prev = handle_before_brackets(iter_prev,op,&gm);
						break;
					}
					else if(iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(op)) == LBRACK){
						#ifdef PARSER_DEBUG
						cprintf(110,"\tBRACK AND SEMICOLON\n");
						#endif


						if(iter_prev->type == COMMA) iter_prev->type = ROW; 
						else if(iter_prev->type != ROW){
							struct fbgc_object * ito = derive_from_new_int_object(ROW,1);
							ito->next = iter_prev->next;
							iter_prev->next = ito;
							iter_prev = ito;
						}
						


						/*struct fbgc_object * rowo = new_fbgc_object(ROW);
						rowo->next = iter_prev->next;
						iter_prev->next = rowo;
						iter_prev = rowo;*/
						break;
					}
					else if(iter->type == PIPE && get_fbgc_object_type(TOP_LL(op)) == LEN){
							
						iter->next = iter_prev->next;
						//eat the pipe insert the len
						iter_prev->next = TOP_LL(op);
						//Pop top from stack
						POP_LL(op);
						iter_prev = iter_prev->next;
						iter_prev->next = iter->next;
						iter->type = RPARA;//cheat the following part, change this it is very bad..

						break;
					}
					else {
						cprintf(111,"Syntax Error :");
						cprintf(011,"Op Top :[%s], Iter:[%s]\n",object_name_array[get_fbgc_object_type(TOP_LL(op))],object_name_array[iter->type]);
						assert(0);
						//goto END_OF_THE_PARSER;
					}

				}

					#ifdef PARSER_DEBUG
						cprintf(010,"##############WHile###############\n");
						print_fbgc_ll_object(head_obj,"M");
						print_fbgc_ll_object(op,"O");
						cprintf(101,"[GM]:{Top:%s} Flag{0x%X} \n",object_name_array[gm.top],gm.flag);
						cprintf(010,"###############$$$$################\n");
					#endif

				//if(!gm_error) goto END_OF_THE_PARSER;
			}

			if(iter->type != RPARA)	gm_error = gm_seek_left(&gm,iter);
			
			if(iter->type == RPARA || iter->type == RBRACK|| 
				iter->type == SEMICOLON|| iter->type == NEWLINE || 
				iter->type == ROW ){
			}
			else if(iter->type == COMMA){

				assert(TOP_LL(op) != NULL);

				/*if(get_fbgc_object_type(TOP_LL(op)) == LPARA){
					cast_fbgc_object_as_int(TOP_LL(op))->content++;
				}*/

				if(get_fbgc_object_type(TOP_LL(op)) == COMMA){
					cast_fbgc_object_as_int(TOP_LL(op))->content++;
					//free_fbgc_object(iter);
				}
				else{
					//now delete iterator,make new int object and hold comma as an int object but changing its type!
					//free_fbgc_object(iter);
					//2 elements for 1 comma
					push_front_fbgc_ll_object(op,derive_from_new_int_object(COMMA,2));
				}
			}
			else {
				push_front_fbgc_ll_object(op,iter);
				
			}
			break;
		}
		case ASSIGN: 		
		case  R_SHIFT_ASSIGN:
		case  L_SHIFT_ASSIGN:
		case  STARSTAR_ASSIGN:
		case  SLASHSLASH_ASSIGN:
		case  PLUS_ASSIGN:
		case  MINUS_ASSIGN:
		case  STAR_ASSIGN:
		case  SLASH_ASSIGN:
		case  CARET_ASSIGN:
		case  PERCENT_ASSIGN:
		{
			gm_error = gm_seek_left(&gm,iter);
			
			assert(TOP_LL(op)->type == IDENTIFIER );

			if(TOP_LL(op)->next != NULL && TOP_LL(op)->next->type == ASSIGN) set_id_flag_PUSH_ITSELF(TOP_LL(op));
			#ifdef PARSER_DEBUG
			struct fbgc_object * pc = TOP_LL(op);
            if(is_id_flag_GLOBAL(pc) ) cprintf(011,"%s{G<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else if(is_id_flag_LOCAL(pc) ) cprintf(011,"%s{L<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else if(is_id_flag_MEMBER(pc) ) cprintf(011,"%s{M<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else if(is_id_flag_SUBSCRIPT(pc) ) cprintf(011,"%s{S<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else{
                cprintf(111,"Undefined ID!\n"); 
            }
            cprintf(111,"\n");     				
			#endif

			TOP_LL(op)->type = iter->type;
			iter_prev->next = iter->next;
			break;
		}


		default:
		{
			#ifdef PARSER_DEBUG
			cprintf(100,"Error else in parser incoming obj %s|%d\n",object_name_array[iter->type],iter->type);
			#endif
			goto END_OF_THE_PARSER;	
		}
 	}
	iter = iter_prev->next;
	
	#ifdef PARSER_DEBUG
		print_fbgc_ll_object(head_obj,"M");
		print_fbgc_ll_object(op,"O");
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
	cprintf(111,"\n\n\n\n");
	#endif

	if(!is_empty_fbgc_ll_object(op) ){
		cprintf(100,"Error occured in parser.. See operator stack\n");
		print_fbgc_ll_object(op,"O");
	}


	return gm_error;

}
