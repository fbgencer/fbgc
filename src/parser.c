#include "fbgc.h"

#define RIGHT_ASSOC 0b10000000
#define LEFT_ASSOC 0	
const fbgc_token precedence_table[] =
{
	RIGHT_ASSOC | 0,//IF
	4,//RETURN
	2,//NEWLINE
	RIGHT_ASSOC | 5,//LPARA
	16,//RPARA
	RIGHT_ASSOC | 5,//LBRACK
	18,//RBRACK
	RIGHT_ASSOC | 5,//LBRACE
	16,//RBRACE
	RIGHT_ASSOC | 18,//COMMA // it was 20 now we changed to make bigger than equal sign for x = 1,2,3 cases
	52,//DOT
	2,//SEMICOLON
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

static uint8_t compare_operators(fbgc_token stack_top, fbgc_token obj_type){
	
	FBGC_LOGV(PARSER,"%c",cprintf(111,"Object type comparison StackTop:[%s]>=Obj[%s]:",object_name_array[stack_top],object_name_array[obj_type]));
	
	// precedence outputs
	// stack_top >= obj_type => return 1;
	// stack_top < obj_type => return 0;

	uint8_t result = 0;
	if(stack_top == IDENTIFIER) 
		result = 1;
	//x=(1,2) requires low precedence!
	else if(obj_type == LBRACK || obj_type == LPARA || obj_type == LBRACE) result =  0;

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

	FBGC_LOGV(PARSER,"%d\n",result&&1);
	return result;
}


static void handle_function_args(struct parser_packet * p){

	struct fbgc_ll_base * fun_holder = _cast_llbase_as_lljumper(TOP_LL(p->op))->content;
	struct fbgc_ll_base * arg_end = p->iter_prev;

	//	arg_start is always next pt of fun_holder
	//	arg_start holds parsed arg expression beginning
	//	e.g: fun(x,y,z) will be parsed x,y,z,3
	//					      arg_start^     ^arg_end

	struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;
	struct fbgc_object * tp = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(fun_obj)->code)->content;
	cast_fbgc_object_as_fun(fun_obj)->no_arg = size_fbgc_tuple_object(tp);
	FBGC_LOGD(PARSER,"Function arg no is:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);

	//Arguments may contain comma or other types because we treat them tuples because parser does not know anything about
	//function definition, this function checks whether given args are valid or not.
	//Basic scenarios : 
	//	-Args cannot be constants
	//	-Arg names must be unique(this was already checked no need to check again)
	//	-Variadic args must be the last argument
	//There could be default assignment situations, we need to handle assignemnt expr and other type of args
	//Variadic arguments
	//We define them by putting function call operator(), ex: fun(a,b,args())
	

	//In the above code we assumed that everything is normal and get the size of tuple which gives us arg_no if nothing is wrong with args

	struct fbgc_ll_base * iter = fun_holder; //arg_start is iter->next because args start after fun holder
	struct fbgc_ll_base * from = NULL;

	struct fbgc_ll_base * def_arg_ptr = NULL;

	bool allow_exp = false;
	uint8_t count_id = 0;

	struct fbgc_ll_base * iter_next = iter->next;
	FBGC_LOGE("===================================================================================\n");
	int i = 0;
	while(iter != arg_end){
		if(++i>20) break;

		bool arg_finished_ok = false;

		FBGC_LOGV(PARSER,"Current : Now iter:%s | iter_next :%s\n",lltp2str(iter),lltp2str(iter_next));
		if(iter_next->type == IDENTIFIER && is_id_flag_LOCAL(iter_next)){
			FBGC_LOGV(PARSER,"Argument:(%s) validated!\n",content_fbgc_str_object(content_fbgc_tuple_object(tp)[get_ll_identifier_loc(iter->next)]));
			from = iter_next;
			allow_exp = true;
			iter = iter->next;
			iter_next = iter->next;
			++count_id;
			arg_finished_ok = true;
		}

		if(allow_exp == false && iter_next != arg_end){
			assert(0);
		}



		FBGC_LOGD(PARSER,"Before while Now iter:%s | iter_next :%s\n",lltp2str(iter),lltp2str(iter_next));
		while(iter != arg_end && iter_next->type != IDENTIFIER ){
			if(++i>20) break;
			printf("In while type:%s\n",lltp2str(iter_next));
			allow_exp = false;
			arg_finished_ok = true;
			if(iter_next->type == COLON){

				/*if(iter_next->next->type == IDENTIFIER){
					FBGC_LOGE("Non-default arguments cannot follow default arguments!\n");
					p->error_code = _FBGC_SYNTAX_ERROR;
					return;
				}*/

				//Default arguments
				struct fbgc_ll_base * fun_make = TOP_LL(p->op); 
				FBGC_LOGE("from:%s | %s\n",lltp2str(from),lltp2str(fun_holder->next));

				//default argument
				struct fbgc_ll_base * assign = iter_next;
				struct fbgc_ll_base * sta_begin = from->next;
				from->next = assign->next;
				
				assign = iter;

				assign->next = fun_make->next;
				fun_make->next = sta_begin;
				iter = from;

				
				if(def_arg_ptr == NULL){
					_insert_next_fbgc_ll(assign,_new_fbgc_ll_base(BUILD_FUN_DEFAULT_ARGS));
					def_arg_ptr = _new_fbgc_ll_opcode_int(BUILD_TUPLE,1);
					_insert_next_fbgc_ll(assign,def_arg_ptr);
				}
				else{
					_cast_llbase_as_llopcode_int(def_arg_ptr)->content++;
				}

				_print_fbgc_ll(p->op,"DD");
				cast_fbgc_object_as_fun(fun_obj)->flag_default_args = 1;

				if(iter_next == arg_end){
					iter = arg_end;
					break;
				}

				iter_next = iter->next;	
				break;
			}
			else if(iter_next == arg_end && iter_next->type == COMMA){
				if(count_id != _cast_llbase_as_llopcode_int(iter_next)->content){
					assert(0);
				}
				
			}
			else if(iter_next->type == FUN_CALL){
				//When we use arg_name() structure, parser thinks that this is function call and we do not change it to not make it complex
				//Here if we see fun_call that means user wants to define variadic function, variadic arg must be the last arg
				//So no need to make sure having more than one variadic args

				//first check is this the last ?
				FBGC_LOGD(PARSER,"Arg end:%s|iternextnext:%s = %d\n",lltp2str(arg_end),lltp2str(iter->next->next),arg_end == iter->next->next);
				if(iter_next->next != arg_end && cast_fbgc_object_as_fun(fun_obj)->no_arg != 1){
					FBGC_LOGE("Variadic arg must be the last arg!\n");
					p->error_code = _FBGC_SYNTAX_ERROR;
					return;	
				}
				//Make arg_no negative so we will understand that it has the variadic template
				cast_fbgc_object_as_fun(fun_obj)->no_arg *= -1;
			}
			else arg_finished_ok = false;		

			iter = iter->next;
			iter_next = iter->next;
		}

		if(allow_exp && def_arg_ptr != NULL){
			FBGC_LOGE("Non-default arguments cannot follow default arguments!\n");
			p->error_code = _FBGC_SYNTAX_ERROR;
			return;
		}

		allow_exp = false;
		if(arg_finished_ok == false){
			assert(0);
		}


		//iter = iter->next;
	}
			//FBGC_LOGE("Undefined token in function arguments\n");
			//p->error_code = _FBGC_SYNTAX_ERROR;
			//return;

	fun_holder->next = arg_end->next;
	p->iter_prev = fun_holder;

	FBGC_LOGV(PARSER,"Handling function args, # of args:%d\n",cast_fbgc_object_as_fun(fun_obj)->no_arg);

	FBGC_LOGE("===================================================================================\n");	
}


static void handle_statements(struct parser_packet * p){
	
	FBGC_LOGV(PARSER,"begin: %s, prev:%s\n",lltp2str(p->statement_begin),lltp2str(p->iter_prev));

	if(is_fbgc_ASSIGNMENT_OPERATOR(TOP_LL(p->op)->type) == 0 && is_fbgc_ASSIGNMENT_OPERATOR(p->iter_prev->type) == 0
		&& TOP_LL(p->op)->type != LOAD_SUBSCRIPT && TOP_LL(p->op)->type != ASSIGN_SUBSCRIPT 
		&& p->iter_prev->type != FUN_CALL 
		&& p->iter_prev->type != LOAD_SUBSCRIPT && p->iter_prev->type != ASSIGN_SUBSCRIPT 
		&&  p->iter_prev->type != RETURN && TOP_LL(p->op)->type != RETURN &&
		p->iter_prev->type !=  BREAK && p->iter_prev->type !=  CONT &&
		TOP_LL(p->op)->type != LPARA && TOP_LL(p->op)->type != LBRACK &&
		TOP_LL(p->op)->type != IF ){
		FBGC_LOGV(PARSER,"Statement did not assigned to a variable\n");
		//Ok remove this branch by usign statement_begin pointer
		// v-------statement begin , actual statement start at statement_begin->next but in order to remove from list we need to hold previous element
		//[]->{[]-> ... []}
		//				^---- p->iter_prev
		//Now removed all the items and statement_begin assigned to its new location

		//p->statement_begin->next = p->iter_prev->next;
		//p->iter_prev = p->statement_begin;
	}
	else if(p->iter_prev->type == FUN_CALL && is_fbgc_ASSIGNMENT_OPERATOR(TOP_LL(p->op)->type) == 0 && is_fbgc_ASSIGNMENT_OPERATOR(p->iter_prev->type) == 0
		&& TOP_LL(p->op)->type != LOAD_SUBSCRIPT && TOP_LL(p->op)->type != ASSIGN_SUBSCRIPT 
		&& p->iter_prev->type != LOAD_SUBSCRIPT && p->iter_prev->type != ASSIGN_SUBSCRIPT 
		&&  p->iter_prev->type != RETURN && TOP_LL(p->op)->type != RETURN &&
		p->iter_prev->type !=  BREAK && p->iter_prev->type !=  CONT &&
		TOP_LL(p->op)->type != LPARA && TOP_LL(p->op)->type != LBRACK &&
		TOP_LL(p->op)->type != IF){
		p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_base(POP_TOP));
	}
	else{
		FBGC_LOGV(PARSER,"Updating statement\n");					
		//p->statement_begin = p->iter_prev;
	}
}


