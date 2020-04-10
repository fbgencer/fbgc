#include "fbgc.h"

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


void handle_function_args(struct fbgc_ll_base * fun_holder,struct fbgc_ll_base * arg_end){
	//	arg_start is always next pt of fun_holder
	//	arg_start holds parsed arg expression beginning
	//	example: fun(x,y,z) will be parsed x,y,z
	//							  arg_start^   ^arg_end
	//	There could be default assignment situations, we need to handle assignemnt expr and other type of args
	//Variadic arguments
	//We define them by putting function call operator(), ex: fun(a,b,args())
	
	struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;
	struct fbgc_object * tp = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(fun_obj)->code)->content;
	cast_fbgc_object_as_fun(fun_obj)->no_arg = size_fbgc_tuple_object(tp); 

	FBGC_LOGD(PARSER,"Function arg no is:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);

	//If arg no is just 1, arg end does not hold correctly
	//if(cast_fbgc_object_as_fun(fun_obj)->no_arg == 1)
	//	arg_end = arg_end->next;

	//Validate the arguments are they normal ?
	struct fbgc_ll_base * iter = fun_holder; //args start is iter->next
	while(iter != arg_end){
		if(iter->next->type == IDENTIFIER || iter->next->type == COMMA){
			FBGC_LOGV(PARSER,"Ok moving, typical function arg\n");
			iter = iter->next;
		}
		else if(iter->next->type == FUN_CALL){
			//this is variadic template, it must be the last arguments
			if(iter->next->next != arg_end){
				_FBGC_LOGE("Variadic argmust be the last arg!\n");
				assert(0);	
			}

			//Make it negative so we will understand that it has variadic template
			cast_fbgc_object_as_fun(fun_obj)->no_arg *= -1;

			arg_end = iter->next;
			
			if(arg_end->next->type == COMMA){
				arg_end = arg_end->next;
			}

			break;

		}
		else{
			_FBGC_LOGE("Syntax error in function definition! iter_type :%s\n",lltp2str(iter->next));
			assert(0);
		}
	}

	fun_holder->next = arg_end->next;



	_info("Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);	
}

void handle_before_paranthesis(struct parser_packet * p){
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));
	
	const fbgc_token top_type = TOP_LL(p->op)->type;

	uint8_t	state = 0;
	if(p->iter_prev->type == COMMA) state = 2;
	else if(p->gm != GM_ATOM) state = 1;

	if(top_type == FUN_CALL){
		
		_print("Function call template, p->iter_prev:%s\n",lltp2str(p->iter_prev));

		//state == 2, p->iter_prev is comma
		if(state == 2){
			p->iter_prev->type = FUN_CALL;
		}
		else{

			struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(FUN_CALL,state==1);
			ito->next = p->iter_prev->next;
			p->iter_prev->next = ito;
			p->iter_prev = ito;
		}
		_cast_llbase_as_llopcode_int(p->iter_prev)->content += _cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;

		//gm_seek_right(gm,TOP_LL(p->op));
		struct fbgc_ll_base * iter = p->iter_prev->next;
		//Insert top p->op to the list  
		//p->iter_prev->next = TOP_LL(p->op);
		POP_LL(p->op);
		//p->iter_prev = p->iter_prev->next;
		
		char put_pop_top = 0;

		if(!is_empty_fbgc_ll(p->op) )
			put_pop_top = 1;

		put_pop_top = 0;

		if(put_pop_top){
			p->iter_prev->next = _new_fbgc_ll_base(POP_TOP);
			p->iter_prev = p->iter_prev->next;			
		}

		p->iter_prev->next = iter;
	}
	else if(top_type == FUN_MAKE){

		_print("Stack top is FUN_MAKE, function make template!\n");
		struct fbgc_ll_base * fun_holder =  _cast_llbase_as_lljumper(TOP_LL(p->op))->content; 
		struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;

		//Is it possible this is not useful*
		if(cast_fbgc_object_as_fun(fun_obj)->no_arg < 65){
			_print("already created function,arg :%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);
			return;
		}
		//fun_make content holds function object, parse the arguments
		//gm_seek_right(&p);

		_print("Function Arg Start:%s\n",lltp2str(fun_holder->next));
		_print_fbgc_ll_code(fun_holder->next,p->iter_prev);
		_print("Function Arg End:%s\n",lltp2str(p->iter_prev));

		handle_function_args(fun_holder,p->iter_prev);
		p->iter_prev = fun_holder;
	}
	else if(top_type == CLASS_MAKE){
		FBGC_LOGD(PARSER,"Stack top is CLASS_MAKE, making class template!\n");



		struct fbgc_ll_base * cls_holder =  _cast_llbase_as_lljumper(TOP_LL(p->op))->content; 
		struct fbgc_object * cls_obj = _cast_llbase_as_llconstant(cls_holder)->content;

		if(_cast_llbase_as_llconstant(TOP_LL(p->scope_list))->content == cls_obj)
			return;

		_print("Class Arg Start:%s\n",lltp2str(cls_holder->next));
		//_print_fbgc_ll_code(cls_holder->next,p->iter_prev);
		_print("Class Arg End:%s\n",lltp2str(p->iter_prev));
		_push_front_fbgc_ll(p->scope_list,_new_fbgc_ll_constant(cls_obj));

		struct fbgc_ll_base * iter = cls_holder;
		/*while(iter != p->iter_prev){
			if(iter->next->type == IDENTIFIER){
				iter = iter->next;
			}
			else assert(0);
		}*/

		//cls_holder->next = p->iter_prev->next;
		if(cls_holder->next->type == IDENTIFIER){
			cls_holder = cls_holder->next;
			//p->iter_prev = p->iter_prev->next;
			//p->gm = GM_NEWLINE;
		}
		cast_fbgc_object_as_class(cls_obj)->code = cls_holder;

		_print("Class code :%s\n",lltp2str(cast_fbgc_object_as_class(cls_obj)->code));


	}
	else if(top_type == FOR){

		if(_cast_llbase_as_lljumper(TOP_LL(p->op))->content->type == FOR_BEGIN)
			return;
		
		//XXgm_seek_right(gm,TOP_LL(p->op));

		_print("Stack top is FOR, for loop template!\n");
		_print("For loop content:%s\n",lltp2str(_cast_llbase_as_lljumper(TOP_LL(p->op))->content));
		_print("Iterator previous:%s\n",lltp2str(p->iter_prev));
		
		if(p->iter_prev->type != ASSIGN){
			//This makes sure we have for i = x type of loop definition
			return;
		}

		struct fbgc_ll_base * d = _cast_llbase_as_lljumper(TOP_LL(p->op))->content;
		while(d->next != p->iter_prev)
			d = d->next;
		
		struct fbgc_ll_base * fb = _new_fbgc_ll_base(FOR_BEGIN);
		fb->next = d->next;
		d->next = fb;
		_cast_llbase_as_lljumper(TOP_LL(p->op))->content = fb;

	}	
	else if( (top_type == IF || top_type == ELIF || top_type == WHILE) && _cast_llbase_as_lljumper(TOP_LL(p->op))->content == NULL){
		_print("Stack top is %s, conditional template!\n",lltp2str(TOP_LL(p->op)));
		//XXgm_seek_right(gm,TOP_LL(p->op));
		_cast_llbase_as_lljumper(TOP_LL(p->op))->content = p->iter_prev;
		_warning("If shows %s\n",lltp2str(p->iter_prev->next));	
	}
	else if(p->iter_prev->type == COMMA || p->gm == GM_ATOM){
		
		if(state == 2){
			_debug("State number is 2, changing COMMA to BUILD_TUPLE\n");
			p->iter_prev->type = BUILD_TUPLE;
		} 
		else if(state == 0){
			_debug("State number is 0, creating new object BUILD_TUPLE with 0 size\n");
			struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(BUILD_TUPLE,0);
			ito->next = p->iter_prev->next;
			p->iter_prev->next = ito;
			p->iter_prev = ito;
		}
		//If nothing than it's a tuple!

	}
	else{
		
		//Should we put an error or not ....?
		/*_println_object("iter prev :",p->iter_prev);
		_print_op_stack();
		assert(0);*/
	}

	_info("==================================\n");
}

