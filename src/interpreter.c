#include "fbgc.h"

/*
uint8_t old_interpreter(struct fbgc_object ** field_obj){

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"==========[INTERPRETER]==========\n");
	#endif


	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );

	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head;
	struct fbgc_object * stack = new_fbgc_ll_object(STACK);
	


	for(int i = 0; (iter != head->tail); i++){

		fbgc_token type = get_fbgc_object_type(iter);

		#ifdef INTERPRETER_DEBUG
		cprintf(010,"################ [%d] = {%s} ########################\n",i,object_type_as_str(iter));
		#endif

		// print_fbgc_memory_block();

		if(is_fbgc_ATOM(type) || type == REFERENCE){
			iter_prev->next = iter->next;
			stack = push_front_fbgc_ll_object(stack,iter);
		}
		else if(is_fbgc_BINARY_OPERATOR(type)){
			struct fbgc_object * o2 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * o1 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);


			struct fbgc_object * res = call_fbgc_binary_op(
					get_fbgc_object_type(iter),
					get_var_from_fbgc_ref_object(o1),
					get_var_from_fbgc_ref_object(o2));

			if(res != NULL) {
				stack = push_front_fbgc_ll_object(stack,res);
				iter_prev->next = iter->next;

				free_fbgc_object(o1);
				free_fbgc_object(o2);
				free_fbgc_object(iter);				
			}
			else {
				cprintf(100,"NULL operator return!\n");
				free_fbgc_object(o1);
				free_fbgc_object(o2);				
				break;

			}
		}
		else if(is_fbgc_ASSIGNMENT_OPERATOR(type)){


			//o1 = o2
			struct fbgc_object * o2 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * o1 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			call_fbgc_assignment_op(type,o1,o2);


			iter_prev->next = iter->next;
					
		}
		
		else if(get_fbgc_object_type(iter) == SUBSCRIPT){
			struct fbgc_object * index = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * var = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);

			cprintf(111,"index:%s,vaR:%s\n",object_type_as_str(index),object_type_as_str(var));

			struct fbgc_object * res = call_get_item_with_index(get_var_from_fbgc_ref_object(var),get_var_from_fbgc_ref_object(index));
			iter_prev->next = iter->next;
			
			free_fbgc_object(var);
			free_fbgc_object(index);
			free_fbgc_object(iter);

			if(res == NULL) {
				cprintf(100,"Error for SUBSCRIPT in interpreter\n");
				break;				
			}

			push_front_fbgc_ll_object(stack,res);
		}		
		
		else if(type == TUPLE){
			struct fbgc_object * to = new_fbgc_tuple_object(cast_fbgc_object_as_int(top_fbgc_ll_object(stack))->content);
			stack = delete_front_fbgc_ll_object(stack);
			for(int q = cast_fbgc_object_as_tuple(to)->size - 1; q>=0; q--){
				
				struct fbgc_object * ob  = top_fbgc_ll_object(stack);
				stack = pop_front_fbgc_ll_object(stack);
				set_object_in_fbgc_tuple_object(to,get_var_from_fbgc_ref_object(ob),q);
			}
			iter_prev->next = iter->next;

			free_fbgc_object(iter);
			stack = push_front_fbgc_ll_object(stack,to);
		}
		else if(type == NUPLE){
			
			iter_prev->next = iter->next;
			stack = push_front_fbgc_ll_object(stack,iter);
		}
		else if(get_fbgc_object_type(iter) == RAW_MATRIX){

			struct fbgc_object * mato = new_fbgc_matrix_object(cast_fbgc_object_as_int(top_fbgc_ll_object(stack))->content);
			append_row_to_fbgc_matrix_object(mato,top_fbgc_ll_object(stack));
			stack = push_front_fbgc_ll_object(stack,mato);
		}

		else if(type == CFUN){
			
			struct fbgc_object * o2 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * o1 = cfun_object_call(iter,o2);
			if(o1 == NULL){
				cprintf(111,"Cfunction returned NULL immeadiately stopped!\n");
				free_fbgc_object(o2);
				break;
			}
			stack = push_front_fbgc_ll_object(stack,o1);
			iter_prev->next = iter->next;
			free_fbgc_object(o2);
			free_fbgc_object(iter);
			
		}
		else {
			//fbgc_assert(get_fbgc_object_type(iter) == TUPLE,"tuple fuck !\n");
			cprintf(101,"Undefined token in interpreter\n");
			break;
		}

		iter = iter_prev->next;
		
		#ifdef INTERPRETER_DEBUG
		//cprintf(111,"helo!\n is nULL :%d\n",cast_fbgc_object_as_field(*field_obj)->head == NULL);
		print_fbgc_ll_object(cast_fbgc_object_as_field(*field_obj)->head,"Main");
		print_fbgc_ll_object(stack,"Stack");
		//print_fbgc_symbol_table(cast_fbgc_object_as_field(*field_obj)->symbol_table);
		cprintf(111,"===========================================\n");
		#endif
	}

	print_fbgc_symbol_table(cast_fbgc_object_as_field(*field_obj)->symbols);

	//claim_ownership_in_symbol_table(cast_fbgc_object_as_field(*field_obj)->global_table);
	//make the linked list connection proper
	head->tail->next = iter_prev;

	free_fbgc_ll_object(stack);
	#ifdef INTERPRETER_DEBUG
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}
*/