static void handle_before_paranthesis(struct parser_packet * p){
	_FBGC_LOGD(PARSER,"%c",cprintf(010,"\n<><><><><><><><><><><><><><><><><><>\nStack top holder:%s\n",lltp2str(TOP_LL(p->op))));
	

	//Name of the function is used to define functionable entries, those can be function definition, class definition
	//if/elif/while/for and tuple constructions. Whenever we hit RPARA/SEMICOLON/NEWLINE we call this function
	//Basically it handles entries before paranthesis, although we may not come here due to paranthesis but the name is just a name.


	//	Left it as an example, just fun call and build tuple uses these cases so I defined conditions inside their case statements
	//	uint8_t	state = 0;
	//	if(p->iter_prev->type == COMMA) state = 2;
	//	else if(p->gm != GM_ATOM) state = 1;
	//

	//Get the top to decide what are we gonna do
	switch(TOP_LL(p->op)->type){
		case KWFUN_CALL:{
			FBGC_LOGV(PARSER,"Keyworded Function call,p->iter_prev:%s\n",lltp2str(p->iter_prev));

			struct fbgc_ll_base * kwcall = _top_and_pop_front_fbgc_ll(p->op);

			struct fbgc_ll_base * kwstart = _cast_llbase_as_lljumper(_top_and_pop_front_fbgc_ll(p->op))->content;
			FBGC_LOGV(PARSER,"Keyword start:%s\n",lltp2str(kwstart));
			if(kwstart->next != p->iter_prev){
				p->error_code = _FBGC_SYNTAX_ERROR;
				return;
			}

			if(p->iter_prev->type == COMMA){
				p->iter_prev->type = KWFUN_CALL;
				_cast_llbase_as_llopcode_int(p->iter_prev)->content += _cast_llbase_as_llopcode_int(kwcall)->content;
			}
			else{
				//insert the function call, if state is 1 means that we have 1 argument, if it is zero than no arg
				_cast_llbase_as_llopcode_int(TOP_LL(p->op))->content += (p->gm != GM_ATOM);
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,kwcall);
			}

			break;		
		}
		case FUN_CALL:{
			FBGC_LOGV(PARSER,"Function call,p->iter_prev:%s\n",lltp2str(p->iter_prev));

			//state == 2, p->iter_prev is comma
			if(p->iter_prev->type == COMMA){
				p->iter_prev->type = FUN_CALL;
				_cast_llbase_as_llopcode_int(p->iter_prev)->content += _cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;
				POP_LL(p->op);
			}
			else{
				//insert the function call, if state is 1 means that we have 1 argument, if it is zero than no arg
				_cast_llbase_as_llopcode_int(TOP_LL(p->op))->content += (p->gm != GM_ATOM);
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_top_and_pop_front_fbgc_ll(p->op));
			}

			//p->current_state = 0;

			/*if(!is_fbgc_OPERATOR(TOP_LL(p->op)->type) && TOP_LL(p->op)->type != COMMA &&
					!is_fbgc_OPERATOR(p->iter_prev->type) && p->iter_prev->next->type != COMMA &&
					TOP_LL(p->op)->type != LPARA && TOP_LL(p->op)->type != LBRACK && 
					TOP_LL(p->op)->type != IF && TOP_LL(p->op)->type != ELIF && TOP_LL(p->op)->type != WHILE
					&& TOP_LL(p->op)->type != RETURN
				){
				
				FBGC_LOGV(PARSER,"Inserting pop_top, iter_prev:%s, top:%s\n",lltp2str(p->iter_prev),lltp2str(TOP_LL(p->op)));
				//p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_base(POP_TOP));

			}
			if(TOP_LL(p->op)->type != COMMA  && p->iter_prev->next->type != COMMA)
				//p->statement_begin = p->iter_prev;
			*/

			break;
		}
		case FOR:{
			
			//Do not try to initialize for loop all the time, this might be the case since we call this function for rpara and newline
			if(_cast_llbase_as_lljumper(TOP_LL(p->op))->content->type == FOR_BEGIN){
				handle_statements(p);
				return;
			}
			
			FBGC_LOGV(PARSER,"FOR loop definition,p->iter_prev:%s\n",lltp2str(p->iter_prev));
			FBGC_LOGV(PARSER,"For loop content:%s\n",lltp2str(_cast_llbase_as_lljumper(TOP_LL(p->op))->content));
			FBGC_LOGV(PARSER,"Iterator previous:%s\n",lltp2str(p->iter_prev));
			
			if(p->iter_prev->type != ASSIGN){
				//This makes sure we have for i = x type of loop definition
				p->error_code = _FBGC_SYNTAX_ERROR;
				return;
			}

			//Ok now for loop looks like for i = expression or for ( i = expression)
			//top is FOR and top->next is jump 
			//the content of FOR holds where the FOR is defined
			//as we move along the list untill iter_prev we will hit assignment operator
			//and we will insert for_begin between expression and assigment operator. 
			//See the explanation about how for loops work in fbgc
			//@TODO prepare explanation!

			struct fbgc_ll_base * d = _cast_llbase_as_lljumper(TOP_LL(p->op))->content;
			while(d->next != p->iter_prev)
				d = d->next;
			
			_cast_llbase_as_lljumper(TOP_LL(p->op))->content = _insert_next_fbgc_ll(d,_new_fbgc_ll_base(FOR_BEGIN));
			//insert for_begin between expression and assignment and return for begin which now for object will hold to insert
			//itself when we see END 
			p->statement_begin = p->iter_prev;
			return;
		}

		case FUN_MAKE:{

			//Normally we expect fun(args) but functions also can be defined typing fun args so we need to make sure we satisfy each definition

			FBGC_LOGV(PARSER,"Stack top is FUN_MAKE, function make template!\n");


			struct fbgc_ll_base * fun_holder =  _cast_llbase_as_lljumper(TOP_LL(p->op))->content; 
			struct fbgc_object * fun_obj = _cast_llbase_as_llconstant(fun_holder)->content;

			//Is it possible this is not useful*
			if(is_fbgc_fun_object_defined(fun_obj)){

				handle_statements(p);
				//maybe function is already defined before
				return;
			}
			//struct fbgc_ll_base * fun_holder,struct fbgc_ll_base * arg_end
			//fun_holder,p->iter_prev
			//Do not call for zero args
			FBGC_LOGV(PARSER,"Function arguments starts(%s) & ends(%s):\n",lltp2str(fun_holder->next),lltp2str(p->iter_prev));
			if(cast_fbgc_object_as_fun(fun_obj)->no_arg != 0 && cast_fbgc_object_as_fun(fun_obj)->no_arg != 65){
				//For zero arg functions calling args may cause infinite loop
				//Just for the debug case nothing important
				FBGC_LOGV(PARSER,"%c\n",_print_fbgc_ll_code(fun_holder->next,p->iter_prev));
			}
			
			handle_function_args(p);
			if(p->error_code != _FBGC_NO_ERROR) return;

			//@TODO increase verbosity of warnings
			//now we can put a warning if this function is not assigned to something
			if(TOP_LL(p->op)->next->type != ASSIGN &&  TOP_LL(p->op)->next->type != COMMA){
				FBGC_LOGW("Function is not assigned.");
			}
			
			p->statement_begin = p->iter_prev;
			
			return;
		}
		case CLASS_MAKE:{
			FBGC_LOGD(PARSER,"Stack top is CLASS_MAKE, making class template!\n");

			struct fbgc_ll_base * cls_holder =  _cast_llbase_as_lljumper(TOP_LL(p->op))->content; 
			struct fbgc_object * cls_obj = _cast_llbase_as_llconstant(cls_holder)->content;

			//Maybe class defined before
			if(_cast_llbase_as_llconstant(TOP_LL(p->scope_list))->content == cls_obj){
				handle_statements(p);
				return;
			}

			FBGC_LOGV(PARSER,"Class Arg Start:%s\n",lltp2str(cls_holder->next));
			//_print_fbgc_ll_code(cls_holder->next,p->iter_prev);
			FBGC_LOGV(PARSER,"Class Arg End:%s\n",lltp2str(p->iter_prev));

			//Because of inheritance, we start scope of a class at the end of its definition 
			_push_front_fbgc_ll(p->scope_list,_new_fbgc_ll_constant(cls_obj));

			while(cls_holder != p->iter_prev){
				if(cls_holder->next->type == IDENTIFIER){
					cls_holder = cls_holder->next;
				}
				else if(cls_holder->next->type == COMMA){
					cls_holder->next = cls_holder->next->next;
					p->iter_prev = cls_holder;
				}
				else{
					//This is valid for zero argument
					p->error_code = _FBGC_SYNTAX_ERROR;
					return;
				}
			}
			cast_fbgc_object_as_class(cls_obj)->code = cls_holder;

			FBGC_LOGV(PARSER,"Class code :%s\n",lltp2str(cast_fbgc_object_as_class(cls_obj)->code));

			p->statement_begin = p->iter_prev;
			
			return;	
		}
		case IF:
		case ELIF:
		case WHILE:{
			//As other cases we first check did we define if/elif/while or not, if it is null then they are not defined so assign their
			//content to jump iter_prev
			if(_cast_llbase_as_lljumper(TOP_LL(p->op))->content == NULL){	
				FBGC_LOGV(PARSER,"Stack top is %s, conditional template!\n",lltp2str(TOP_LL(p->op)));
				_cast_llbase_as_lljumper(TOP_LL(p->op))->content = p->iter_prev;
				FBGC_LOGV(PARSER,"If shows %s\n",lltp2str(p->iter_prev->next));

				p->statement_begin = p->iter_prev;
			}
			else handle_statements(p);

			return;
		}
		default:{

			//So we have nothing in op-stack, only thing is left it could be list of items or just nuple e.g tuple with no entry ()
			//We need to check for two cases and either change comma to tuple or create nuple
							
			if(p->iter_prev->type == COMMA){
				FBGC_LOGV(PARSER,"State number is 2, changing COMMA to BUILD_TUPLE\n");
				p->iter_prev->type = BUILD_TUPLE;
			} 
			else if(p->gm == GM_ATOM){
				FBGC_LOGV(PARSER,"State number is 0, creating new object BUILD_TUPLE with 0 size\n");
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_opcode_int(BUILD_TUPLE,0));
			}
			//If nothing than it's a tuple!

			handle_statements(p);
			
			return;
		}
	}

}

