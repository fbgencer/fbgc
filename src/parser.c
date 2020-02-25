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
	RIGHT_ASSOC | 0,//IF
	4,//RETURN
	2,//NEWLINE
	RIGHT_ASSOC | 10,//LPARA
	6,//RPARA
	RIGHT_ASSOC | 10,//LBRACK
	6,//RBRACK
	RIGHT_ASSOC | 10,//LBRACE
	6,//RBRACE
	RIGHT_ASSOC | 31,//COMMA // it was 20 now we changed to make bigger than equal sign for x = 1,2,3 cases
	52,//DOT
	3,//SEMICOLON
	32,//COLON	
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
	34,//PIPE
	36,//AMPERSAND
	46,//EXCLAMATION
	RIGHT_ASSOC | 46,//TILDE	
	RIGHT_ASSOC | 48,//UPLUS
	RIGHT_ASSOC | 48,//UMINUS
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
	47,//LEN
};

uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
	// precedence outputs
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	uint8_t result = 0;
	#ifdef PARSER_DEBUG
	cprintf(010,"Object type comparison StackTop:[%s]>=Obj[%s] ",object_name_array[stack_top],object_name_array[obj_type]);
	#endif

	if(obj_type == NEWLINE && stack_top == COMMA) result = 0;
	else if(obj_type == LPARA || obj_type == LBRACK) result =  0;
	//else if(obj_type == DOT && stack_top == IDENTIFIER) result = 0;
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
	cprintf(101,": %d\n",result && 1);
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

struct fbgc_object * handle_before_paranthesis(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm,uint8_t state){
	
	uint8_t gm_error = 1;


	fbgc_token top_type = get_fbgc_object_type(TOP_LL(op));

	//	#ifdef PARSER_DEBUG
	//cprintf(111,"In %s op top [%s]\n",__FUNCTION__,object_name_array[top_type]);
	//#endif