uint8_t interpreter(struct fbgc_object ** field_obj){

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"==========[INTERPRETER]==========\n");
	#endif


	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * pc = head->base.next; //program counter

	#define PROGRAM_STACK_SIZE 100
	struct fbgc_object * stack = new_fbgc_tuple_object(PROGRAM_STACK_SIZE);
	struct fbgc_object ** sp = tuple_object_content(stack);
	int sctr = 0;
	int fctr = -1;
	
	struct fbgc_object ** globals = tuple_object_content(cast_fbgc_object_as_field(*field_obj)->locals);

/*
#define _STACK_GOTO(i) 	(sp += i)
#define _PUSH(x)		(*sp++ = (x))
#define PUSH(x)			()
#define _POP()			(*--sp)
#define TOP()			(sp[-1])
#define SECOND()		(sp[-2])
#define THIRD()			(sp[-3])
#define TOPN(i)			(sp[-(i)])
#define SET_TOP(x)		(sp[-1] = (x))
#define SET_SECOND(x)	(sp[-2] = (x))
#define SET_THIRD(x)	(sp[-3] = (x))
#define SET_TOPN(i, x)	(sp[-(i)] = (x))
*/

#define _STACK_GOTO(i) 	(sctr += i)
#define _PUSH(x)		(*(sp+sctr++) = (x))
#define PUSH(x)			()
#define _POP()			(sp[--sctr])
#define TOP()			(sp[sctr-1])
#define SECOND()		(sp[sctr-2])
#define THIRD()			(sp[sctr-3])
#define TOPN(i)			(sp[sctr-(i)])
#define SET_TOP(x)		(sp[sctr-1] = (x))
#define SET_SECOND(x)	(sp[sctr-2] = (x))
#define SET_THIRD(x)	(sp[sctr-3] = (x))
#define SET_TOPN(i, x)	(sp[sctr-(i)] = (x))	

#define GET_AT_FP(n)		(sp[fctr+(n)])
#define SET_AT_FP(n, x)	(sp[fctr+(n)] = (x))