static void handle_before_brackets(struct parser_packet *p){
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));

	//Same as handle paranthesis, this time we handle brackets 
	//There are three scenarios
	// - Matrix entry
	// - loading subsciprt e.g: x[1,2,3]
	// - assigning to subscript e.g: y[5,10] = 'fbgc'

	uint8_t	state = 0;
	if(p->iter_prev->type == COMMA) state = 2;
	else if(p->gm != GM_ATOM) state = 1;

	//For subscript assignments, just leave it assignment operator will handle this scenario	
	if(is_fbgc_ASSIGNMENT_OPERATOR(p->iter_prev->next->type)){
		if(state == 2){
			//p->iter_prev->type = 
		}
		return;
	}
	
	if(TOP_LL(p->op)->type == IDENTIFIER || TOP_LL(p->op)->type == LOAD_SUBSCRIPT){	
		FBGC_LOGV(PARSER,"Operator stack top load_local or global, this is a subscript call template!\n");

		
		if(state == 2){
			//comma already in the list, just change its type to load subscript and pop load subscript
			p->iter_prev->type = LOAD_SUBSCRIPT;
			POP_LL(p->op);
		}
		else if(state == 1){
			//there is no comma we need to put load subscript into main list
			p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_top_and_pop_front_fbgc_ll(p->op));
		}
		else{
			p->error_code = _FBGC_SYNTAX_ERROR;
			return;
			//cannot have entries like x[], empty subscripts
		}
	}
	else{
		if(state == 2) p->iter_prev->type = BUILD_MATRIX; 
		else{
			p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_opcode_int(BUILD_MATRIX,state==1));
		}		
	}
	
}

static void handle_before_braces(struct parser_packet *p){
	_FBGC_LOGD(PARSER,"\n==================================\nStack top holder:%s\n",lltp2str(TOP_LL(p->op)));
	struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p->op);
	assert(jump_obj->type == JUMP);


	struct fbgc_ll_base * iter = _cast_llbase_as_lljumper(jump_obj)->content;
	//FBGC_LOGE("iter type %s\n",lltp2str(iter));
	//FBGC_LOGE("piter type %s\n",lltp2str(p->iter));
	uint8_t count = 0;

	if(iter == p->iter_prev){
		p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_opcode_int(BUILD_MAP,0));
		return;
	}
	int i = 0;
	while(iter != p->iter_prev){
		++i;
		if(i>200) break;
		FBGC_LOGE("..iter type %s\n",lltp2str(iter));
		if(iter->next->type == CONSTANT){
			printf("inc count ...\n");
			++count;
		}
		else if(iter->next->type == COLON){
			printf("count :%d\n",count);
			if(count < 2) assert(0);
			count = 0;

			if(iter->next == p->iter_prev){
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_opcode_int(BUILD_MAP,1));
				iter->next = iter->next->next;
				break;
			}

			iter->next = iter->next->next;

			continue;
		}
		else if(iter->next->type == COMMA){
			//_cast_llbase_as_llopcode_int(iter->next)->content *= 2;
			iter->next->type = BUILD_MAP;
		}
		iter = iter->next;
	}

	if(TOP_LL(p->op)->type == CLASS_SELF){
		struct fbgc_ll_base * it = _top_and_pop_front_fbgc_ll(p->op);
		while(it->type != FUN_CALL){
			it = it->next;
			printf("it :%s\n",lltp2str(it));
			if(it == tail_fbgc_ll(p->op)){
				p->error_code = _FBGC_SYNTAX_ERROR;
				return;
			}
		}

		printf("hit the fun call :%s\n",lltp2str(it));
		it->type = KWFUN_CALL;
		
		struct fbgc_ll_base * jumper = _insert_next_fbgc_ll(it,_new_fbgc_ll_jumper_with_content(JUMP,p->iter_prev));
	}		
}



static uint8_t parser(struct parser_packet * p);

static inline struct parser_packet init_parser_packet(struct fbgc_object * field_obj, char * s){
	struct parser_packet p = {
		.head = _cast_llbase_as_ll( cast_fbgc_object_as_field(field_obj)->head ),
		.iter = p.head->base.next,
		.iter_prev = (struct fbgc_ll_base *)p.head, //note that p->iter_prev->next is p->iter, always this is the case!
		.op = _new_fbgc_ll(),
		.scope_list = _new_fbgc_ll(),
		.statement_begin = (struct fbgc_ll_base *)p.head,
		.line = NULL,
		.line_no = 1,
		.error_code = _FBGC_NO_ERROR,
		.gm = GM_NEWLINE,
	};

	_push_front_fbgc_ll(p.scope_list,_new_fbgc_ll_constant(field_obj));

	return p;
}