void handle_before_brackets(struct parser_packet *p){
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));

	
	uint8_t	state = 0;
	if(p->iter_prev->type == COMMA) state = 2;
	else if(p->gm != GM_ATOM) state = 1;


	
	if(is_fbgc_ASSIGNMENT_OPERATOR(p->iter_prev->next->type)) return;
	
	if(TOP_LL(p->op)->type == IDENTIFIER || TOP_LL(p->op)->type == LOAD_SUBSCRIPT){	
		_info("Operator stack top load_local or global, this is a subscript call template!\n");

		struct fbgc_ll_base * iter = p->iter_prev->next;
		//Insert top op to the list  
		p->iter_prev->next = TOP_LL(p->op);
		POP_LL(p->op);
		p->iter_prev = p->iter_prev->next;
		p->iter_prev->next = iter;
	}
	else{
		if(state == 2) p->iter_prev->type = BUILD_MATRIX; 
		else {
			struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(BUILD_MATRIX,state==1);
			ito->next = p->iter_prev->next;
			p->iter_prev->next = ito;
			p->iter_prev = ito;
		}		
	}
	
	_info("==================================\n");
}

#define PARSER_CHECK_ERROR(p_error_code)( {if(p_error_code != _FBGC_NO_ERROR) goto PARSER_ERROR_LABEL;} )