#define FETCH_NEXT()(pc = pc->next)

	for(int i = 0; (pc != head->tail); i++){

		fbgc_token type = get_fbgc_object_type(pc);
		

		#ifdef INTERPRETER_DEBUG
		cprintf(010,"################ [%d] = {%s} ########################\n",i,object_type_as_str(pc));
		cprintf(100,"sctr:%d, fctr:%d\n",sctr,fctr);
		#endif

		// print_fbgc_memory_block();

		switch(type){
			case INT:
			case DOUBLE:
			case STRING:
			case COMPLEX:
			case GLOBAL:
			case LOCAL:
			case FUN:
			case NUPLE:
			case MONUPLE:
			{
				_PUSH(pc);
				break;
			}

			case LOAD_GLOBAL:
			{
				_PUSH(globals[cast_fbgc_object_as_int(pc)->content]);
				break;

			}

			case LOAD_LOCAL:
			{
				_PUSH(GET_AT_FP(cast_fbgc_object_as_int(pc)->content));
				break;
			}

			case PLUS:
			case MINUS:
			case STAR:
			case SLASH:
			case MOD:
			case STARSTAR:
			case CARET:
			case OR_OP:
			case AND_OP:
			case GREATER:
			case LOWER:
			case LO_EQ:
			case GR_EQ:
			case EQ_EQ:
			case NOT_EQ:
			case IS_EQ:
			case IF_THEN:
			case R_ARROW:
			case L_ARROW:
			case COL_LO:
			case RW_ARROW:
			case LW_ARROW:
			case COLSTAR:
			case COLSLASH:
			case SLASHSLASH:
			case COLON:
			case RSHIFT:
			case LSHIFT:
			{
				struct fbgc_object * res =  call_fbgc_binary_op(type,_POP(),_POP());

				if(res != NULL) {
					_PUSH(res);				
				}
				else {
					cprintf(100,"NULL operator return!\n");				
					return 0;
				}
				break;				
			}

			case ASSIGN:
			case PLUS_ASSIGN:
			case MINUS_ASSIGN:
			case STAR_ASSIGN:
			case SLASH_ASSIGN:
			case CONST_ASSIGN:
			{
				struct fbgc_object * rhs = _POP();
				struct fbgc_object * lhs = _POP();
				
				if(lhs->type == GLOBAL){
					globals[cast_fbgc_object_as_int(lhs)->content] = rhs;
				}
				else if(lhs->type == LOCAL){
					GET_AT_FP(cast_fbgc_object_as_int(lhs)->content) = rhs;
				}
				else {
					return 0;
				}
				break;				
			}

			case FUN_CALL:
			{
				struct fbgc_object * funo = _POP();
				struct fbgc_object * args = _POP();


				if(args->type == TUPLE && size_fbgc_tuple_object(args) != cast_fbgc_object_as_fun(funo)->no_arg){
					cprintf(100,"Argument match error!");
					return 0;
				}
				else if(args->type == MONUPLE && cast_fbgc_object_as_fun(funo)->no_arg == 1){
					args = _POP();
				}

				//save the old pc in the stack
				_PUSH(pc->next);
				//hold old frame pointer location
				_PUSH(new_fbgc_int_object(fctr));
				//hold old position of sp with fp	
				fctr = sctr;
				//change sp to hold local variables
				sctr += cast_fbgc_object_as_fun(funo)->no_locals;	
				//write args into locals
				if(cast_fbgc_object_as_fun(funo)->no_arg == 1) GET_AT_FP(0) = args;

				//execute function
				stack->next = cast_fbgc_object_as_fun(funo)->code;
				pc = stack;

				#ifdef INTERPRETER_DEBUG
				cast_fbgc_object_as_tuple(stack)->size = sctr;
				cprintf(111,"\n==============Stack==========================\n");
				print_fbgc_object(stack);
				cprintf(111,"\n==================globals===================\n");
				print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->locals);
				cprintf(111,"\n==============================================\n\n");
				#endif

				break;
				//continue;
			}


			case RETURN:{

				struct fbgc_object * ret = _POP();
				sctr = fctr;
				fctr = cast_fbgc_object_as_int(TOP())->content;
				stack->next = SECOND();
				_STACK_GOTO(-2);
				_PUSH(ret);
				pc = stack;
				break;
				//continue;
			}


			default:
			{
				cprintf(101,"Undefined token in interpreter\n");
				return 0;						
			}
		}
		
		#ifdef INTERPRETER_DEBUG
		cast_fbgc_object_as_tuple(stack)->size = sctr;//sp - tuple_object_content(stack);
		cprintf(111,"\n==============Stack==========================\n");
		print_fbgc_object(stack);
		cprintf(111,"\n==================globals===================\n");
		print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->locals);
		cprintf(111,"\n==============================================\n\n");
		#endif

		FETCH_NEXT();

		/*
		if(is_fbgc_ATOM(type) ||type == GLOBAL || type == LOCAL || type == FUN || type == NUPLE ||type == MONUPLE){
			_PUSH(pc);
		}
		else if(type == LOAD_GLOBAL){
			_PUSH(globals[cast_fbgc_object_as_int(pc)->content]);  
		}
		else if(type == LOAD_LOCAL){

			_PUSH(GET_AT_FP(cast_fbgc_object_as_int(pc)->content) ) ;  
		}		
		else if(is_fbgc_BINARY_OPERATOR(type)){

			struct fbgc_object * res =  call_fbgc_binary_op(type,_POP(),_POP());

			if(res != NULL) {
				_PUSH(res);				
			}
			else {
				cprintf(100,"NULL operator return!\n");				
				return 0;
			}
		}
		else if(is_fbgc_ASSIGNMENT_OPERATOR(type)){
			// lhs = rhs

			struct fbgc_object * rhs = _POP();
			struct fbgc_object * lhs = _POP();
			
			if(lhs->type == GLOBAL){
				globals[cast_fbgc_object_as_int(lhs)->content] = rhs;
			}
			else if(lhs->type == LOCAL){
				GET_AT_FP(cast_fbgc_object_as_int(lhs)->content) = rhs;
			}
			else {
				return 0;
			}
			//call_fbgc_assignment_op(type,lhs,rhs);
		}
		else if(type == FUN_CALL){

			struct fbgc_object * funo = _POP();
			struct fbgc_object * args = _POP();


			if(args->type == TUPLE && size_fbgc_tuple_object(args) != cast_fbgc_object_as_fun(funo)->no_arg){
				cprintf(100,"Argument match error!");
				return 0;
			}
			else if(args->type == MONUPLE && cast_fbgc_object_as_fun(funo)->no_arg == 1){
				args = _POP();
			}

			//save the old pc in the stack
			_PUSH(pc->next);
			//hold old frame pointer location
			_PUSH(new_fbgc_int_object(fctr));
			//hold old position of sp with fp	
			fctr = sctr;
			//change sp to hold local variables
			sctr += cast_fbgc_object_as_fun(funo)->no_locals;	
			//write args into locals
			if(cast_fbgc_object_as_fun(funo)->no_arg == 1) GET_AT_FP(0) = args;

			//execute function
			pc = cast_fbgc_object_as_fun(funo)->code;

			#ifdef INTERPRETER_DEBUG
			cast_fbgc_object_as_tuple(stack)->size = sctr;
			cprintf(111,"\n==============Stack==========================\n");
			print_fbgc_object(stack);
			cprintf(111,"\n==================globals===================\n");
			print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->locals);
			cprintf(111,"\n==============================================\n\n");
			#endif			
			continue;
   
		}
		else if(type == RETURN){
			//hold return object
			struct fbgc_object * ret = _POP();
			sctr = fctr;
			fctr = cast_fbgc_object_as_int(TOP())->content;
			pc = SECOND();
			_STACK_GOTO(-2);
			_PUSH(ret);

			#ifdef INTERPRETER_DEBUG
			cast_fbgc_object_as_tuple(stack)->size = sctr;
			cprintf(111,"\n==============Stack==========================\n");
			print_fbgc_object(stack);
			cprintf(111,"\n==================globals===================\n");
			print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->locals);
			cprintf(111,"\n==============================================\n\n");
			#endif

			continue;
		}
		else if(type == IF_BEGIN || type == ELIF_BEGIN || type == WHILE_BEGIN){
			struct fbgc_object * cond = _POP();
			//struct fbgc_object * obj = pc;
			if( get_fbgc_object_type(is_fbgc_object_true(cond)) == FALSE  ){
				pc = cast_fbgc_object_as_jumper(pc)->content;
			}
		}
		else if(type == JUMP){
			pc = cast_fbgc_object_as_jumper(pc)->content;
		}	
		else if(type == BREAK){
			struct fbgc_object * loop_obj =  cast_fbgc_object_as_jumper(pc)->content;
			pc = cast_fbgc_object_as_jumper(loop_obj)->content;
		}				
		else {
			cprintf(101,"Undefined token in interpreter\n");
			return 0;		

		}

		//#############TODO#############################
		//This part is required for the stack size increment, stack is a tuple object and it has a defined size range
		//In order to increase operation speed we don't wanna increase the stack size after each push but it has to be
		//checked periodically to handle big objects like constant value tuples/lists etc.
		//Now just left for debug but later this problem has to be solved.
		#ifdef INTERPRETER_DEBUG
			cast_fbgc_object_as_tuple(stack)->size = sctr;//sp - tuple_object_content(stack);
		#endif

		pc = pc->next;
		*/
		
	}

	//#ifdef INTERPRETER_DEBUG
	cprintf(111,"\n==============Stack==========================\n");
	print_fbgc_object(stack);
	cprintf(111,"\n==================globals===================\n");
	print_fbgc_object(cast_fbgc_object_as_field(*field_obj)->locals);
	cprintf(111,"\n==============================================\n\n");
	//#endif

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}