uint8_t parse_file(struct fbgc_object ** field_obj, const char * file_name){

	FBGC_LOGD(PARSER,"==========[PARSER]==========\n");
	FILE * fp = fopen(file_name,"r");
	
	if(fp == NULL){
		FBGC_LOGE("File %s could not be found\n",file_name);
		return 0;
	}



	//printf("Adding new file name %s\n",file_name );
	//add_variable_in_field_object(*field_obj,"__file__",new_fbgc_str_object(file_name));
	

	struct parser_packet p = init_parser_packet(*field_obj,NULL);

	while(fbgc_getline_from_file(&p.line,fp)){
		FBGC_LOGD(PARSER,"-------------------Parsing line :%ld-------------------\n",p.line_no);
		++p.line_no;
		if(p.line[0] == '#' || p.line[0] == '\0') {
			continue; //fast passing the comment
		}

		p.head->tail->next = p.iter_prev;
		regex_lexer(field_obj,p.line);
		p.iter = p.iter_prev->next;
		if(parser(&p) != _FBGC_NO_ERROR){
			break;
		}
	}
	fclose(fp);

	//make the linked list connection proper
	p.head->tail->next = p.iter_prev;

	if(p.error_code != _FBGC_NO_ERROR){
		cprintf(100,"error at line %d:%s\n",p.line_no,p.line);
		free(p.line);
		return 0;
	}
	if(!is_empty_fbgc_ll(p.op)){
		//Check the stack and give proper error
		if(is_fbgc_FUNCTIONABLE(get_fbgc_object_type(TOP_LL(p.op)))){
			FBGC_LOGE("Missing 'end' keyword, '%s' must be finished with 'end'\n",object_type_as_str(TOP_LL(p.op)));
			return 0;
		}

		cprintf(100,"Syntax error in line %d, Non-empty operator stack!\n",p.line_no-1);
		_print_fbgc_ll(p.op,"O");
		return 0;
	}

	FBGC_LOGV(PARSER,"==========================\nParser finished, defined symbols:");
	FBGC_LOGV(PARSER,"%c\n",print_fbgc_object(fbgc_symbols));
	FBGC_LOGV(PARSER,"Global field locals:");
	FBGC_LOGV(PARSER,"%c\n",print_field_object_locals(*field_obj));
	FBGC_LOGV(PARSER,"==========================\n");

	return 1;
}


uint8_t parse_string(struct fbgc_object ** field_obj, char * str ){

	struct parser_packet p = init_parser_packet(*field_obj,str);
	
	regex_lexer(field_obj,str);
	p.iter = p.iter_prev->next;
	if(parser(&p) != _FBGC_NO_ERROR){
		cprintf(100,"error at line %d:%s\n",p.line_no,p.line);
	}
	
	//make the linked list connection proper
	p.head->tail->next = p.iter_prev;

	if(!is_empty_fbgc_ll(p.op)){
		//Check the stack and give proper error
		if(is_fbgc_FUNCTIONABLE(get_fbgc_object_type(TOP_LL(p.op)))){
			FBGC_LOGE("Missing <END> keyword at %ld : <%s>\n",p.line_no,object_type_as_str(TOP_LL(p.op)));
			return 0;
		}

		cprintf(100,"Syntax error in line %d, Non-empty operator stack!\n",p.line_no);
		_print_fbgc_ll(p.op,"O");
		return 0;
	}

	FBGC_LOGV(PARSER,"==========================\nParser finished, defined symbols:");
	FBGC_LOGV(PARSER,"%c\n",print_fbgc_object(fbgc_symbols));
	FBGC_LOGV(PARSER,"Global field locals:");
	FBGC_LOGV(PARSER,"%c\n",print_field_object_locals(*field_obj));
	FBGC_LOGV(PARSER,"==========================\n");
	return 1;
}

#define PARSER_CHECK_ERROR(p_error_code)( {if(p_error_code != _FBGC_NO_ERROR) return p_error_code;} )

