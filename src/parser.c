#include "fbgc.h"

//Close everything if we are not gonna debug
#ifndef PARSER_DEBUG
#define _info(s,...)
#define _info_green(s,...)
#define _warning(s,...)
#define _print(s,...)
#define _cprint(c,s,...)
#define _debug(s,...)
#define _print_object(s,obj)
#define _println_object(s,obj)
#define _obj2str(obj)
#define _gm2str(gm)
#endif


#ifdef PARSER_DEBUG
	#define _print_main_list()(print_fbgc_ll_object(head_obj,"Main"))
	#define _print_op_stack()(print_fbgc_ll_object(op,"O"))
	#define _print_field_locals(x)(print_field_object_locals(x),cprintf(111,"\n"))
#else
	#define _print_main_list()
	#define _print_op_stack()
	#define _print_field_locals(x)
#endif


#define is_pushable_in_main(x)(!is_fbgc_PARA(x) && x != LEN)


#define RIGHT_ASSOC 0b10000000
#define LEFT_ASSOC 0	
const fbgc_token const precedence_table[] =
{
	RIGHT_ASSOC | 0,//IF
	4,//RETURN
	2,//NEWLINE
	RIGHT_ASSOC | 5,//LPARA
	16,//RPARA
	RIGHT_ASSOC | 5,//LBRACK
	16,//RBRACK
	RIGHT_ASSOC | 5,//LBRACE
	16,//RBRACE
	RIGHT_ASSOC | 18,//COMMA // it was 20 now we changed to make bigger than equal sign for x = 1,2,3 cases
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
	RIGHT_ASSOC | 17,//ASSIGN
	RIGHT_ASSOC | 17,//R_SHIFT_ASSIGN
	RIGHT_ASSOC | 17,//L_SHIFT_ASSIGN
	RIGHT_ASSOC | 17,//STARSTAR_ASSIGN
	RIGHT_ASSOC | 17,//SLASHSLASH_ASSIGN
	RIGHT_ASSOC | 17,//PLUS_ASSIGN
	RIGHT_ASSOC | 17,//MINUS_ASSIGN
	RIGHT_ASSOC | 17,//STAR_ASSIGN
	RIGHT_ASSOC | 17,//SLASH_ASSIGN
	RIGHT_ASSOC | 17,//CARET_ASSIGN
	RIGHT_ASSOC | 17,//PERCENT_ASSIGN
	RIGHT_ASSOC | 17,//ASSIGN_SUBSCRIPT
	47,//LEN
};

uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
	


	_info("Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]);
	
	// precedence outputs
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	uint8_t result = 0;
	if(stack_top == IDENTIFIER) 
		result = 1;
	//x=(1,2) requires low precedence!
	else if(obj_type == LBRACK || obj_type == LPARA) result =  0;

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


	_info("%d\n",result&&1);

	return result;
}