	//know that iter_prev is iter_prev_prev!!

 
	if(top_type == IDENTIFIER || top_type == CFUN){
		#ifdef PARSER_DEBUG
		cprintf(110,"Operator stack top ID or CFUN, this is a function call template!\n");
		print_fbgc_object(iter_prev);
		
		#endif

		//state == 2
		if(iter_prev->type == COMMA) iter_prev->type = INT; 
		//state == 0
		else{
			struct fbgc_object * ito = new_fbgc_int_object(state == 1); //nuple
			ito->next = iter_prev->next;
			iter_prev->next = ito;
			iter_prev = ito;
		}

		gm_seek_right(gm,TOP_LL(op));
		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		iter_prev->next = TOP_LL(op);
		POP_LL(op);
		
		char put_pop_top = 0;

		if(iter_prev->next->type != CFUN && is_id_flag_MEMBER(iter_prev->next) ){
			//get the top object 



			//method call
			iter_prev->next->type = METHOD_CALL;
			iter_prev = iter_prev->next;
			put_pop_top = 1;
		}else{
			//function call

			iter_prev = iter_prev->next;
			iter_prev->next = new_fbgc_object(FUN_CALL);
			iter_prev = iter_prev->next;
		}

		if(!is_empty_fbgc_ll_object(op) )
			put_pop_top = 1;

		put_pop_top = 0;


		if(put_pop_top){
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
		cprintf(110,"Operator stack top FUN, this is an fun template!\n");
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

		
		//struct fbgc_object * i = new_fbgc_int_object(-1);
		struct fbgc_object * i = new_fbgc_object(FOR_BEGIN);
		//
		i->next = d->next;
		d->next = i;
		cast_fbgc_object_as_jumper(TOP_LL(op))->content = i;

	}	
	else{

		//## It is a bug that we cannot know whether it is tuple or just an entry in paranthesis
		if(state != 1 || iter_prev->type == COMMA){
			
			if(iter_prev->type == COMMA){
				iter_prev->type = BUILD_TUPLE;
			} 
			else {
				//Actually this is always zero ? Check this later
				struct fbgc_object * ito = 
				derive_from_new_int_object(BUILD_TUPLE,0);
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

struct fbgc_object * handle_before_brackets(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm,uint8_t state){
	#ifdef PARSER_DEBUG
	cprintf(111,">>%s<<\n",__FUNCTION__);
	cprintf(111,"iter_prev %s\n",object_name_array[iter_prev->type]);
	#endif
	uint8_t gm_error = 1;

	if(iter_prev->type == COMMA) iter_prev->type = INT; 
	else {
		struct fbgc_object * ito = new_fbgc_int_object(state == 1);
		ito->next = iter_prev->next;
		iter_prev->next = ito;
		iter_prev = ito;
	}

	#ifdef PARSER_DEBUG
	if(TOP_LL(op) != NULL){
		cprintf(110,"############## IN bracket closing ###############\n");
		print_fbgc_ll_object(op,"O");
		cprintf(101,"[GM]:{%s}\n",gm_name_array[gm->top]);
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

uint8_t parser(struct fbgc_object ** field_obj, FILE * input_file){
	#ifdef PARSER_DEBUG
	cprintf(111,"==========[PARSER]==========\n");
	#endif

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * head_obj =  cast_fbgc_object_as_field(*field_obj)->head;
	struct fbgc_object * op = new_fbgc_ll_object();
	struct fbgc_grammar gm = {.top =  GM_NEWLINE};
	
	struct fbgc_object * current_scope = *field_obj;

	uint8_t gm_error = 1;

	uint8_t error_code = _FBGC_NO_ERROR;

	char line[1000] = {0};

	
	
	
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head; //note that iter_prev->next is iter, always this is the case!
	

	int line_no = 0;


	for(int i = 0; fbgc_error(error_code,line_no); i++){
		

		if(iter == head->tail){
			if(fbgc_getline_from_file(line, sizeof(line), input_file)){
				++line_no;

				#ifdef PARSER_DEBUG
				cprintf(111,"---Line:[%d]:{%s}\n",line_no,line);
				#endif

				if(line[0] == '#' || line[0] == '\0' || line[0] == '\n') continue; //fast passing the comment
				
				head->tail->next = iter_prev;
				regex_lexer(field_obj,line);
				
				iter = iter_prev->next;
				
			}
			else break;
		}
	
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
 		case STRING:{
			if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
			}
 			iter_prev = iter;
 			break;
 		}
 		case IDENTIFIER:{
			if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
			}
			#ifdef PARSER_DEBUG
			cprintf(111,"current_scope :[%s]\n",object_name_array[current_scope->type]);
			#endif

			struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,cast_fbgc_object_as_id_opcode(iter)->loc);

			#ifdef PARSER_DEBUG
			cprintf(100,"symbol name:["); print_fbgc_object(cstr_obj);
			cprintf(100,"]\n");
			#endif
			
			struct fbgc_object * cf = NULL;

			if(!is_empty_fbgc_ll_object(op) && TOP_LL(op)->type == DOT){
				//so this is just a member selection
				POP_LL(op); //pop the dot object
				//now get the name of instance


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
					if(temp_id->name == cstr_obj) {
						where = i;
						break;
					} 
				}

				if(where == -1) {
					#ifdef PARSER_DEBUG
					cprintf(111,"Creating variable inside global field\n");
					#endif
					struct fbgc_identifier id;		
					id.name = cstr_obj; id.content = NULL;
					local_array = push_back_fbgc_array_object(local_array,&id);
					where = size_fbgc_array_object(local_array)-1;
					cast_fbgc_object_as_field(current_scope)->locals = local_array;
					
					#ifdef PARSER_DEBUG
					cprintf(111,"Location of this symbol[%d]\n",where);
					#endif
				}
				else{
					#ifdef PARSER_DEBUG
					cprintf(111,"Symbol already defined @[%d]\n",where);
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
		case END:{
			gm_error = gm_seek_left(&gm,iter);
			#ifdef PARSER_DEBUG
			cprintf(010,"############## BEFORE END ###############\n");
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op,"O");
			cprintf(101,"[GM]:{Top:%s}\n",gm_name_array[gm.top]);
			cprintf(010,"###############$$$$################\n");
			#endif

			iter_prev->next = iter->next;
			//iter = iter_prev;

			//iter_prev = iter_prev->next;
			
			#ifdef PARSER_DEBUG
			print_fbgc_ll_object(head_obj,"??");	
			#endif

			if(TOP_LL(op)->type == IF){
				//now insert if in its place,

				struct fbgc_object * if_obj = TOP_LL(op);
				POP_LL(op);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;
				cast_fbgc_object_as_jumper(if_obj)->content = iter_prev; //now assign where to jump

				#ifdef PARSER_DEBUG
				print_fbgc_ll_object(head_obj,"IF_main");
				#endif	
			}
			else if(TOP_LL(op)->type == ELIF){
				//now insert if in its place,
				struct fbgc_object * if_obj = TOP_LL(op);
				POP_LL(op);

				if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
				cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;

				cast_fbgc_object_as_jumper(if_obj)->content = iter_prev;
			}
			else if(TOP_LL(op)->type == WHILE){
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
			else if(TOP_LL(op)->type == FOR){
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
				iter_prev->next = iter->next;
				
			}
			//else assert(0);



			while(TOP_LL(op) != NULL && TOP_LL(op)->type == JUMP){
				struct fbgc_object * jump_obj = TOP_LL(op);
				POP_LL(op);

				jump_obj->next = cast_fbgc_object_as_jumper(jump_obj)->content->next;
				cast_fbgc_object_as_jumper(jump_obj)->content->next = jump_obj;

				cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;	
			}

			//iter_prev->next = iter->next;


			#ifdef PARSER_DEBUG
			cprintf(010,"############## AFTER END ###############\n");
			print_fbgc_ll_object(head_obj,"M");
			print_fbgc_ll_object(op,"O");
			cprintf(101,"[GM]:{Top:%s} \n",gm_name_array[gm.top]);
			cprintf(010,"##############$$$$################\n");
			#endif

			break;			
		}
		case FUN_MAKE:{
			gm_error = gm_seek_left(&gm,iter);	
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
		case ELIF:{
			//gm_error = gm_seek_right(&gm,TOP_LL(op));
			//gm_error = gm_seek_left(&gm,iter);	

			if(is_empty_fbgc_ll_object(op) && (TOP_LL(op)->type != IF || TOP_LL(op)->type != ELIF)){
				//We raise this error because we couldn't find any if before so it's not just a syntax error.
				//## Give proper error message
				error_code = _FBGC_SYNTAX_ERROR;
				goto PARSER_ERROR_LABEL;
			}

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
		case ELSE:{

			//gm_error = gm_seek_right(&gm,TOP_LL(op));
			//gm_error = gm_seek_left(&gm,iter);	

			if(is_empty_fbgc_ll_object(op) || (TOP_LL(op)->type != IF && TOP_LL(op)->type != ELIF) ){
				cprintf(100,"Top type = %s\n",object_name_array[TOP_LL(op)->type]);
				error_code = _FBGC_SYNTAX_ERROR;
				goto PARSER_ERROR_LABEL;
			}

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
		case WHILE:{
			gm_error = gm_seek_left(&gm,iter);	



			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);
			if(TOP_LL(op) != NULL && TOP_LL(op)->type == WHILE){
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
			else if(TOP_LL(op) != NULL && TOP_LL(op)->type == FOR){
				cprintf(111,"Why do we have this?\n");
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
		case FOR:{
			gm_error = gm_seek_left(&gm,iter);	


			cast_fbgc_object_as_jumper(iter)->content = iter_prev;

			struct fbgc_object * jump_obj = new_fbgc_jumper_object(JUMP);
			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;
			push_front_fbgc_ll_object(op,jump_obj);
			iter_prev->next = iter->next;	
			push_front_fbgc_ll_object(op,iter);				
			break;
		}			
		case BREAK:{
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);

			struct fbgc_object * op_top = TOP_LL(op);

			for(;;){
				assert(op_top != NULL);
				if(op_top->type == WHILE || op_top->type == FOR){
					cast_fbgc_object_as_jumper(iter)->content = op_top;
					break;
				}

				op_top = op_top->next;
			}
			break;				
		}
		case CONT:{
			iter_prev = iter;
			gm_error = gm_seek_left(&gm,iter);

			struct fbgc_object * op_top = TOP_LL(op);

			for(;;){

				assert(op_top != NULL);
				if(op_top->type == WHILE || op_top->type == FOR){
					//here we assumed there will be always jump object after while in the stack
					op_top = op_top->next;
					
					cast_fbgc_object_as_jumper(iter)->content = op_top;
					break;
				}
				op_top = op_top->next;
			}
			break;		
		}
		case RPARA:
		case RBRACK:
		case RBRACE:{


			//check this part again
			uint8_t which_para = iter->type-1;

			uint8_t element_number = 0;
			//determine nuple,monuple or tuple

			//cprintf(111,"gm top %s\n",gm_name_array[gm.top]);
			//buna bir cozum bulalim gm bilmeden bulmak mumkun mu?
			// && get_fbgc_object_type(TOP_LL(op)) != which_para 
			element_number = (gm.top != GM_LPARA && gm.top != GM_LBRACK) + (get_fbgc_object_type(TOP_LL(op)) == COMMA);
			
			#ifdef PARSER_DEBUG
			cprintf(111,"PARA)Element_number %d\n",element_number);
			#endif
			iter_prev->next = iter->next;

			while(get_fbgc_object_type(TOP_LL(op)) != which_para){

				
				if( (error_code = gm_seek_right(&gm,TOP_LL(op))) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
				}
				//Insert top op to the list  
				iter_prev->next = TOP_LL(op);
				//Pop top from stack
				POP_LL(op);
				//connect list again
				iter_prev->next->next = iter->next; 
				
				//make the iter_prev proper
				iter_prev = iter_prev->next;


				if(is_empty_fbgc_ll_object(op)){
					cprintf(100,"Para match error\n");
					return 0;
				}
			}

			//so everything is ok
			//pop the left paranthesis
			delete_front_fbgc_ll_object(op);
			if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
			}
			//gm_error = gm_seek_left(&gm,iter);
			if(iter->type == RPARA){
				//balanced paranthesis now search other objects in the stack top is lpara but what is the previous ? 
				iter_prev = handle_before_paranthesis(iter_prev,op,&gm,element_number);
			}
			else if(iter->type == RBRACK){
				iter_prev = handle_before_brackets(iter_prev,op,&gm,element_number);
			}			
			break;

		}
		/*case LOAD:{
			if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
			}

				

			assert(0);
			break;
		}*/
		case LOAD:
		case IF:
		case RETURN:
		case NEWLINE:
		case LPARA:
		//case RPARA:
		case LBRACK:
		//case RBRACK:
		case LBRACE:
		//case RBRACE:
		case COMMA:
		case DOT:
		case SEMICOLON:
		case COLON:
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
		case TILDE:		
		case UPLUS:
		case UMINUS:{

			//take the op object from main list and connect previous one to the next one 
			//[H]->[2]->[+]->[3] => [H]->[2]->[3], now iter holds the operator, iter->next is [3] but we will change that too 
			//     p^	i^					
			iter_prev->next = iter->next;
			//struct fbgc_object * iterp2 = iter_prev;

			
			while( !is_empty_fbgc_ll_object(op) && compare_operators(get_fbgc_object_type(TOP_LL(op)),iter->type) ){

				if( (error_code = gm_seek_right(&gm,TOP_LL(op))) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
				}

				
				if(is_pushable_in_main(get_fbgc_object_type(TOP_LL(op)))){		

					//gm_error = gm_seek_left(&gm,iter);
					
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

					#ifdef PARSER_DEBUG
						cprintf(010,"Cannot push in main list\n");
					#endif

					if(iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(op)) == LBRACK){
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
						cprintf(010,"#############< While >###############\n");
						print_fbgc_ll_object(head_obj,"M");
						print_fbgc_ll_object(op,"O");
						cprintf(101,"[GM]:{%s}\n",gm_name_array[gm.top]);
						cprintf(010,"###############$$$$################\n");
					#endif

				//if(!gm_error) goto END_OF_THE_PARSER;
			}

			//if(iter->type != RPARA)	
			//gm_error = gm_seek_left(&gm,iter);
			if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
			}
			//cprintf(011,"iter prev %s, iter_prev-next %s, iter %s iter-next %s\n",
			//	object_name_array[iter_prev->type],object_name_array[iter_prev->next->type],
			//	object_name_array[iter->type],object_name_array[iter->next->type]);

			if(iter->type == RPARA || iter->type == RBRACK|| iter->type == SEMICOLON ||  iter->type == ROW ){
				;
				//PASS EMPTY, BUT IT lookS BAD
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
			else if(iter->type == NEWLINE){
				#ifdef PARSER_DEBUG
					cprintf(010,"Newline OUT\n");
				#endif

				if(is_empty_fbgc_ll_object(op) ){
					gm.top = GM_NEWLINE;
					break;
				}

				if(TOP_LL(op)->type == FOR){
					#ifdef PARSER_DEBUG
					cprintf(111,"Newline will handle FOR !\n");
					#endif
					
					if(cast_fbgc_object_as_jumper(TOP_LL(op))->content->type != FOR_BEGIN){
						handle_before_paranthesis(iter_prev,op,&gm,1);
					}
					
					/*if(cast_fbgc_object_as_jumper(TOP_LL(op))->content->type != INT ||
					 cast_fbgc_object_as_int(cast_fbgc_object_as_jumper(TOP_LL(op))->content)->content != -1){
						handle_before_paranthesis(iter_prev,op,&gm,1);
					}*/
				}
				else if(TOP_LL(op)->type == IF || TOP_LL(op)->type == ELIF || TOP_LL(op)->type == WHILE){
					#ifdef PARSER_DEBUG 
					cprintf(111,"Newline will handle IF|ELIF !\n");
					#endif
					//cast_fbgc_object_as_jumper(TOP_LL(op))->content

					if(cast_fbgc_object_as_jumper(TOP_LL(op))->content == NULL){
						handle_before_paranthesis(iter_prev,op,&gm,1);
					}
				}


				if(TOP_LL(op)->type == COMMA && TOP_LL(op)->next->type != LPARA){
					
					gm_error = gm_seek_right(&gm,TOP_LL(op));
					
					iter_prev->next = TOP_LL(op);
					//Pop top from stack
					POP_LL(op);
					//connect list again
					iter_prev->next->next = iter->next; 
					//make the iter_prev proper
					iter_prev = iter_prev->next;


					gm_error = gm_seek_left(&gm,iter);
					//bu kisim duzelmeli.
					iter_prev = handle_before_paranthesis(iter_prev,op,&gm,2);
					iter_prev->next = iter;
					iter = iter_prev;
					cprintf(111,"pr %s it %s\n",object_name_array[iter_prev->next->type],object_name_array[iter->next->type]);
					//iter = iter_prev;
					gm.top = GM_EXPRESSION;
				}
				else{
					//BU HATA CIKARIYOR DEGISTIR
					gm.top = GM_NEWLINE;		
				}

			}
			else {
				push_front_fbgc_ll_object(op,iter);
				
			}
			break;
		}
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
		case  PERCENT_ASSIGN:{

			if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){
					goto PARSER_ERROR_LABEL;
			}
				
			if(TOP_LL(op)->type != IDENTIFIER){
				error_code = _FBGC_SYNTAX_ERROR;
				goto PARSER_ERROR_LABEL;
			}
			
			//XXX remove this push itself thing
			//fbgc_assert(TOP_LL(op)->type == IDENTIFIER ,"Assignment to a non-identifier object, object type:%s\n",object_name_array[TOP_LL(op)->type]);
			//cprintf(111,"top->next = %s\n",object_name_array[TOP_LL(op)->next->type]);
			if(TOP_LL(op)->next != NULL && 
				(TOP_LL(op)->next->type == ASSIGN || TOP_LL(op)->next->type == LPARA || TOP_LL(op)->next->type == COMMA) ) {
				#ifdef PARSER_DEBUG
				cprintf(100,"Opening flag push itself\n");
				#endif

				if(TOP_LL(op)->next->next == NULL || TOP_LL(op)->next->next->type != FOR)
					set_id_flag_PUSH_ITSELF(TOP_LL(op));

				//this creates error when we use "for(i=smth)... end", it pushes iter object
			}
			#ifdef PARSER_DEBUG 
			struct fbgc_object * pc = TOP_LL(op);
            if(is_id_flag_GLOBAL(pc) ) cprintf(011,"%s{G<%d>}","GlobalID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else if(is_id_flag_LOCAL(pc) ) cprintf(011,"%s{L<%d>}","LocalID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else if(is_id_flag_MEMBER(pc) ) cprintf(011,"%s{M<%d>}","MemberID",cast_fbgc_object_as_id_opcode(pc)->loc);
            else if(is_id_flag_SUBSCRIPT(pc) ) cprintf(011,"%s{S<%d>}","[]ID",cast_fbgc_object_as_id_opcode(pc)->loc);
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
			goto PARSER_ERROR_LABEL;
		}
 	}
	iter = iter_prev->next;
	
	#ifdef PARSER_DEBUG
		print_fbgc_ll_object(head_obj,"M");
		print_fbgc_ll_object(op,"O");
		cprintf(101,"[GM]:{%s}\n",gm_name_array[gm.top]);
	#endif
	}


	//make the linked list connection proper
	head->tail->next = iter_prev;

	#ifdef PARSER_DEBUG
	cprintf(111,"Parser finished job normally.\n");
	cprintf(111,"Symbols :");
	//:>print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	cprintf(111,"Field Locals :");
	print_field_object_locals(*field_obj);
	cprintf(111,"\n");

	cprintf(111,"\n\n\n\n");
	#endif

	if(!is_empty_fbgc_ll_object(op) ){
		cprintf(100,"Error occured in parser.. See operator stack\n");
		print_fbgc_ll_object(op,"O");
		assert(0);
	}

	return 1;


	PARSER_ERROR_LABEL:
	fbgc_error(error_code,line_no);
		cprintf(100,"%s\n",line);
	return 0;

} 	