static uint8_t parser(struct parser_packet * p){


	#define current_scope _cast_llbase_as_llconstant(TOP_LL(p->scope_list))->content	
	
	
	while(p->iter != p->head->tail){
		FBGC_LOGD(PARSER,"%c",cprintf(011,"\n\n==========================={%s}===========================\n\n",lltp2str(p->iter)));
	switch(p->iter->type){
	case CONSTANT:{
		//! check the grammar and move forward, we don't push constants to stack
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);
		//At the end of switch-case there is a command p->iter = p->iter_prev->next
		//We always use p->iter_prev to make insertions etc, iter just a pointer to move in list
		FBGC_LOGV(PARSER,"%c:Content\n",print_fbgc_object(_cast_llbase_as_llconstant(p->iter)->content));
		p->iter_prev = p->iter;
		break;
	}
	case IDENTIFIER:{
		//First check the grammar
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		//Now there are different cases for identifiers. They can be CFUN objects, like print, input etc.
		//There can belong to fields,classes and functions we need to find to understand where the user defined them

		FBGC_LOGV(PARSER,"Current Scope for identifier:(%s)\n",objtp2str(current_scope));
		
		//Symbols are defined as cstr_obj in fbgc_symbols which is a tuple, now from relexer they come with a tag called location
		//Which basically tells us where this symbol is, we ask the symbol first 
		struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,get_ll_identifier_loc(p->iter));
		FBGC_LOGV(PARSER,"Symbol name as a cstring object:'%s'\n",content_fbgc_str_object(cstr_obj));
		FBGC_LOGV(PARSER,"prev:'%s'\n",lltp2str(p->iter_prev));
		
		//Now we got the symbol, we are gonna use it to identify is this a new definition of a variable or cfun or using pre defined variable

		struct fbgc_object * cf = NULL;

		/*if(p->kwargs_holder->type == FUN_CALL && p->iter->next->type == COLON ){
			//named args
			p->iter_prev->next = p->iter->next;
			p->iter_prev = _insert_next_fbgc_ll(p->iter_prev, _new_fbgc_ll_constant(cstr_obj));



			break;
		}*/


		if(TOP_LL(p->op)->type == DOT){
			//If we have a enty looks like 'x.y' , when we read x we don't know anything about the dot symbol
			//But when it comes to 'y', y is not a variable because it is an attribute of x, so we cannot define y as a variable
			//It is needed to stay there as c-str to use it in interpreter. At this stage we CANNOT tell "does x have y attribute?" the answer is we do not know
			//So it looks ugly checking from stack here but couldn't find any solution, we will check it and put c_str into the main list

			POP_LL(p->op); //pop the dot object, we do not need it
			//now get the name of instance
			p->iter_prev = _insert_next_fbgc_ll(p->iter_prev, _new_fbgc_ll_constant(cstr_obj));
			
			//Just change the flag of iter, below we will push this into stack
			set_id_flag_MEMBER(p->iter);
			//but we are using IDENTIFIER type and ask for its flags, I left it in order not to increase opcode number
			//However it will affect our performance
			//@TODO make a new opcode for member
		}
		else if(TOP_LL(p->op)->type == CLASS_SELF){
			if(current_scope->type == FUN && _cast_llbase_as_llconstant(TOP_LL(p->scope_list)->next)->content->type == CLASS){
				//So we are gonna change .x entry to self.x which self is our class object previously defined before function
				POP_LL(p->op); //first pop class_self we do not need it
				//struct fbgc_object * cls_obj = _cast_llbase_as_llconstant(TOP_LL(p->scope_list)->next)->content;
				
				struct fbgc_ll_base * tmp = _new_fbgc_ll_identifier(0);
				set_id_flag_LOCAL(tmp);
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,tmp);
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev, _new_fbgc_ll_constant(cstr_obj));

				//Just change the flag of iter, below we will push this into stack
				set_id_flag_MEMBER(p->iter); //we are gonna use its name
			}
			else{
				return p->error_code = _FBGC_SYNTAX_ERROR;
			}
		}
		else if( ( cf = new_cfun_object_from_str(current_field,content_fbgc_str_object(cstr_obj)) ) != NULL){
			//Identifier can be cfun object, if it is not above condition returns null
			FBGC_LOGD(PARSER,"Symbol is a CFUN, inserting cfun into list\n");
			//We don't need iter anymore skip it and insert cfun
			p->iter_prev = _insert_fbgc_ll(p->iter_prev,p->iter,_new_fbgc_ll_constant(cf));
			break;//let's break here because we deleted p->iter, we don't want to push it into op-stack
		}
		else{

			//This do-while(0) loop is used instead of using goto, because if scope is fun we do not want to go through below field variable
			//registeration, the whole point is reducing the code size not when we break, we jump at the end of do-while loop so which is what we wanted
			ssize_t where = -1;
			do{
			struct fbgc_object ** current_locals;

			if(current_scope->type == FUN){
				//function have local tuple that is defined when FUN_MAKE first creates this function			
				current_locals = &(_cast_llbase_as_llconstant(cast_fbgc_object_as_fun(current_scope)->code)->content);
				
				set_id_flag_LOCAL(p->iter); //make it local, if it is not it will be changed

				//Ask for this pointer, notice that this is different than array object because tuple only holds fbgc_object pointers
				//So we are basically asking does your contents contain this ?
				where = index_fbgc_tuple_object(*current_locals,cstr_obj);
				if(where != -1){
					//Most likely we will call our previous definitions
					//We already set flag to local, so just break do-while loop to go at the end of case IDENTIFIER
					//However first check that are we still reading function args ? Because in that case we MUST NOT find our previous
					//args so set an error
					if(!is_fbgc_fun_object_defined(current_scope)){
						FBGC_LOGE("Function arguments must have unique names\n");
						return p->error_code = _FBGC_SYNTAX_ERROR;
					}


					break;
				}
				else{ 
					if(p->iter->next->type == ASSIGN  || (!is_fbgc_fun_object_defined(current_scope) && TOP_LL(p->op)->type != ASSIGN)){
					//Now ask for is this a new definition ? if it is not, it will leave as where = -1
			
						FBGC_LOGD(PARSER,"First time defining IDENTIFIER inside function\n");

						push_back_fbgc_tuple_object(*current_locals,cstr_obj);
						where = size_fbgc_tuple_object(*current_locals)-1;
						//_cast_llbase_as_llconstant(cast_fbgc_object_as_fun(current_scope)->code)->content = current_locals;
						
						FBGC_LOGD(PARSER,"Function local tuple:");
						FBGC_LOGD(PARSER,"%c\n",print_fbgc_object(*current_locals));
						break;//Break do-while loop to go to at the end of case IDENTIFIER
					}
				}
				
				FBGC_LOGV(PARSER,"Identifier could be global, searching in global fields.\n");
				//Now change current locals to field object locals because we do not allow to get variables defined in classes, 
				//only field  is allowed

				current_locals = &(cast_fbgc_object_as_field(current_field)->locals);
				set_id_flag_GLOBAL(p->iter);

			}else{
				//If we are here we sure that current scope either field or class no need to check
				//get the local array of field/class objects

				if(current_scope->type == FIELD){
					set_id_flag_GLOBAL(p->iter);
					current_locals = &(cast_fbgc_object_as_field(current_scope)->locals);
				}
				else{
					set_id_flag_CLASS(p->iter);
					current_locals = &(cast_fbgc_object_as_class(current_scope)->locals);
				}

				
			}
			

			while(1){
				//@TODO instead of for loop can we use c++ like iterators here ?
				//Maybe this variable is defined before, lets check first
				for(size_t i = 0; i<size_fbgc_array_object(*current_locals); ++i){
					struct fbgc_identifier * temp_id = (struct fbgc_identifier *) at_fbgc_array_object(*current_locals,i);
					if(temp_id->name == cstr_obj){  //Notice that this is not a string comparison because we use same symbols for each occurence of a symbol
						where = i;
						break;
					} 
				}

				//If it is not a class definition then change the locals and search it again maybe it is defined in global scope
				//This is why we have a loop here
				//if it is NOT then always breaks the loop so it is going to be defined.
				if(where == -1 && current_scope->type == CLASS && p->iter->next->type != ASSIGN && is_id_flag_GLOBAL(p->iter) == 0){

					FBGC_LOGD(PARSER,"Could not find in CLASS, assuming it is defined in global scope\n");
					current_locals = &(cast_fbgc_object_as_field(current_field)->locals);
					set_id_flag_GLOBAL(p->iter);
				}
				else break;
			}

			//New definition of our symbol
			if(where == -1) {
				FBGC_LOGD(PARSER,"Creating a variable inside a field obj\n");
				struct fbgc_identifier temp_id;
				temp_id.name = cstr_obj; temp_id.content = NULL;
				//Now array will make copy of this id object in our array so we can pass its address
				push_back_fbgc_array_object(*current_locals,&temp_id);
				//Now we now the location in globals
				where = size_fbgc_array_object(*current_locals)-1;

				//else if(current_scope->type == FIELD) cast_fbgc_object_as_field(current_scope)->locals = current_locals;
				//else cast_fbgc_object_as_class(current_scope)->locals = current_locals;
				
				FBGC_LOGD(PARSER,"Symbol is created @ [%ld]\n",where);
			}
			else{
				FBGC_LOGD(PARSER,"Symbol is already defined @ [%ld]\n",where);
			}

			}while(0);
			
			//Now current identifier object needs know the location of its definition
			set_ll_identifier_loc(p->iter,where);
		}

		//Connect the list and insert current identifier
		p->iter_prev->next = p->iter->next;
		//push the identifier into op stack
		_push_front_fbgc_ll(p->op,p->iter);
		break;
	}
	case END:{

		//@TODO empty statement checking seem lots of work, can we reduce to one code ?
		//Check the grammar is it ok ?
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);
		p->iter_prev->next = p->iter->next; //skip END
		
		//Get top_type and top object, also pop it we don't need them in stack
		//They are going to main list
		struct fbgc_ll_base * top_obj = _top_and_pop_front_fbgc_ll(p->op);
		FBGC_LOGV(PARSER,"Top type :%s\n",lltp2str(top_obj));
		FBGC_LOGV(PARSER,"Iter prev type :%s\n",lltp2str(p->iter_prev));
		switch(top_obj->type){
			case IF:
			case ELIF:{
				//Take IF/ELIF object from op-stack
				//ELIF and IF are not different when inserting them into list, so no need to make another condition
				//Here top_obj is either IF or ELIF
				//Recal that if_obj holds its location to insert correctly
				//content->nect is where we are gonna insert IF
				//if_obj->next = _cast_llbase_as_lljumper(if_obj)->content->next;
				//_cast_llbase_as_lljumper(if_obj)->content->next = if_obj;
				//Following function does the above operations

				if(_cast_llbase_as_lljumper(top_obj)->content == p->iter_prev){
					FBGC_LOGE("Conditional statements cannot be empty\n");
					//if or elif statements cannot be empty
					return p->error_code = _FBGC_SYNTAX_ERROR;
				}

				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(top_obj)->content,top_obj);
				//Now if object know where to jump if condition is not satisfied
				_cast_llbase_as_lljumper(top_obj)->content = p->iter_prev; //now assign where to jump
				break;
			}
			case WHILE:{
				if(_cast_llbase_as_lljumper(top_obj)->content == p->iter_prev){
					//while statements cannot be empty
					FBGC_LOGE("Conditional statements cannot be empty\n");
					return p->error_code = _FBGC_SYNTAX_ERROR;
				}
				//now insert while in its place,
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(top_obj)->content,top_obj);

				//Now also insert jump object back to END token which is iter_prev
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p->op);
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,jump_obj);

				//While object is gonna jump to next of its content for unsatisfied conditions
				_cast_llbase_as_lljumper(top_obj)->content = jump_obj;


				//Tell statement_begin we finished while statement
				p->statement_begin = p->iter_prev;

				break;
			}
			case FOR:{

				//Now insert for in its place
				_insert_next_fbgc_ll(_cast_llbase_as_lljumper(top_obj)->content,top_obj);

				//Now also insert jump object back to END token which is iter_prev
				struct fbgc_ll_base * jump_obj = _top_and_pop_front_fbgc_ll(p->op);
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,jump_obj);

				//for content holds prev location of for, jump need to know where to jump to make loop
				_cast_llbase_as_lljumper(jump_obj)->content = _cast_llbase_as_lljumper(top_obj)->content;	
				
				//For object is gonna jump to next of its jump_obj to finish the loop
				_cast_llbase_as_lljumper(top_obj)->content = jump_obj;


				//Tell statement begin we finished for statement
				p->statement_begin = p->iter_prev;

				break;

			}
			case FUN_MAKE:{
				//Check the function is empty or not, we need to handle this

				if(is_constant_and_token(p->iter_prev,FUN) ){
					FBGC_LOGE("Functions cannot be empty\n");
					return p->error_code = _FBGC_SYNTAX_ERROR;
				}

				//Now we need to find which functions are we holding
				struct fbgc_ll_base * fun_holder = _cast_llbase_as_lljumper(top_obj)->content;
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
				//insert next to p->iter and iterate it to its next
				
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_constant(__fbgc_nil));
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_base(RETURN));
				
				//Now we are finishing fun_obj->code
				p->iter_prev->next = NULL; //last token in function code shows NULL
				//Now finished, the only reference from previous tokens are from fun_obj->code
				p->iter_prev = fun_holder;

				POP_LL(p->scope_list); //remove this function from scope list
				//eat END we no longer need it
				p->iter_prev->next = p->iter->next;
				
				if(cast_fbgc_object_as_fun(fun_obj)->flag_default_args){
					struct fbgc_ll_base * start_def_arg = TOP_LL(p->op);
					struct fbgc_ll_base * end_def_arg = TOP_LL(p->op);
					while(end_def_arg->type != BUILD_FUN_DEFAULT_ARGS){
						end_def_arg = end_def_arg->next;
					}
					cprintf(100,"fun holder next :%s\n",lltp2str(p->iter_prev->next));
					_cast_llbase_as_ll(p->op)->base.next = end_def_arg->next;

					end_def_arg->next = p->iter_prev->next;
					p->iter_prev->next = start_def_arg;
					p->iter_prev = end_def_arg;
				}


				FBGC_LOGV(PARSER,"Created function code:");
				FBGC_LOGV(PARSER,"%c\n",_print_fbgc_ll_code(cast_fbgc_object_as_fun(fun_obj)->code,NULL));

				
				break;

			}
			case CLASS_MAKE:{
				//finishing the class definition

				if(is_constant_and_token(p->iter_prev,CLASS)){
					FBGC_LOGE("Classes cannot be empty\n");
					return p->error_code = _FBGC_SYNTAX_ERROR;
				}

				struct fbgc_ll_base * cls_holder = _cast_llbase_as_lljumper(top_obj)->content;
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
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_constant(cls_obj));
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_base(RETURN));
				
				//Now finish the class code
				p->iter_prev->next = NULL;

				//Let's print this class object to see its code
				FBGC_LOGV(PARSER,"Created class :");
				FBGC_LOGV(PARSER,"%c\n",_print_fbgc_ll_code(cast_fbgc_object_as_class(cls_obj)->code,NULL));

				
				POP_LL(p->scope_list); //remove class from scope list

				//We need to put class_call for the first time class definitions in order to run their code
				//Start from cls_holder
				p->iter_prev = _insert_next_fbgc_ll(cls_holder,_new_fbgc_ll_opcode_int(CLASS_CALL,arg_no));
				p->iter_prev->next = p->iter->next;

				break;
			}
			case JUMP:{

				FBGC_LOGV(PARSER,"Inserting jump into list\n");
				for(;;){
					_insert_next_fbgc_ll(_cast_llbase_as_lljumper(top_obj)->content,top_obj);
					//New location to jump
					_cast_llbase_as_lljumper(top_obj)->content = p->iter_prev;
					if(TOP_LL(p->op)->type == JUMP){
						top_obj = _top_and_pop_front_fbgc_ll(p->op);
					}
					else break;
				}
				break;
			}
			default:{
				FBGC_LOGE("END keyword is not required here\n");
				return p->error_code = _FBGC_SYNTAX_ERROR;
				
			}
		}

		//Is there any jump object left in the stack ? make their connections
		/* Why multiple jumps ?
		while(0 && top_obj->type == JUMP){
			FBGC_LOGV(PARSER,"Inserting jump into list\n");
			top_obj->next = _cast_llbase_as_lljumper(top_obj)->content->next;
			_cast_llbase_as_lljumper(top_obj)->content->next = top_obj;
			_cast_llbase_as_lljumper(top_obj)->content = p->iter_prev;
			top_obj = _top_and_pop_front_fbgc_ll(p->op);
		}*/

		break;			
	}
	case FUN_MAKE:{
		//FUN_MAKE is a lljumper type holder, it will hold the function object
		//while we are seeing till the END token.


		//Let's not allow closures right now, later we might change this property
		if(current_scope->type == FUN){
			FBGC_LOGE("Nested functions are not allowed\n");
			return p->error_code = _FBGC_SYNTAX_ERROR;
		}


		//Check the grammar is it ok ?
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		//Create a new function object and holder for this object
		//Holder must be a constant_holder because it has a fbgc_object * type pointer
		struct fbgc_object * fun_obj = new_fbgc_fun_object();
		struct fbgc_ll_base * fun_holder = _new_fbgc_ll_constant(fun_obj);

		//Now we do not need iter at this point, later we will push it into our op-stack
		//This function takes 3 args and makes the following operatiobs

		// 1) fun_holder->next = p->iter->next;
		// 2) p->iter_prev->next = fun_holder;
		// 3) p->iter_prev = fun_holder;
		//For the final assignment it returns fun_holder so we update our iter_prev
		p->iter_prev = _insert_fbgc_ll(p->iter_prev,p->iter,fun_holder);

		//Now FUN_MAKE object shows fun_holder and we will push it into op-stack
		_cast_llbase_as_lljumper(p->iter)->content = fun_holder;
		_push_front_fbgc_ll(p->op,p->iter);
		//When we have END operator we will ask FUN_MAKE object to learn which fun obj is this.

		//When we create functions we need a local identifier holder and this must be a linear array
		//because when a new identifier is read it just becomes local<n>, and they are hold by the following tuple.
		//here we use tuple object to hold each identifier, when we finish the definition of our function
		//we no longer need to hold this tuple. That's why code pointer is used for this purpose.
		//Normally code should point code but at this point it just holds one type 
		cast_fbgc_object_as_fun(fun_obj)->code = _new_fbgc_ll_constant(new_fbgc_tuple_object(0));

		if(current_scope->type == CLASS){
			struct fbgc_object ** tp =&(_cast_llbase_as_llconstant(cast_fbgc_object_as_fun(fun_obj)->code)->content);
			push_back_fbgc_tuple_object(*tp,new_fbgc_str_object("."));
		}


		//Now update the scope we need it for local identifiers
		//This scope is opened before reading arguments and it is important to note that arguments are assumed local variable
		//First argument is local<0>, the second is local<1> etc. When we call this function in interpreter we do not change the 
		//location of args, we say that local<0> starts at first argument and so on.
		_push_front_fbgc_ll(p->scope_list,_new_fbgc_ll_constant(fun_obj));
		



		FBGC_LOGV(PARSER,"New fun_obj is inserted into list. FUN_MAKE goes to op-stack. Scope updated\n");
		break;		
	}
	case CLASS_MAKE:{
		//CLASS_MAKE is a lljumper type holder, it will hold a class object
		//while we are seeking till the END token.
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		//Create a new class_object and its holder to insert into the list
		struct fbgc_object * cls_obj = new_fbgc_class_object();
		struct fbgc_ll_base * cls_holder = _new_fbgc_ll_constant(cls_obj);

		//Insert cls_holder and remove iter from the list
		p->iter_prev = _insert_fbgc_ll(p->iter_prev,p->iter,cls_holder);

		//Now class_make shows our class object, push it into the stack
		_cast_llbase_as_lljumper(p->iter)->content = cls_holder;
		_push_front_fbgc_ll(p->op,p->iter);
		
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

		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		//When we read ELIF token we expect previous ELIF and IF tokens in op-stack
		//If either of those is not satisfied this is an error, put syntax error
		if(TOP_LL(p->op)->type != IF && TOP_LL(p->op)->type != ELIF){
			FBGC_LOGE("%s cannot be defined without 'IF'\n",lltp2str(p->iter));
			p->error_code = _FBGC_SYNTAX_ERROR;
			return _FBGC_SYNTAX_ERROR;
		}

		//Now stack op is if or elif object, which is a jumper type holder
		//Get the top object and pop from op stack, we will insert it into the main list
		struct fbgc_ll_base * if_obj = _top_and_pop_front_fbgc_ll(p->op);

		//The content of if object knows where are we gonna insert it
		//Basically it holds its location. It must be insterted next of the content
		//The following function does the following operations
		// 1) if_obj->next = _cast_llbase_as_lljumper(if_obj)->content->next;
		// 2) _cast_llbase_as_lljumper(if_obj)->content->next = if_obj;
		_insert_next_fbgc_ll(_cast_llbase_as_lljumper(if_obj)->content,if_obj);

		//Now get a jump object because when a condition is satisfied for the previous if/elif 
		//There must be a jump at the end of their scope to finish conditional structure
		_cast_llbase_as_lljumper(if_obj)->content = _new_fbgc_ll_jumper_with_content(JUMP, p->iter_prev);

		//struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
		//_cast_llbase_as_lljumper(jump_obj)->content = p->iter_prev;
		//_cast_llbase_as_lljumper(if_obj)->content = jump_obj;

		//Push the jump object into op-stack
		_push_front_fbgc_ll(p->op,_cast_llbase_as_lljumper(if_obj)->content);
		//Remove iter from the list
		p->iter_prev->next = p->iter->next;

		//if iter is elif we need to see it into our stack but if it is else we don't need it
		if(p->iter->type == ELIF){
			FBGC_LOGV(PARSER,"ELIF is pushed into op-stack \n");
			_push_front_fbgc_ll(p->op,p->iter);		
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
		
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		
		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
		if(TOP_LL(p->op)->type != WHILE){
			//Jump now holds token before the while
			_cast_llbase_as_lljumper(jump_obj)->content = p->iter_prev;
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
			
			//_cast_llbase_as_lljumper(jump_obj)->content = TOP_LL(p->op);
			assert(0);
		}
		//Push jump object
		_push_front_fbgc_ll(p->op,jump_obj);
		//Take the current iterator from the list, eating while to push into op-stack
		p->iter_prev->next = p->iter->next;
		//Insert WHILE object into op-stack
		_push_front_fbgc_ll(p->op,p->iter);	
		FBGC_LOGV(PARSER,"WHILE is finished. JUMP and WHILE objects are pushed into stack \n");
		break;
	}
	case FOR:{
		//@TODO FOR and WHILE have the same structure, merge them

		//FOR is a jumper type holder
		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		//For needs to know its current location to insert later
		_cast_llbase_as_lljumper(p->iter)->content = p->iter_prev;

		struct fbgc_ll_base * jump_obj = _new_fbgc_ll_jumper(JUMP);
		_cast_llbase_as_lljumper(jump_obj)->content = p->iter_prev;
		//Push jump holder
		_push_front_fbgc_ll(p->op,jump_obj);
		//take FOR from main list 
		p->iter_prev->next = p->iter->next;	
		//push FOR into main list
		_push_front_fbgc_ll(p->op,p->iter);
		FBGC_LOGV(PARSER,"FOR is finished. JUMP and FOR objects are pushed into stack \n");	
		break;
	}			
	case BREAK:
	case CONT:{
		//Break and Cont both require while or for loop in op-stack
		//in order to find one we need to iterate through the stack, if we cannot find then this is a syntax error

		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		struct fbgc_ll_base * op_top = TOP_LL(p->op);
		for(;;){
			//Ok now move in op-stack
			if(op_top->type == WHILE || op_top->type == FOR){
				FBGC_LOGV(PARSER,"%s found %s in op-stack \n",lltp2str(p->iter),lltp2str(op_top));
				//There is a loop object
				if(p->iter->type == CONT){
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
				_cast_llbase_as_lljumper(p->iter)->content = op_top;
				break;
			}
			op_top = op_top->next;

			if(op_top == _cast_llbase_as_ll(p->op)->tail){
				//We are at the end of list and we found nothing so this is an error, no loop object before CONT/BREAK
				FBGC_LOGE("%s is not inside a loop \n",lltp2str(p->iter));
				p->error_code = _FBGC_SYNTAX_ERROR;
				return _FBGC_SYNTAX_ERROR;
			}
		}
		//Finally everything is ok, we found
		//Just iterate in the main list, we are not gonna put this token in op-stack;
		p->iter_prev = p->iter;
		FBGC_LOGV(PARSER,"Moving in the list %s stays \n",lltp2str(p->iter));

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
		//Operator part is almost the main part of the Shunting-Yard Algorithm.
		//The other parts are somehow merged to work with this part. The main idea is checking top of the op-stack and if precedence
		//of the top is bigger than the incoming operator we must put top operator to the main list, this will continue until we see
		//lower precedence (or an empty stack) in our comparisons

		//Take the p->op object from main list and connect previous one to the next one 
		//[H]->[2]->[+]->[3] => [H]->[2]->[3], now p->iter holds the operator, p->iter->next is [3] but we will change that too 
		//     p^	i^					
		//We either push this operator in op-stack or remove it, either way we need to take it from our main list
		p->iter_prev->next = p->iter->next;
		gm_convert_left(p);

		while( is_not_empty_fbgc_ll(p->op) && compare_operators(get_fbgc_object_type(TOP_LL(p->op)),p->iter->type) ){
			
			//first check the grammar
			gm_seek_right(p); PARSER_CHECK_ERROR(p->error_code);

			#define is_pushable_in_main(x)(!is_fbgc_PARA(x) && x != LEN)
			
			if(is_pushable_in_main(get_fbgc_object_type(TOP_LL(p->op)))){
				FBGC_LOGV(PARSER,"Top is pushable in main, pushing\n");
				//Now if the above condition is satisfied we are ready to move this object into main list
				//This function insert top of op-stack into current iter position and updates iter_prev
				//(move one right through the list)
				p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_top_and_pop_front_fbgc_ll(p->op));
			}
			else{
				//In this case top is not pushable to main list, so we need to handle it breaking the loop
				//Why do we have this ? 
				//In order to construct tuples, matrices, function calls etc. we need a handle mechanism but when I wrote this part
				//I assumed it won't be something so complex hence adding up new features led this type of handling mechanism
				//not pushable things are just paranthesis and len operator which will be pushed by hand in main list

				FBGC_LOGV(PARSER,"%c",cprintf(010,"Top object(%s) is not pushable to main list\n",lltp2str(TOP_LL(p->op))));
				break;
			}
			
			FBGC_LOGV(PARSER,"%c",cprintf(001,"Pushing to main list in while loop->.\n"));
			FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p->head,"Main"));
			FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p->op,"Op"));
			FBGC_LOGV(PARSER,"[GM]:{%s}\n",gm2str(p->gm));
		}

		fbgc_grammar gm_prev = p->gm;

		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);

		/*
			print((1,2,3)) gibi durumlarda p->iter_prev:cfun ve stacktop:lpara olduğu için
			tuple kurmak yerine function call atıyor. Bu hatayı düzeltmek için grammarde ne olduğunu bilmek gerekir
			Bu tip durumların düzeltilmesi gerekli.
		*/

		FBGC_LOGV(PARSER,"++++++++++++++++++++++\n");
		FBGC_LOGV(PARSER,"p->iter is %s, p->iter_prev %s\n",lltp2str(p->iter),lltp2str(p->iter_prev));

		if(p->iter->type == LPARA){
			// For left paranthesis we could have the following entries
			// 	("fbgc") : just entry in para
			// 	("fbgc",1,4,5) : tuple in para
			//	x(arg1,arg2) : function call
			// x[0](arg1,arg2) : function call from subscript
			// print((1,2,3)) : function call and tuple -> this one is tricky and gives problem because we cannot understand
			//without checking grammar, I did not want to add checking grammar in parser yet could not run away from it.
			//We are first checking grammar to handle lpara condition if it is not ID or Matrıx which are designed for adding function call
			//in top stack later handle paranthesis function will check top of the stack after lpara and see what we want to 

			if((gm_prev == GM_ID || gm_prev == GM_MATRIX) && 
				(p->iter_prev->type == IDENTIFIER || p->iter_prev->type == LOAD_SUBSCRIPT || is_constant_and_token(p->iter_prev,CFUN))) {
				
				//assume typical function call
				_push_front_fbgc_ll(p->op,_new_fbgc_ll_opcode_int(FUN_CALL,0));
				
				//p->current_state = FUN_CALL;

				//So user want so call member function we will push fun_call with 1 arg because self is also an argument for member methods		
				if(p->iter_prev->type == IDENTIFIER && is_id_flag_MEMBER(p->iter_prev)){
					_FBGC_LOGD(PARSER,"Setting id flag to member\n");
					set_id_flag_MEMBER_METHOD(p->iter_prev);
					//increase arg number of the fun_call that we pushed two lines ago
					++_cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;
				}		
			}

			//Do not put break, we want to push lpara in top stack
		}
		else if(p->iter->type == LBRACK){
			//Same as lpara, there are two cases
			// x[1,2,3]  : subscripts
			// x(1,2)[3] : subscript from return type of a function which led iter_prev as fun_call so we check two cases here
			if(p->iter_prev->type == IDENTIFIER || p->iter_prev->type == FUN_CALL){
				//If there is a list which includes comma, type of the comma will be changed to load_subscript
				//if it is not it means it just one entry such as x[10], so it is gonna insert this top object to main list
				//that is why we begin with 1 arg in order not to change later
				_push_front_fbgc_ll(p->op,_new_fbgc_ll_opcode_int(LOAD_SUBSCRIPT,1));
			}
			//Do not put break, we want to push lpara in top stack
		}
		else if(p->iter->type == SEMICOLON && get_fbgc_object_type(TOP_LL(p->op)) == LBRACK){
			//@TODO
			//can we at least check basic entries such as [1,2;3], it is not meaningful to check this during runtime

			if(p->iter_prev->type == COMMA) p->iter_prev->type = ROW; 
			else if(p->iter_prev->type != ROW) p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_new_fbgc_ll_opcode_int(ROW,1));
			break;
		}		
		else if(p->iter->type == RPARA || p->iter->type == NEWLINE || p->iter->type == SEMICOLON){

			//These two iter type lead the same functions below
			//each time a line is finished we need to figure it out what it was by using newline/rpara/semicolon
			//handle paranthesis function will decide which action to make

			//Now evertyhing pop lpara from stack
			if(TOP_LL(p->op)->type == LPARA) _pop_front_fbgc_ll(p->op);

			if(p->iter->type == NEWLINE || p->iter->type == SEMICOLON) p->gm = GM_NEWLINE;

			//Before calling below function lets check is this statement assigned to somewhere ?
			//Only we check end of the statements e.g we expect newline and semicolon not rpara
			
			handle_before_paranthesis(p);
			PARSER_CHECK_ERROR(p->error_code);

			if(p->iter_prev->type == BUILD_TUPLE){
				if((p->iter->type == NEWLINE || p->iter->type == SEMICOLON) && !is_empty_fbgc_ll(p->op)){
					//Now fool the loop and check newline state again,
					p->iter_prev->next = p->iter;
					p->iter = p->iter_prev;
					p->gm = GM_EXPRESSION;
				}
			}
			break;
		}		
		else if(p->iter->type == COMMA){

			if(TOP_LL(p->op)->type == COMMA){
				++_cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;
			}
			else {
				//So if we have an entry such as 1,2,3,4 without lpara, we assume these are tuple entries
				//basically we will add lpara and handle_paranthesis function will assume that these are nothing but tuples and pop lpara
				if(get_fbgc_object_type(TOP_LL(p->op)) != LPARA && get_fbgc_object_type(TOP_LL(p->op)) != LBRACK 
					&&get_fbgc_object_type(TOP_LL(p->op)) != LBRACE){
					if(current_scope->type == FUN && is_fbgc_fun_object_defined(current_scope) == 0){
						;
					}
					else
						_push_front_fbgc_ll(p->op,_new_fbgc_ll_base(LPARA));
				}
				// comma with 2 means 2 entry
				_push_front_fbgc_ll(p->op, _new_fbgc_ll_opcode_int(COMMA,2));
			}
			//@TODO the problem here is comma is base type and we here change its type to opcode_int which contains one int arg
			//When we deal with garbage collection it might be confusing so better to change it
			//We cannot make every comma opcode_int because it requires memory for each comma entry and assuming we will read lots of commas
			//we are just counting them and increasing here

			//Do not push current comma, we increased counter
			break;
		}
		else if(p->iter->type == LBRACE){
			_push_front_fbgc_ll(p->op,_new_fbgc_ll_jumper_with_content(JUMP,p->iter_prev));
		}
		else if(p->iter->type == RBRACE){
			if(TOP_LL(p->op)->type == LBRACE){
				_pop_front_fbgc_ll(p->op);
				handle_before_braces(p);
				PARSER_CHECK_ERROR(p->error_code);
			}
			else assert(0);
			break;
		}
		else if( p->iter->type == RBRACK){
			
			if(TOP_LL(p->op)->type == COMMA){
				//top->next->next is load_subscript, change its content
				_cast_llbase_as_llopcode_int(TOP_LL(p->op)->next->next)->content = _cast_llbase_as_llopcode_int(TOP_LL(p->op))->content;
				_pop_front_fbgc_ll(p->op);
			}
			
			if(TOP_LL(p->op)->type == LBRACK){
				_pop_front_fbgc_ll(p->op);
			}
			else assert(0);
			handle_before_brackets(p);
			PARSER_CHECK_ERROR(p->error_code);

			break;
		}
		else if(p->iter->type == PIPE && get_fbgc_object_type(TOP_LL(p->op)) == LEN){
			p->iter_prev = _insert_next_fbgc_ll(p->iter_prev,_top_and_pop_front_fbgc_ll(p->op));
			break;
		}
		
		_push_front_fbgc_ll(p->op,p->iter);
		
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

		//@TODO
		//The problem here is we change identifier type to assignment operator, hence we lose some extra bytes while creating assignment
		//operators. In order to reduce memory size, there must be two types

		gm_seek_left(p); PARSER_CHECK_ERROR(p->error_code);
		
		//There is a problem with assignment operator and grammar rules, in order not to blow up below code we need to check
		//by hand top value types must be identifier or load subscript
		/* Now grammar checks these 
		if(TOP_LL(p->op)->type != IDENTIFIER && TOP_LL(p->op)->type != LOAD_SUBSCRIPT){
			p->error_code = _FBGC_SYNTAX_ERROR;
			goto PARSER_ERROR_LABEL;
		}
		*/

		if(current_scope->type == FUN && !is_fbgc_fun_object_defined(current_scope)){
			//is this default argument entry ?
			//User tries to assign in function definition..
			
			return p->error_code = _FBGC_SYNTAX_ERROR;
			
		}

		//In order to support entries 'x = y = 1', we need to set a flag for identifier 'y' to push itself to the stack again
		//Since we just parse one time our list we cannot know whether to insert assigned variable one time. Instead we tell second variable to push itself
		if(TOP_LL(p->op)->next->type == ASSIGN|| TOP_LL(p->op)->next->type == LPARA || TOP_LL(p->op)->next->type == COMMA){
			// || TOP_LL(p->op)->next->type == LPARA || TOP_LL(p->op)->next->type == COMMA
			FBGC_LOGV(PARSER,"Opening push itself flag\n");

			if(TOP_LL(p->op)->next->next->type != FOR){
				//set_id_flag_PUSH_ITSELF(TOP_LL(p->op));
			}
		}

		//Eat current assignment operator
		p->iter_prev->next = p->iter->next;

		if(TOP_LL(p->op)->type == IDENTIFIER){
			FBGC_LOGV(PARSER,"Stack top is [%x]ID<%d>\n",get_ll_identifier_flag(TOP_LL(p->op)),get_ll_identifier_loc(TOP_LL(p->op)));
			TOP_LL(p->op)->type = p->iter->type;				
		}
		else if(TOP_LL(p->op)->type == LOAD_SUBSCRIPT){
			FBGC_LOGV(PARSER,"Stack top is load subscript, converting to assign\n");
			TOP_LL(p->op)->type = ASSIGN_SUBSCRIPT;
		}
		
		break;
	}
	default:{

		FBGC_LOGE("Undefined object type[%s] in parser\n",lltp2str(p->iter));
		assert(0);
	}
	}

	p->iter = p->iter_prev->next;
	FBGC_LOGD(PARSER,"%c\n",cprintf(010,"------------------------------------"));
	FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p->head,"Main"));
	FBGC_LOGV(PARSER,"%c",_print_fbgc_ll((struct fbgc_ll_base *)p->op,"Op"));
	FBGC_LOGV(PARSER,"[GM]:{%s}\n",gm2str(p->gm));
	FBGC_LOGD(PARSER,"%c\n",cprintf(010,"------------------------------------"));

	}

	return _FBGC_NO_ERROR;
}


//Kalan kısım 
// precedence comparisona bakılacak
// f(1,2) gibi durumlarda stacktan pop yapılacak
// assignmenta tekrar bakılabilir
// her durum için test yaz + dot ile çizdir