uint8_t parser(struct fbgc_object ** field_obj, FILE * input_file){

	FBGC_LOGD(PARSER,"==========[PARSER]==========\n");

	struct parser_packet p = {
		.head = _cast_llbase_as_ll( cast_fbgc_object_as_field(*field_obj)->head ),
		.iter = p.head->base.next,
		.iter_prev = (struct fbgc_ll_base *)p.head, //note that p.iter_prev->next is p.iter, always this is the case!
		.op = _new_fbgc_ll(),
		.scope_list = _new_fbgc_ll(),
		.error_code = _FBGC_NO_ERROR,
		.gm = GM_NEWLINE,
	};
	
		
	_push_front_fbgc_ll(p.scope_list,_new_fbgc_ll_constant(*field_obj));
	#define current_scope _cast_llbase_as_llconstant(TOP_LL(p.scope_list))->content
	#define previous_scope _cast_llbase_as_llconstant(TOP_LL(p.scope_list)->next)->content


	char line[1000] = {0};
	int line_no = 0;

	for(int i = 0; fbgc_error(p.error_code,line_no) ; i++){
		if(p.iter == p.head->tail){
			if(fbgc_getline_from_file(line, sizeof(line), input_file)){
				++line_no;

				if(line[0] == '#' || line[0] == '\0' || line[0] == '\n') {
					--i;
					continue; //fast passing the comment
				}
				_debug("Processed line[%d]:{%s}\n",line_no,line);
				
				p.head->tail->next = p.iter_prev;
				regex_lexer(field_obj,line);
				p.iter = p.iter_prev->next;
			}
			else break;
		}
	
	FBGC_LOGD(PARSER,"-----[%d]{%s}-------\n",i,lltp2str(p.iter));

	switch(p.iter->type){
	case CONSTANT:{
		//! check the grammar and move forward, we don't push constants to stack
		gm_seek_left(&p);
		PARSER_CHECK_ERROR(p.error_code);
		p.iter_prev = p.iter;
		break;
	}
	case IDENTIFIER:{

		//First check the grammar
		gm_seek_left(&p);
		PARSER_CHECK_ERROR(p.error_code);

		FBGC_LOGV(PARSER,"Current Scope for identifier:(%s)\n",objtp2str(current_scope));
		
		struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,get_ll_identifier_loc(p.iter));
		FBGC_LOGV(PARSER,"Symbol name as a cstring object:%s\n",content_fbgc_cstr_object(cstr_obj));
			
		struct fbgc_object * cf = NULL;

		if(!is_empty_fbgc_ll(p.op) && TOP_LL(p.op)->type == DOT){
			//so this is just a member selection
			POP_LL(p.op); //pop the dot object
			//now get the name of instance
			struct fbgc_ll_base * cstr_holder = _new_fbgc_ll_constant(cstr_obj);
			p.iter_prev->next = cstr_holder;
			cstr_holder->next = p.iter->next;
			p.iter_prev = cstr_holder;
			set_id_flag_MEMBER(p.iter);
		}
		else if((cf = new_cfun_object_from_str(*field_obj,content_fbgc_cstr_object(cstr_obj))) != NULL){
			//remove identifier object, put cfun object into list
			//cf->next = p.iter->next;
			//p.iter = cf;
			struct fbgc_ll_base * cfun_holder = _new_fbgc_ll_constant(cf);
			p.iter_prev->next =  cfun_holder;
			cfun_holder->next = p.iter->next;
			p.iter_prev = cfun_holder;
			break;	
		}
		else if(current_scope->type == FIELD || current_scope->type == CLASS){

			struct fbgc_object * local_array; 

			if(current_scope->type == FIELD) local_array = cast_fbgc_object_as_field(current_scope)->locals;
			else local_array = cast_fbgc_object_as_class(current_scope)->locals;

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
				_info("Creating a variable inside a field obj\n");
				
				struct fbgc_identifier id;		
				id.name = cstr_obj; id.content = NULL;
				local_array = push_back_fbgc_array_object(local_array,&id);
				where = size_fbgc_array_object(local_array)-1;

				if(current_scope->type == FIELD) cast_fbgc_object_as_field(current_scope)->locals = local_array;
				else cast_fbgc_object_as_class(current_scope)->locals = local_array;
				
				_info("Symbol is created @ [%d]\n",where);
			}
			else{
				_info_green("Symbol is already defined @ [%d]\n",where);
			}

			
			set_id_flag_GLOBAL(p.iter);

			_cast_fbgc_object_as_llidentifier(p.iter)->loc = where;
		}

		else if(current_scope->type == FUN){				
			struct fbgc_object * local_tuple = 
				_cast_llbase_as_llconstant(cast_fbgc_object_as_fun(current_scope)->code)->content;
			int where = index_fbgc_tuple_object(local_tuple,cstr_obj);
			set_id_flag_LOCAL(p.iter);

			if(where == -1) {
				_print("p.iter->next type(%s)\n",lltp2str(p.iter->next));

				//65 arg means func definition hasnt been done yet! So we are reading arguments
				if(p.iter->next->type == ASSIGN  || cast_fbgc_object_as_fun(current_scope)->no_arg == 65){
					_info("Id object is an argument of function or a new definition inside the function\n");

					local_tuple = push_back_fbgc_tuple_object(local_tuple,cstr_obj);
					where = size_fbgc_tuple_object(local_tuple)-1;
					_cast_llbase_as_llconstant(cast_fbgc_object_as_fun(current_scope)->code)->content = local_tuple;
					
					_println_object("Fun local tuple:",local_tuple);
				}
				else {
					_info("Searching ID object in global (class or field)\n");

					local_tuple = cast_fbgc_object_as_field(*field_obj)->locals;
					struct fbgc_identifier * temp_id; 
					for(int i = 0; i<size_fbgc_array_object(local_tuple); i++){
						temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_tuple,i);
						if(temp_id->name == cstr_obj) where = i; 
					}						
					
					if(where == -1){
						_info("Creating a variable inside a previous global\n");
						struct fbgc_object * local_array; 

						local_array = cast_fbgc_object_as_field(*field_obj)->locals;
						

						
						struct fbgc_identifier id;		
						id.name = cstr_obj; id.content = NULL;
						local_array = push_back_fbgc_array_object(local_array,&id);
						where = size_fbgc_array_object(local_array)-1;

						cast_fbgc_object_as_field(*field_obj)->locals = local_array;
						
					}
					_print("Id object is found @ [%d]",where);
					set_id_flag_GLOBAL(p.iter);
				}
			}
			
			
			set_ll_identifier_loc(p.iter,where);
		}

		p.iter_prev->next = p.iter->next;
		_push_front_fbgc_ll(p.op,p.iter);
		break;
	}
	case END:{

		//Check the grammar is it ok ?
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);


		p.iter_prev->next = p.iter->next;
		
		fbgc_token top_type = TOP_LL(p.op)->type;

		switch(top_type){
			case IF:
			case ELIF:{
				//Take IF/ELIF object from op-stack
				//ELIF and IF are not different when inserting them into list, so no need to make another condition
				struct fbgc_ll_base * if_obj = _top_and_pop_front_fbgc_ll(p.op);
				//Recal that if_obj holds its location to insert correctly
				//content->nect is where we are gonna insert IF
				//if_obj->next = _cast_llbase_as_lljumper(if_obj)->content->next;
				//_cast_llbase_as_lljumper(if_obj)->content->next = if_obj;
				//Following function does the above operations
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(if_obj)->content,if_obj);
				//Now if object know where to jump if condition is not satisfied
				_cast_llbase_as_lljumper(if_obj)->content = p.iter_prev; //now assign where to jump
				break;
			}
			case WHILE:{
				//now insert while in its place,
				struct fbgc_ll_base * wh_obj = _top_and_pop_front_fbgc_ll(p.op);
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(wh_obj)->content,wh_obj);

				//Now also insert jump object back to END token which is iter_prev
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,jump_obj);

				//While object is gonna jump to next of its content for unsatisfied conditions
				_cast_llbase_as_lljumper(wh_obj)->content = jump_obj;
				break;
			}
			case FOR:{
				//Now insert for in its place
				struct fbgc_ll_base * for_obj = _top_and_pop_front_fbgc_ll(p.op);
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(for_obj)->content,for_obj);

				//Now also insert jump object back to END token which is iter_prev
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,jump_obj);

				//for content holds prev location of for, jump need to know where to jump to make loop
				_cast_llbase_as_lljumper(jump_obj)->content = _cast_llbase_as_lljumper(for_obj)->content;	
				
				//For object is gonna jump to next of its jump_obj to finish the loop
				_cast_llbase_as_lljumper(for_obj)->content = jump_obj;
				break;

			}
			case FUN_MAKE:{
				//Check the function is empty or not, we need to handle this

				if(is_constant_and_token(p.iter_prev,FUN) ){
					p.error_code = _FBGC_SYNTAX_ERROR;
					FBGC_LOGE("Functions cannot be empty\n");
					goto PARSER_ERROR_LABEL;
				}

				//Now we need to find which functions are we holding
				struct fbgc_ll_base * fun_holder = _cast_llbase_as_lljumper(TOP_LL(p.op))->content;
				struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;
				
				//Recall that fun_obj code was holding a tuple which holds local symbols, its size gives us number of locals
				struct fbgc_object * tp = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(fun_obj)->code)->content;
				cast_fbgc_object_as_fun(fun_obj)->no_locals = size_fbgc_tuple_object(tp);
				FBGC_LOGV(PARSER,"Function number of locals:%lu\n",size_fbgc_tuple_object(tp));
				//Now we know where our function starts, so its code will hold directly a linked list taken from main list
				// fun_holder, ...., other things in list
				//             ^----------------------------------fun_object code starts here
				cast_fbgc_object_as_fun(fun_obj)->code = fun_holder->next;
				//Notica that we did not take the whole list from the main list, we need return something
				//if user does not return anything and try to get something this will yield stack error during interpreter phase
				
				//We will not create another nil_object, we will use the same nil all the time
				//insert next to p.iter and iterate it to its next
				
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(__fbgc_nil));
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_base(RETURN));
				
				
				
				//Now we are finishing fun_obj->code
				p.iter_prev->next = NULL; //last token in function code shows NULL
				//Now finished, the only reference from previous tokens are from fun_obj->code
				p.iter_prev = fun_holder;

				POP_LL(p.op); //Remove fun_make
				POP_LL(p.scope_list); //remove this function from scope list

				//eat END we no longer need it
				p.iter_prev->next = p.iter->next;
				break;

			}
			case CLASS_MAKE:{
				if(is_constant_and_token(p.iter_prev,CLASS)){
					p.error_code = _FBGC_SYNTAX_ERROR;
					FBGC_LOGE("Classes cannot be empty\n");
					goto PARSER_ERROR_LABEL;
				}

				struct fbgc_ll_base * cls_holder = _cast_llbase_as_lljumper(TOP_LL(p.op))->content;
				struct fbgc_object * cls_obj = _cast_llbase_as_llconstant(cls_holder)->content;
				
				//Now check for inheritance, assuming that is not gonna be long so we can just iterate through next to cls_holder
				//layout look like this
				// cls_holder, father1, father2, fatherN, cls_obj->code starts here, ... 
				uint8_t arg_no = 0;
				while(cls_holder != cast_fbgc_object_as_class(cls_obj)->code){
					cls_holder = cls_holder->next;
					arg_no++;
				}

				FBGC_LOGV(PARSER,"Class inheritance arg no:%d\n",arg_no);

				//Now we no longer need to previous arguments
				cls_holder = cast_fbgc_object_as_class(cls_obj)->code;
				cast_fbgc_object_as_class(cls_obj)->code = cast_fbgc_object_as_class(cls_obj)->code->next;

				//We must return this class object
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_constant(cls_obj));
				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_base(RETURN));
				
				//Now finish the class code
				p.iter_prev->next = NULL;
				
				POP_LL(p.op); //Remove class_make
				POP_LL(p.scope_list); //remove class from scope list

				//We need to put class_call for the first time class definitions in order to run their code
				p.iter_prev = cls_holder; //Start from cls_holder

				p.iter_prev = _insert_next_fbgc_ll(p.iter_prev,_new_fbgc_ll_opcode_int(CLASS_CALL,arg_no));
				p.iter_prev->next = p.iter->next;

				break;
			}
			case JUMP:{
				break;
			}
			default:{
				FBGC_LOGE("END keyword is not required here\n");
				p.error_code = _FBGC_SYNTAX_ERROR;
				goto PARSER_ERROR_LABEL;
			}
		}

		//Is there any jump object left in the stack ? make their connections
		while(TOP_LL(p.op)->type == JUMP){
			struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p.op);
			jump_obj->next = _cast_llbase_as_lljumper(jump_obj)->content->next;
			_cast_llbase_as_lljumper(jump_obj)->content->next = jump_obj;
			_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;	
		}

		break;			
	}
	case FUN_MAKE:{
		//FUN_MAKE is a lljumper type holder, it will hold the function object
		//while we are seeing till the END token.

		//Check the grammar is it ok ?
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);

		//Create a new function object and holder for this object
		//Holder must be a constant_holder because it has a fbgc_object * type pointer
		struct fbgc_object * fun_obj = new_fbgc_fun_object();
		struct fbgc_ll_base * fun_holder = _new_fbgc_ll_constant(fun_obj);

		//Now we do not need iter at this point, later we will push it into our op-stack
		//This function takes 3 args and makes the following operatiobs

		// 1) fun_holder->next = p.iter->next;
		// 2) p.iter_prev->next = fun_holder;
		// 3) p.iter_prev = fun_holder;
		//For the final assignment it returns fun_holder so we update our iter_prev
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,fun_holder);

		//Now FUN_MAKE object shows fun_holder and we will push it into op-stack
		_cast_llbase_as_lljumper(p.iter)->content = fun_holder;
		_push_front_fbgc_ll(p.op,p.iter);
		//When we have END operator we will ask FUN_MAKE object to learn which fun obj is this.

		//Now update the scope we need it for local identifiers
		//This scope is opened before reading arguments and it is important to note that arguments are assumed local variable
		//First argument is local<0>, the second is local<1> etc. When we call this function in interpreter we do not change the 
		//location of args, we say that local<0> starts at first argument and so on.
		_push_front_fbgc_ll(p.scope_list,_new_fbgc_ll_constant(fun_obj));
		//When we create functions we need a local identifier holder and this must be a linear array
		//because when a new identifier is read it just becomes local<n>, and they are hold by the following tuple.
		//here we use tuple object to hold each identifier, when we finish the definition of our function
		//we no longer need to hold this tuple. That's why code pointer is used for this purpose.
		//Normally code should point code but at this point it just holds one type 
		cast_fbgc_object_as_fun(fun_obj)->code = _new_fbgc_ll_constant(new_fbgc_tuple_object(0));

		FBGC_LOGV(PARSER,"New fun_obj is inserted into list. FUN_MAKE goes to op-stack. Scope updated\n");
		break;		
	}
	case CLASS_MAKE:{
		//CLASS_MAKE is a lljumper type holder, it will hold a class object
		//while we are seeking till the END token.
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);

		//Create a new class_object and its holder to insert into the list
		struct fbgc_object * cls_obj = new_fbgc_class_object();
		struct fbgc_ll_base * cls_holder = _new_fbgc_ll_constant(cls_obj);

		//Insert cls_holder and remove iter from the list
		p.iter_prev = _insert_fbgc_ll(p.iter_prev,p.iter,cls_holder);

		//Now class_make shows our class object, push it into the stack
		_cast_llbase_as_lljumper(p.iter)->content = cls_holder;
		_push_front_fbgc_ll(p.op,p.iter);
		
		//Since class object arguments come from global scope and they are related with inheritance
		//scope change is start after finishing the reading of class arguments. This way is different than fun_make

		FBGC_LOGV(PARSER,"New cls_obj is inserted into list. CLASS_MAKE goes to op-stack. ");
		break;
	}	
	case ELIF:
	case ELSE:{
		//ELIF and ELSE are both lljumper type holder, because later we need to jump if condition is not satisfied
		//They both check op-stack to make sure there are previous if/elif tokens. Otherwise it raises syntax error
		//Layout for IF-ELIF-ELSE structure as follows
		//				   |-----------v			|-----------v
		// ..., condition,if, .... jump,condition,elif,.... jump,else,... , ...
		//							|-------------------------|--------------^
		//													  |--------------^
		//We push current tokens into stack to check if-elif-else like structure to not make syntax mistakes.
		//They hold iter_prev which later will be used to insert next to their content

		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);

		//When we read ELIF token we expect previous ELIF and IF tokens in op-stack
		//If either of those is not satisfied this is an error, put syntax error
		if(TOP_LL(p.op)->type != IF || TOP_LL(p.op)->type != ELIF){
			p.error_code = _FBGC_SYNTAX_ERROR;
			goto PARSER_ERROR_LABEL;
		}

		//Now stack op is if or elif object, which is a jumper type holder
		//Get the top object and pop from op stack, we will insert it into the main list
		struct fbgc_ll_base * if_obj = _top_and_pop_front_fbgc_ll(p.op);

		//The content of if object knows where are we gonna insert it
		//Basically it holds its location. It must be insterted next of the content
		//The following function does the following operations
		// 1) if_obj->next = _cast_llbase_as_lljumper(if_obj)->content->next;
		// 2) _cast_llbase_as_lljumper(if_obj)->content->next = if_obj;
		_insert_next_fbgc_ll(_cast_llbase_as_lljumper(if_obj)->content,if_obj);

		//Now get a jump object because when a condition is satisfied for the previous if/elif 
		//There must be a jump at the end of their scope to finish conditional structure
		_cast_llbase_as_lljumper(if_obj)->content = _new_fbgc_ll_jumper_with_content(JUMP, p.iter_prev);

		//struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
		//_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;
		//_cast_llbase_as_lljumper(if_obj)->content = jump_obj;

		//Push the jump object into op-stack
		_push_front_fbgc_ll(p.op,_cast_llbase_as_lljumper(if_obj)->content);
		//Remove iter from the list
		p.iter_prev->next = p.iter->next;

		//if iter is elif we need to see it into our stack but if it is else we don't need it
		if(p.iter->type == ELIF){
			FBGC_LOGV(PARSER,"ELIF is pushed into op-stack \n");
			_push_front_fbgc_ll(p.op,p.iter);		
		}

		FBGC_LOGV(PARSER,"Previous conditional inserted into list. JUMP is push into op-stack \n");

		break;
	}
	case WHILE:{
		//WHILE is a lljumper type holder, 
		//Here jump object must be another jumper to finish and jump succesfully beginning of the while loop
		//While object holds next to jump to jump if the condition is not satisfied
		//Typical list layout for while is
		//                 |----------------v
		//..., condition,while, ... , jump, ...
		//    ^-------------------------|
		
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);

		
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
		if(TOP_LL(p.op)->type != WHILE){
			//Jump now holds token before the while
			_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;
		}
		else{
			//@TODO leaving this part as a comment, not tested!
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
			
			//_cast_llbase_as_lljumper(jump_obj)->content = TOP_LL(p.op);
			assert(0);
		}
		//Push jump object
		_push_front_fbgc_ll(p.op,jump_obj);
		//Take the current iterator from the list, eating while to push into op-stack
		p.iter_prev->next = p.iter->next;
		//Insert WHILE object into op-stack
		_push_front_fbgc_ll(p.op,p.iter);	
		FBGC_LOGV(PARSER,"WHILE is finished. JUMP and WHILE objects are pushed into stack \n");
		break;
	}
	case FOR:{
		//@TODO FOR and WHILE have the same structure, merge them

		//FOR is a jumper type holder
		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);

		//For needs to know its current location to insert later
		_cast_llbase_as_lljumper(p.iter)->content = p.iter_prev;

		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
		_cast_llbase_as_lljumper(jump_obj)->content = p.iter_prev;
		//Push jump holder
		_push_front_fbgc_ll(p.op,jump_obj);
		//take FOR from main list 
		p.iter_prev->next = p.iter->next;	
		//push FOR into main list
		_push_front_fbgc_ll(p.op,p.iter);
		FBGC_LOGV(PARSER,"FOR is finished. JUMP and FOR objects are pushed into stack \n");	
		break;
	}			
	case BREAK:
	case CONT:{
		//Break and Cont both require while or for loop in op-stack
		//in order to find one we need to iterate through the stack, if we cannot find then this is a syntax error

		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);

		struct fbgc_ll_base * op_top = TOP_LL(p.op);
		for(;;){
			//Ok now move in op-stack
			if(op_top->type == WHILE || op_top->type == FOR){
				FBGC_LOGV(PARSER,"%s found %s in op-stack \n",lltp2str(p.iter),lltp2str(op_top));
				//There is a loop object
				if(p.iter->type == CONT){
					assert(op_top->next->type == JUMP); //Is there any way that we cant see jump here?

					//for CONT token we need to go back our loop object and we cannot hold it directly because
					//we need to check condition first, that's why we move iterator to next which must be JUMP object
					//In interpreter we are gonna jump to JUMP and ask for its content which shows the beginning of the condition
					//of loop object
					//here we assumed there will be always jump object after loop objects
					op_top = op_top->next;
				}

				//Know break or continue is gonna hold the object, in interpreter when we execute break or cont
				//we will go to this loop object and ask for its end because loop object holds their ends.
				_cast_llbase_as_lljumper(p.iter)->content = op_top;
				break;
			}
			op_top = op_top->next;

			if(op_top == _cast_llbase_as_ll(p.op)->tail){
				//We are at the end of list and we found nothing so this is an error, no loop object before CONT/BREAK
				FBGC_LOGE("%s is not inside a loop \n",lltp2str(p.iter));
				p.error_code = _FBGC_SYNTAX_ERROR;
				goto PARSER_ERROR_LABEL;
			}
		}
		//Finally everything is ok, we found
		//Just iterate in the main list, we are not gonna put this token in op-stack;
		p.iter_prev = p.iter;
		FBGC_LOGV(PARSER,"Moving in the list %s stays \n",lltp2str(p.iter));

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

		//take the p.op object from main list and connect previous one to the next one 
		//[H]->[2]->[+]->[3] => [H]->[2]->[3], now p.iter holds the operator, p.iter->next is [3] but we will change that too 
		//     p^	i^					
		p.iter_prev->next = p.iter->next;
		//struct fbgc_object * p.iterp2 = p.iter_prev;

		
		while( is_not_empty_fbgc_ll(p.op) && compare_operators(get_fbgc_object_type(TOP_LL(p.op)),p.iter->type) ){

			if( (p.error_code = gm_seek_right(&p)) != _FBGC_NO_ERROR ){
				goto PARSER_ERROR_LABEL;
			}

			
			if(is_pushable_in_main(get_fbgc_object_type(TOP_LL(p.op)))){
				//Insert top p.op to the list  
				p.iter_prev->next = TOP_LL(p.op);
				//Pop top from stack
				POP_LL(p.op);
				//connect list again
				p.iter_prev->next->next = p.iter->next; 
				//make the p.iter_prev proper
				p.iter_prev = p.iter_prev->next;
				
			}
			else{

				_warning("Top object is not pushable to main list\n");
				
				if(p.iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(p.op)) == LBRACK){
					
					#ifdef PARSER_DEBUG
					cprintf(110,"\tBRACK AND SEMICOLON\n");
					#endif
					break;
				}
				else if(p.iter->type == PIPE && get_fbgc_object_type(TOP_LL(p.op)) == LEN){
						
					p.iter->next = p.iter_prev->next;
					//eat the pipe insert the len
					p.iter_prev->next = TOP_LL(p.op);
					//Pop top from stack
					POP_LL(p.op);
					p.iter_prev = p.iter_prev->next;
					p.iter_prev->next = p.iter->next;

					break;
				}
				else if(get_fbgc_object_type(TOP_LL(p.op)) == LPARA){
					POP_LL(p.op);
					break;
				}
				else {
					cprintf(111,"Syntax Error :");
					cprintf(011,"Op Top :[%s], p.iter:[%s]\n",object_name_array[get_fbgc_object_type(TOP_LL(p.op))],object_name_array[p.iter->type]);
					assert(0);
					//goto END_OF_THE_PARSER;
				}
			}
			_info_green("Checking the operator stack, inside the while loop\n");
			
			FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p.head,"Main"));
			FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p.op,"Op"));
			FBGC_LOGV(PARSER,"[GM]:{%s}\n",gm2str(p.gm));
		}
		
		/*
			print((1,2,3)) gibi durumlarda p.iter_prev:cfun ve stacktop:lpara olduğu için
			tuple kurmak yerine function call atıyor. Bu hatayı düzeltmek için grammarde ne olduğunu bilmek gerekir
			Bu tip durumların düzeltilmesi gerekli.
		*/		
		fbgc_grammar gm_prev = p.gm;

		if( (p.error_code = gm_seek_left(&p)) != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }

		if(p.iter->type == LPARA){
			_warning("p.iter is LPARA, p.iter_prev %s\n",lltp2str(p.iter_prev));

			// && TOP_LL(p.op)->type != LPARA we need to put this to ensure tuple entries

			if((gm_prev == GM_ID || gm_prev == GM_MATRIX) && (p.iter_prev->type == IDENTIFIER || p.iter_prev->type == LOAD_SUBSCRIPT || is_constant_and_token(p.iter_prev,CFUN) )) {
						
				if(p.iter_prev->type == CONSTANT && _cast_llbase_as_llconstant(p.iter_prev)->content->type == CFUN){
					_warning("match!\n");
				}

				if(p.iter_prev->type == IDENTIFIER && is_id_flag_MEMBER(p.iter_prev)){
					set_id_flag_MEMBER_METHOD(p.iter_prev);
					_push_front_fbgc_ll(p.op,_new_fbgc_ll_opcode_int(FUN_CALL,1));
				}
				else _push_front_fbgc_ll(p.op,_new_fbgc_ll_opcode_int(FUN_CALL,0));
			}
			_push_front_fbgc_ll(p.op,p.iter);
		}
		else if(p.iter->type == LBRACK){
			_warning("p.iter is LBRACK, p.iter_prev %s\n",lltp2str(p.iter_prev));
			if(p.iter_prev->type == IDENTIFIER || p.iter_prev->type == FUN_CALL){
				_push_front_fbgc_ll(p.op,_new_fbgc_ll_base(LOAD_SUBSCRIPT));
			}
			_push_front_fbgc_ll(p.op,p.iter);
		}
		else if(p.iter->type == ROW ){
			;
		}
		else if(p.iter->type == COMMA){
			if(TOP_LL(p.op)->type == COMMA){
				_cast_llbase_as_llopcode_int(TOP_LL(p.op))->content++;
			}
			else {
				//Also insert a lpara object for tuple entries without paranthesis
				if(get_fbgc_object_type(TOP_LL(p.op)) != LPARA && get_fbgc_object_type(TOP_LL(p.op)) != LBRACK){
					_push_front_fbgc_ll(p.op,_new_fbgc_ll_base(LPARA));
				}

				_push_front_fbgc_ll(p.op, _new_fbgc_ll_opcode_int(COMMA,2));
			}
		}
		else if( p.iter->type == RBRACK){
			
			if(TOP_LL(p.op)->type == LBRACK){
				_pop_front_fbgc_ll(p.op);
			}
			else assert(0);
			handle_before_brackets(&p);
			if( p.error_code != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }

					
		}
		else if(p.iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(p.op)) == LBRACK){
			if(p.iter_prev->type == COMMA) p.iter_prev->type = ROW; 
			else if(p.iter_prev->type != ROW){
				struct fbgc_ll_base * ito = _new_fbgc_ll_opcode_int(ROW,1);
				ito->next = p.iter_prev->next;
				p.iter_prev->next = ito;
				p.iter_prev = ito;
			}
		}
		else if(p.iter->type == RPARA || p.iter->type == NEWLINE || p.iter->type == SEMICOLON){

			_warning("%s out of while loop\n",lltp2str(p.iter));
			_warning("Grammar top %s\n",gm2str(p.gm));


			if(p.iter->type == RPARA && TOP_LL(p.op)->type == LPARA) _pop_front_fbgc_ll(p.op);
			else if(p.iter->type == NEWLINE && TOP_LL(p.op)->type == LPARA) _pop_front_fbgc_ll(p.op);

			handle_before_paranthesis(&p);
			if( p.error_code != _FBGC_NO_ERROR  ){ goto PARSER_ERROR_LABEL; }


			if(p.iter->type == NEWLINE || p.iter->type == SEMICOLON) p.gm = GM_NEWLINE;

			if(p.iter_prev->type == BUILD_TUPLE){
				if(p.iter->type == NEWLINE && !is_empty_fbgc_ll(p.op)){
					//Now fool the loop and check newline state again,
					p.iter_prev->next = p.iter;
					p.iter = p.iter_prev;
					p.gm = GM_EXPRESSION;
				}
			}
		}
		else {
			_push_front_fbgc_ll(p.op,p.iter);
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
	case PERCENT_ASSIGN:{


		gm_seek_left(&p); PARSER_CHECK_ERROR(p.error_code);
		
		if(TOP_LL(p.op)->type != IDENTIFIER && TOP_LL(p.op)->type != LOAD_SUBSCRIPT){
			p.error_code = _FBGC_SYNTAX_ERROR;
			cprintf(100,"Assignment operator requires identifier before itself\n");
			goto PARSER_ERROR_LABEL;
		}
		
		//XXX remove this push itself thing
		//fbgc_assert(TOP_LL(p.op)->type == IDENTIFIER ,"Assignment to a non-identifier object, object type:%s\n",object_name_array[TOP_LL(p.op)->type]);
		//cprintf(111,"top->next = %s\n",object_name_array[TOP_LL(p.op)->next->type]);
		if(TOP_LL(p.op)->next->type == ASSIGN || TOP_LL(p.op)->next->type == LPARA || TOP_LL(p.op)->next->type == COMMA){
			
			_info_green("Opening push itself flag\n");

			if(TOP_LL(p.op)->next->next == NULL || TOP_LL(p.op)->next->next->type != FOR)
				set_id_flag_PUSH_ITSELF(TOP_LL(p.op));

			//this creates error when we use "for(i=smth)... end", it pushes p.iter object
		}

		p.iter_prev->next = p.iter->next;

		if(TOP_LL(p.op)->type == IDENTIFIER){
			_info("Stack top is [%x]ID<%d>\n",get_ll_identifier_flag(TOP_LL(p.op)),get_ll_identifier_loc(TOP_LL(p.op)));
			TOP_LL(p.op)->type = p.iter->type;				
		}
		else if(TOP_LL(p.op)->type == LOAD_SUBSCRIPT){
			_info("Stack top is load subscript, converting to assign\n");
			TOP_LL(p.op)->type = ASSIGN_SUBSCRIPT;
		}
		
		break;
	}
	default:{

		_warning("Undefined object type[%s] in parser\n",lltp2str(p.iter));
		assert(0);
	}
}

	p.iter = p.iter_prev->next;

	FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p.head,"Main"));
	FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p.op,"Op"));
	FBGC_LOGV(PARSER,"[GM]:{%s}\n",gm2str(p.gm));

	}
	//make the linked list connection proper
	p.head->tail->next = p.iter_prev;

	if(!is_empty_fbgc_ll(p.op)){
		//Check the stack and give proper error
		if(is_fbgc_FUNCTIONABLE(get_fbgc_object_type(TOP_LL(p.op)))){
			cprintf(100,"Missing <END> : <%s>\n",object_type_as_str(TOP_LL(p.op)));
			return 0;
		}

		cprintf(100,"Syntax error in line %d, Non-empty operator stack!\n",line_no);
		_print_fbgc_ll(p.op,"O");
		return 0;
	}

	FBGC_LOGV(PARSER,"==========================\nParser finished, defined symbols:");
	FBGC_LOGV(PARSER,"%c\n",print_fbgc_object(fbgc_symbols));
	FBGC_LOGV(PARSER,"Global field locals:");
	FBGC_LOGV(PARSER,"%c\n",print_field_object_locals(*field_obj));
	FBGC_LOGV(PARSER,"==========================\n");

	return 1;


	PARSER_ERROR_LABEL:
	fbgc_error(p.error_code,line_no);
		cprintf(100,"%s\n",line);
	return 0;

}