void handle_function_args(struct fbgc_object * fun_obj,struct fbgc_object * arg_end){
	//	arg_start is always next pt of fun_obj
	//	arg_start holds parsed arg expression beginning
	//	example: fun(x,y,z) will be parsed x,y,z,3,build_tuple,
	//							  arg_start^      arg_end^	
	//	There could be default assignment situations, we need to handle assignemnt expr and other type of args	
	cast_fbgc_object_as_fun(fun_obj)->no_arg = size_fbgc_tuple_object(cast_fbgc_object_as_fun(fun_obj)->code); 
	fun_obj->next = arg_end->next;

	_info("Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);	
}

struct fbgc_object * handle_before_paranthesis(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	
	_info("==================================\n");
	_info("%s is called\n",__FUNCTION__);
	_info("Stack top:%s\n",_obj2str(TOP_LL(op)));

	uint8_t gm_error = 1;
	

	fbgc_token top_type = get_fbgc_object_type(TOP_LL(op));

	uint8_t	state = 0;
	if(iter_prev->type == COMMA) state = 2;
	else if(gm->top != GM_ATOM) state = 1;

	if(top_type == FUN_CALL){
		
		_println_object("Function call template, iter_prev:",iter_prev);

		//state == 2
		if(state == 2) iter_prev->type = INT; 
		else{
			struct fbgc_object * ito = new_fbgc_int_object(state == 1); //nuple
			ito->next = iter_prev->next;
			iter_prev->next = ito;
			iter_prev = ito;
		}

		//gm_seek_right(gm,TOP_LL(op));
		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		iter_prev->next = TOP_LL(op);
		POP_LL(op);
		iter_prev = iter_prev->next;
		
		char put_pop_top = 0;

		if(!is_empty_fbgc_ll_object(op) )
			put_pop_top = 1;

		put_pop_top = 0;

		if(put_pop_top){
			iter_prev->next = new_fbgc_object(POP_TOP);
			iter_prev = iter_prev->next;			
		}

		iter_prev->next = iter;
	}
	else if(top_type == FUN_MAKE){

		_print("Stack top is FUN_MAKE, function make template!\n");
		if(cast_fbgc_object_as_fun(cast_fbgc_object_as_jumper(TOP_LL(op))->content)->no_arg != -1)
			return iter_prev;
		//fun_make content holds function object, parse the arguments
		gm_seek_right(gm,TOP_LL(op));

		struct fbgc_object * top = TOP_LL(op);
		struct fbgc_object * fun_obj =  cast_fbgc_object_as_jumper(TOP_LL(op))->content; 

		_println_object("Function Arg Start:",fun_obj->next);
		_println_object("Function Arg End:",iter_prev);

		handle_function_args(fun_obj,iter_prev);
		iter_prev = fun_obj;
	}
	else if(top_type == FOR){

		if(cast_fbgc_object_as_jumper(TOP_LL(op))->content->type == FOR_BEGIN)
			return iter_prev;
		
		gm_seek_right(gm,TOP_LL(op));

		_print("Stack top is FOR, for loop template!\n");
		_print("For loop content:%s\n",_obj2str(cast_fbgc_object_as_jumper(TOP_LL(op))->content));
		_print("Iterator previous:%s\n",_obj2str(iter_prev));
		
		if(iter_prev->type != ASSIGN){
			//This makes sure we have for i = x type of loop definition
			return NULL;
		}

		struct fbgc_object * d = cast_fbgc_object_as_jumper(TOP_LL(op))->content;
		while(d->next != iter_prev)
			d = d->next;
		
		struct fbgc_object * fb = new_fbgc_object(FOR_BEGIN);
		fb->next = d->next;
		d->next = fb;
		cast_fbgc_object_as_jumper(TOP_LL(op))->content = fb;

	}	
	else if( (top_type == IF || top_type == ELIF || top_type == WHILE) && cast_fbgc_object_as_jumper(TOP_LL(op))->content == NULL){
		_print("Stack top is %s, conditional template!\n",_obj2str(TOP_LL(op)));
		gm_seek_right(gm,TOP_LL(op));
		cast_fbgc_object_as_jumper(TOP_LL(op))->content = iter_prev;
		_warning("If shows %s\n",_obj2str(iter_prev->next));	
	}
	else if(iter_prev->type == COMMA || gm->top == GM_ATOM){
		
		if(state == 2){
			_debug("State number is 2, changing COMMA to BUILD_TUPLE\n");
			iter_prev->type = BUILD_TUPLE;
		} 
		else if(state == 0){
			_debug("State number is 0, creating new object BUILD_TUPLE with 0 size\n");
			struct fbgc_object * ito = derive_from_new_int_object(BUILD_TUPLE,0);
			ito->next = iter_prev->next;
			iter_prev->next = ito;
			iter_prev = ito;
		}
		//If nothing than it's a tuple!

	}
	/*else{
		
		_println_object("iter prev :",iter_prev);
		_print_op_stack();
		assert(0);
	}*/

	_info("==================================\n");

	return iter_prev;

}

struct fbgc_object * handle_before_brackets(struct fbgc_object * iter_prev,struct fbgc_object * op, struct fbgc_grammar * gm){
	
	_info("==================================\n");
	_info("%s is called\n",__FUNCTION__);
	_info("Stack top:%s\n",_obj2str(TOP_LL(op)));


	uint8_t gm_error = 1;
	uint8_t	state = 0;
	if(iter_prev->type == COMMA) state = 2;
	else if(gm->top != GM_ATOM) state = 1;

	if(state == 2) iter_prev->type = INT; 
	else {
		struct fbgc_object * ito = new_fbgc_int_object(state == 1);
		ito->next = iter_prev->next;
		iter_prev->next = ito;
		iter_prev = ito;
	}
	
	if(is_fbgc_ASSIGNMENT_OPERATOR(iter_prev->next->type)) return iter_prev;
	
	if(TOP_LL(op)->type == IDENTIFIER || TOP_LL(op)->type == LOAD_SUBSCRIPT){	
		_info("Operator stack top load_local or global, this is a subscript call template!\n");

		struct fbgc_object * iter = iter_prev->next;
		//Insert top op to the list  
		iter_prev->next = TOP_LL(op);
		POP_LL(op);
		iter_prev = iter_prev->next;
		iter_prev->next = iter;
	}
	else{

		iter_prev->type = BUILD_MATRIX;
	}
	
	_info("==================================\n");
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

				if(line[0] == '#' || line[0] == '\0' || line[0] == '\n') {
					--i;
					continue; //fast passing the comment
				}
				_debug("Processed line[%d]:{%s}\n",line_no,line);
				
				head->tail->next = iter_prev;
				regex_lexer(field_obj,line);
				iter = iter_prev->next;
			}
			else break;
		}
	

	_cprint(010,"----------[%d]{%s:",i,_obj2str(iter)); _print_object("",iter); _cprint(010,"}----------\n");

	switch(iter->type){
	case LOGIC:
	case INT:
	case DOUBLE:
	case COMPLEX:
	case STRING:
	case TUPLE:
	case MATRIX:{

		if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }	
		iter_prev = iter;
		break;
	}
	case IDENTIFIER:{

		if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }

		_info("Current Scope for identifier:(%s)\n",_obj2str(current_scope));
		struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,get_id_opcode_loc(iter));
		_println_object("Symbol name as a cstring object:",cstr_obj);


			
		struct fbgc_object * cf = NULL;

		if(!is_empty_fbgc_ll_object(op) && TOP_LL(op)->type == DOT){
			//so this is just a member selection
			POP_LL(op); //pop the dot object
			//now get the name of instance
			iter_prev->next = cstr_obj;
			cstr_obj->next = iter->next;
			iter_prev = cstr_obj;
			set_id_flag_MEMBER(iter);
		}
		else if((cf = new_cfun_object_from_str(*field_obj,&cast_fbgc_object_as_cstr(cstr_obj)->content)) != NULL){
			//remove identifier object, put cfun object into list
			//cf->next = iter->next;
			//iter = cf;
			iter_prev->next = cf;
			cf->next = iter->next;
			iter_prev = cf;
			break;	
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
				_info("Creating a variable inside the Global field\n");
				
				struct fbgc_identifier id;		
				id.name = cstr_obj; id.content = NULL;
				local_array = push_back_fbgc_array_object(local_array,&id);
				where = size_fbgc_array_object(local_array)-1;
				cast_fbgc_object_as_field(current_scope)->locals = local_array;
				
				_info("Symbol is created @ [%d]\n",where);

			}
			else{
				_info_green("Symbol is already defined @ [%d]\n",where);
			}
			set_id_flag_GLOBAL(iter);
			cast_fbgc_object_as_id_opcode(iter)->loc = where;
		}

		else if(current_scope->type == FUN){				
			struct fbgc_object * local_tuple = cast_fbgc_object_as_fun(current_scope)->code;
			int where = index_fbgc_tuple_object(local_tuple,cstr_obj);
			set_id_flag_LOCAL(iter);

			if(where == -1) {
				_print("Iter->next type(%s)\n",_obj2str(iter->next));

				//-1 arg means func definition hasnt been done yet! So we are reading arguments
				if(iter->next->type == ASSIGN  || cast_fbgc_object_as_fun(current_scope)->no_arg == -1){
					_info("Id object is an argument of function or a new definition inside the function\n");

					local_tuple = push_back_fbgc_tuple_object(local_tuple,cstr_obj);
					where = size_fbgc_tuple_object(local_tuple)-1;
					cast_fbgc_object_as_fun(current_scope)->code = local_tuple;
					
					_println_object("Fun local tuple:",local_tuple);
				}
				else {
					_info("Searching ID object in global field\n");

					local_tuple = cast_fbgc_object_as_field(*field_obj)->locals;
					struct fbgc_identifier * temp_id; 
					for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
						temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_tuple,i);
						if(temp_id->name == cstr_obj) where = i; 
					}						
					
					//XXX add undefined id error!
					if(where == -1){
						cprintf(100,"%s is not defined before\n",&cast_fbgc_object_as_cstr(cstr_obj)->content);
						assert(0);
					}
					_print("Id object is found @ [%d]",where);
					set_id_flag_GLOBAL(iter);
				}
			}
			
			
			set_id_opcode_loc(iter,where);
		}

		iter_prev->next = iter->next;
		push_front_fbgc_ll_object(op,iter);
		break;
	}
	case END:{
		gm_error = gm_seek_left(&gm,iter);

		_info("Before handling END keyword\n");
		_print_main_list();
		_print_op_stack();

		iter_prev->next = iter->next;
		

		if(TOP_LL(op)->type == IF){
			//now insert if in its place,

			struct fbgc_object * if_obj = TOP_LL(op);
			POP_LL(op);

			if_obj->next = cast_fbgc_object_as_jumper(if_obj)->content->next;
			cast_fbgc_object_as_jumper(if_obj)->content->next = if_obj;
			cast_fbgc_object_as_jumper(if_obj)->content = iter_prev; //now assign where to jump

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

			cast_fbgc_object_as_jumper(jump_obj)->content = cast_fbgc_object_as_jumper(if_obj)->content;	

			jump_obj->next = iter_prev->next;
			iter_prev->next = jump_obj;
			iter_prev = jump_obj;
			
			cast_fbgc_object_as_jumper(if_obj)->content = jump_obj;
		}
		else if(TOP_LL(op)->type == FUN_MAKE){

			assert(iter_prev->type != FUN);  // why do we have this?
			assert(TOP_LL(op)->type == FUN_MAKE);

			struct fbgc_object * fun_obj = cast_fbgc_object_as_jumper(TOP_LL(op))->content;
			

			#ifdef PARSER_DEBUG
			cprintf(111,"top :%s \n",object_name_array[TOP_LL(op)->type]);
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

		while(TOP_LL(op) != NULL && TOP_LL(op)->type == JUMP){
			struct fbgc_object * jump_obj = TOP_LL(op);
			POP_LL(op);

			jump_obj->next = cast_fbgc_object_as_jumper(jump_obj)->content->next;
			cast_fbgc_object_as_jumper(jump_obj)->content->next = jump_obj;

			cast_fbgc_object_as_jumper(jump_obj)->content = iter_prev;	
		}

		//iter_prev->next = iter->next;

		_info("After handling END keyword\n");
		_print_main_list();
		_print_op_stack();

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
	case BREAK:
	case CONT:{
		iter_prev = iter;
		gm_error = gm_seek_left(&gm,iter);

		struct fbgc_object * op_top = TOP_LL(op);

		for(;;){
			assert(!is_empty_fbgc_ll_object(op));

			if(op_top->type == WHILE || op_top->type == FOR){

				if(iter->type == CONT){
					assert(op_top->next->type == JUMP);
					//here we assumed there will be always jump object after while in the stack
					op_top = op_top->next;
				}

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
	//case LBRACE:
	//case RBRACE:
	case COMMA:
	case DOT:
	case SEMICOLON:
	case COLON:
	case RSHIFT:
	case LSHIFT:
	case STARSTAR:
	case SLASHSLASH:
	case PLUS:
	case MINUS:
	case STAR:
	case SLASH:
	case CARET:
	case PERCENT:
	case LOEQ:
	case GREQ:
	case EQEQ:
	case NOTEQ:
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

		
		while( is_not_empty_fbgc_ll_object(op) && compare_operators(get_fbgc_object_type(TOP_LL(op)),iter->type) ){

			if( (error_code = gm_seek_right(&gm,TOP_LL(op))) != _FBGC_NO_ERROR  ){
				goto PARSER_ERROR_LABEL;
			}

			
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

				_warning("Top object is not pushable to main list");
				
				if(iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(op)) == LBRACK){
					
					#ifdef PARSER_DEBUG
					cprintf(110,"\tBRACK AND SEMICOLON\n");
					#endif
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
				else if(get_fbgc_object_type(TOP_LL(op)) == LPARA){
					POP_LL(op);
					break;
				}
				else {
					cprintf(111,"Syntax Error :");
					cprintf(011,"Op Top :[%s], Iter:[%s]\n",object_name_array[get_fbgc_object_type(TOP_LL(op))],object_name_array[iter->type]);
					assert(0);
					//goto END_OF_THE_PARSER;
				}
			}
			_info_green("Checking the operator stack, inside the while loop\n");
			_print_main_list();
			_print_op_stack();
			_print("[GM]:{%s}\n",_gm2str(gm.top));

		}

		if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }

		if(iter->type == LPARA){
			if(iter_prev->type == IDENTIFIER || iter_prev->type == CFUN){
				push_front_fbgc_ll_object(op,new_fbgc_object(FUN_CALL));	
			}
			push_front_fbgc_ll_object(op,iter);
		}
		else if(iter->type == LBRACK){
			if(iter_prev->type == IDENTIFIER){
				push_front_fbgc_ll_object(op,new_fbgc_object(LOAD_SUBSCRIPT));
			}
			push_front_fbgc_ll_object(op,iter);
		}
		else if(iter->type == ROW ){
			;
		}
		else if(iter->type == COMMA){
			if(get_fbgc_object_type(TOP_LL(op)) == COMMA){
				cast_fbgc_object_as_int(TOP_LL(op))->content++;
			}
			else {
				//Also insert a lpara object for tuple entries without paranthesis
				if(get_fbgc_object_type(TOP_LL(op)) != LPARA && get_fbgc_object_type(TOP_LL(op)) != LBRACK){
					push_front_fbgc_ll_object(op,new_fbgc_object(LPARA));
				}

				push_front_fbgc_ll_object(op,derive_from_new_int_object(COMMA,2));
			}
		}
		else if( iter->type == RBRACK){
			
			if(TOP_LL(op)->type == LBRACK){
				delete_front_fbgc_ll_object(op);
			}
			else assert(0);
			iter_prev = handle_before_brackets(iter_prev,op,&gm);
					
		}
		else if(iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(op)) == LBRACK){
			if(iter_prev->type == COMMA) iter_prev->type = ROW; 
			else if(iter_prev->type != ROW){
				struct fbgc_object * ito = derive_from_new_int_object(ROW,1);
				ito->next = iter_prev->next;
				iter_prev->next = ito;
				iter_prev = ito;
			}
		}
		else if(iter->type == RPARA || iter->type == NEWLINE || iter->type == SEMICOLON){

			_warning("%s out of while loop\n",_obj2str(iter));
			_warning("Grammar top %s\n",_gm2str(gm.top));

			/*if(is_empty_fbgc_ll_object(op) ){
				gm.top = GM_NEWLINE;
				break;
			}*/

			if(iter->type == RPARA && TOP_LL(op)->type == LPARA) delete_front_fbgc_ll_object(op);
			else if(iter->type == NEWLINE && TOP_LL(op)->type == LPARA) delete_front_fbgc_ll_object(op);

			iter_prev = handle_before_paranthesis(iter_prev,op,&gm);
			if(iter_prev == NULL){
				error_code = _FBGC_SYNTAX_ERROR; goto PARSER_ERROR_LABEL;
			}	

			if(iter->type == NEWLINE || iter->type == SEMICOLON) gm.top = GM_NEWLINE;

			if(iter_prev->type == BUILD_TUPLE){
				if(iter->type == NEWLINE && !is_empty_fbgc_ll_object(op)){
					//Now fool the loop and check newline state again,
					iter_prev->next = iter;
					iter = iter_prev;
				}
			}
		}
		else {
			push_front_fbgc_ll_object(op,iter);
		}


		break;
	}
	case ASSIGN:
	case RSHIFT_ASSIGN:
	case LSHIFT_ASSIGN:
	case STARSTAR_ASSIGN:
	case SLASHSLASH_ASSIGN:
	case PLUS_ASSIGN:
	case MINUS_ASSIGN:
	case STAR_ASSIGN:
	case SLASH_ASSIGN:
	case CARET_ASSIGN:
	case PERCENT_ASSIGN:
	{

		if( (error_code = gm_seek_left(&gm,iter)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }
		

		if(TOP_LL(op)->type != IDENTIFIER && TOP_LL(op)->type != LOAD_SUBSCRIPT){
			error_code = _FBGC_SYNTAX_ERROR;
			cprintf(100,"Assignment operator requires identifier before itself\n");
			goto PARSER_ERROR_LABEL;
		}
		
		//XXX remove this push itself thing
		//fbgc_assert(TOP_LL(op)->type == IDENTIFIER ,"Assignment to a non-identifier object, object type:%s\n",object_name_array[TOP_LL(op)->type]);
		//cprintf(111,"top->next = %s\n",object_name_array[TOP_LL(op)->next->type]);
		if(TOP_LL(op)->next != NULL && 
			(TOP_LL(op)->next->type == ASSIGN || TOP_LL(op)->next->type == LPARA || TOP_LL(op)->next->type == COMMA) ) {
			
			_info_green("Opening push itself flag\n");

			if(TOP_LL(op)->next->next == NULL || TOP_LL(op)->next->next->type != FOR)
				set_id_flag_PUSH_ITSELF(TOP_LL(op));

			//this creates error when we use "for(i=smth)... end", it pushes iter object
		}

		iter_prev->next = iter->next;

		if(TOP_LL(op)->type == IDENTIFIER){
			_info("Stack top is [%x]ID<%d>\n",get_id_flag(TOP_LL(op)),get_id_opcode_loc(TOP_LL(op)));
			TOP_LL(op)->type = iter->type;				
		}
		else if(TOP_LL(op)->type == LOAD_SUBSCRIPT){
			_info("Stack top is load subscript, converting to assign\n");
			TOP_LL(op)->type = ASSIGN_SUBSCRIPT;
		}
		
		break;
	}
	default:{

		_warning("Undefined object type[%s] in parser\n",_obj2str(iter));
		assert(0);
	}
}

	iter = iter_prev->next;
	
	_print_main_list();
	_print_op_stack();
	_cprint(101,"[GM]:{%s}\n\n",_gm2str(gm.top));

	}
	//make the linked list connection proper
	head->tail->next = iter_prev;

	if(!is_empty_fbgc_ll_object(op)){
		cprintf(100,"Syntax error in line %d, Non-empty operator stack!\n",line_no);
		print_fbgc_ll_object(op,"O");
		return 0;
	}

	_info_green("==================================\n");
	_print("Parser finished the job successfully\n");
	_println_object("Defined symbols in the parser:",fbgc_symbols);
	_print("Global field locals:"); _print_field_locals(*field_obj);
	_info_green("==================================\n\n");

	return 1;


	PARSER_ERROR_LABEL:
	fbgc_error(error_code,line_no);
		cprintf(100,"%s\n",line);
	return 0;

} 	
