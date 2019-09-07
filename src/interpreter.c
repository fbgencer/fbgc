#include "fbgc.h"

uint8_t interpreter(struct fbgc_object ** field_obj){

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"==========[INTERPRETER]==========\n");
	#endif


	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * pc = head->base.next; //program counter

	#define PROGRAM_STACK_SIZE 1000
	struct fbgc_object * stack = new_fbgc_tuple_object(PROGRAM_STACK_SIZE);
	struct fbgc_object ** sp = tuple_object_content(stack);
	int sctr = 0;
	int fctr = -1;
	
	struct fbgc_object * globals = cast_fbgc_object_as_field(*field_obj)->locals;

	struct fbgc_object * last_called_function = NULL;
	size_t recursion_ctr = 0;

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

#define RECURSION_LIMIT 100

	for(int i = 0; (pc != head->tail) ; i++){

		if(recursion_ctr>RECURSION_LIMIT){
			printf("Reached Recursion limit!\n");
			break;
		}

		if(sctr > PROGRAM_STACK_SIZE){
			printf("Stack Overflow!");
			break;
		}

		fbgc_token type = get_fbgc_object_type(pc);
		

		#ifdef INTERPRETER_DEBUG
		cprintf(010,"################ [%d] = {%s} ########################\n",i,object_type_as_str(pc));
		//print_fbgc_object(pc);
		//cprintf(100,"sctr:%d, fctr:%d\n",sctr,fctr);
		#endif

		// print_fbgc_memory_block();

		switch(type){
			case NIL:
			case INT:
			case DOUBLE:
			case COMPLEX:			
			case STRING:
			case FUN:
			case CFUN:
			case ROW:
			{
				_PUSH(pc);
				break;
			}
			case IDENTIFIER:
			{	
				if(is_id_flag_SUBSCRIPT(pc)){
					//cprintf(111,"Id flag subscript \n");
					//first pop the number of indexes
					int index_no = cast_fbgc_object_as_int(_POP())->content;
					//take index values one by one and finally left last index 
					struct fbgc_object * dummy;

					if(is_id_flag_GLOBAL(pc)){
						//cprintf(111,"Globalde subscript\n");
						struct fbgc_identifier * tmp = 
						(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
						dummy = tmp->content;
						
					}
					else if(is_id_flag_LOCAL(pc)){
						//cprintf(111,"Localde subscript\n");
						dummy = GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc);
					}	

					int index = 0;
					for(int i = 0; i<index_no; i++){
						if(dummy->type == TUPLE){
							index = cast_fbgc_object_as_int(TOPN(index_no-i))->content;
							//cprintf(111,"Current index %d\n",index);
							dummy = get_object_in_fbgc_tuple_object(dummy,index);
							//print_fbgc_object(dummy); cprintf(111,"<<<\n");
						}else {
							cprintf(111,"Not index accessable!\n");
							cprintf(111,"Dummy: "); print_fbgc_object(dummy); printf("\n");
							return 0;
						}

					}
					//Since this is the top index we can just use top
					//index = cast_fbgc_object_as_int(TOP())->content;
					//cprintf(111,"Son index %d\n",index);

					_STACK_GOTO(-index_no);
					_PUSH(dummy);
					break;
				}
				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = (struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					assert(tmp->content != NULL);
						_PUSH(tmp->content);
					//_PUSH(globals[cast_fbgc_object_as_id_opcode(pc)->loc]);	
				} 
				else if(is_id_flag_LOCAL(pc))  _PUSH(GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc));
				break;
			}
			case BREAK:
			{
				struct fbgc_object * loop_obj =  cast_fbgc_object_as_jumper(pc)->content;
				pc = cast_fbgc_object_as_jumper(loop_obj)->content;	
				break;
			}			
			case RETURN:{

				struct fbgc_object * ret = _POP();
				int old_fctr = fctr;
				fctr = cast_fbgc_object_as_int(TOP())->content;
				stack->next = SECOND();
				sctr = old_fctr;
				//_STACK_GOTO(-2);
				 _PUSH(ret);
				//##Solve this pc->next problem!!!!!!!!
				pc = stack;
				//cprintf(111,"Stack next :");
				//print_fbgc_object(stack->next);
				break;
			}			

			case STARSTAR:
			case PLUSPLUS:
			case MINUSMINUS:
			case SLASHSLASH:
			case LO_EQ:
			case GR_EQ:
			case EQ_EQ:
			case NOT_EQ:
			case R_SHIFT:
			case L_SHIFT:
			case CARET:
			case PERCENT:
			case LOWER:
			case GREATER:
			case PIPE:
			case AMPERSAND:
			case SLASH:
			case STAR:
			case MINUS:
			case PLUS:
			{
				
				assert(TOP() != NULL && SECOND() != NULL);

					fbgc_token main_tok = 
					(get_fbgc_object_type(TOP()) > get_fbgc_object_type(SECOND())) ? 
					get_fbgc_object_type(TOP()) : 
					get_fbgc_object_type(SECOND()) ;

				struct fbgc_object * res =  call_fbgc_binary_op(type,_POP(),_POP(),main_tok);

				assert(res != NULL);
				_PUSH(res);		

				break;	

			}
			case ASSIGN:
			{
				struct fbgc_object * rhs = _POP();

				//cprintf(100,"Assign Flag %0x\n",get_id_flag(pc));
				//return 0;

				if(is_id_flag_SUBSCRIPT(pc)){
					//cprintf(111,"Id flag subscript \n");
					//first pop the number of indexes
					int index_no = cast_fbgc_object_as_int(_POP())->content;
					//take index values one by one and finally left last index 
					struct fbgc_object * dummy;

					if(is_id_flag_GLOBAL(pc)){
						//cprintf(111,"Globalde subscript\n");
						struct fbgc_identifier * tmp = 
						(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
						dummy = tmp->content;
						
					}
					else if(is_id_flag_LOCAL(pc)){
						//cprintf(111,"Localde subscript\n");
						dummy = GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc);
					}	

					int index = 0;
					for(int i = 0; i<index_no-1; i++){
						if(dummy->type == TUPLE){
							index = cast_fbgc_object_as_int(TOPN(index_no-i))->content;
							//cprintf(111,"Current index %d\n",index);
							dummy = get_object_in_fbgc_tuple_object(dummy,index);
							//print_fbgc_object(dummy); cprintf(111,"<<<\n");
						}else {
							cprintf(111,"Not index accessable!\n");
							cprintf(111,"Dummy: "); print_fbgc_object(dummy); printf("\n");
							return 0;
						}

					}
					//Since this is the top index we can just use top
					index = cast_fbgc_object_as_int(TOP())->content;
					//cprintf(111,"Son index %d\n",index);
					set_object_in_fbgc_tuple_object(dummy,rhs,index);
						//tmp->content = rhs;
					_STACK_GOTO(-index_no);
					break;
				}

				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = 
					(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					tmp->content = rhs;
				} 
				else if(is_id_flag_LOCAL(pc)){
					GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc) = rhs;
				}	
				break;
			}
			case JUMP:
			{
				pc = cast_fbgc_object_as_jumper(pc)->content;	
				break;
			}
			case IF_BEGIN:
			case ELIF_BEGIN:
			case WHILE_BEGIN:
			{
				struct fbgc_object * cond = _POP();
				//Check this! cond->type == INT 
				if(!cast_fbgc_object_as_int(cond)->content  ){
					pc = cast_fbgc_object_as_jumper(pc)->content;
				}
				break;
			}
			case FUN_CALL:
			{
				struct fbgc_fun_object * funo = cast_fbgc_object_as_fun(_POP());
				int arg_no = cast_fbgc_object_as_int(_POP())->content;
				
				
				//assert(funo->base.type == FUN || funo->base.type == CFUN);

				if(funo->base.type == CFUN){
					//return 0;
					struct fbgc_object * arg_tuple =  new_fbgc_tuple_object_from_tuple_content(sp+sctr-arg_no,arg_no);
					_STACK_GOTO(-arg_no);
					struct fbgc_object * res = cfun_object_call(funo,arg_tuple);
					if(res != NULL) _PUSH(res);
					break;

					// In order to increase speed, DELETE new tuple creation it causes 2sec for 100,000 print('ffdfds') code
					
					//return 0;

				}

				if(last_called_function == funo) recursion_ctr++;
				else {
					last_called_function = (struct fbgc_object *) funo;
					recursion_ctr = 0;
				}

				if(funo->no_arg != arg_no ){
					cprintf(100,"Argument match error! funo->arg %d, arg_no %d\n",funo->no_arg,arg_no);
					return 0;
				}

				_STACK_GOTO(funo->no_locals - arg_no);
				//save our first next operation after this function call
				//After returning the value we will take this space and run the main code
				_PUSH(pc->next);
				//hold old frame pointer location
				_PUSH(new_fbgc_int_object(fctr));
				//hold old position of sp with fp, assume that args already pushed into stack
				fctr = sctr-funo->no_locals-2;
				//execute function
				//##Solve this pc->next problem!!!!!!!!
				stack->next = cast_fbgc_object_as_fun(funo)->code;
				pc = stack;
				break;
			}
			case BUILD_TUPLE:
			{	

				int tuple_size = cast_fbgc_object_as_int(pc)->content;
				struct fbgc_object * to = new_fbgc_tuple_object(tuple_size);
				size_fbgc_tuple_object(to) = tuple_size;
					
				while(--tuple_size >= 0){
					set_object_in_fbgc_tuple_object(to,_POP(),tuple_size);
				}

				_PUSH(to);
				break;
			}
			/*case BUILD_MATRIX:
			{	


				//first inspect the data 
				//this is for opening a whole memory because we wanna
				//contain all the elemnts in a c-array
				//this would be slow but instead of consuming memory, it's worth it(is it?)
				int column = cast_fbgc_object_as_int(pc)->content;
				int row = 1;

				//traverse reverse!

				for(int i = 1; i<=(row*(column+1)); i++){
					print_fbgc_object(TOPN(i));
					cprintf(111,"\n");
					if(TOPN(i)->type == ROW){
						row++;
					}
					if(i == sctr) break;
				}

				cprintf(010,"So row:%d column %d\n",row,column);
				return 0;
			}
			*/
			default:
			{
				cprintf(101,"Undefined token in interpreter [%s]\n",object_name_array[type]);
				return 0;						
			}
		}
		
		#ifdef INTERPRETER_DEBUG
		cast_fbgc_object_as_tuple(stack)->size = sctr;//sp - tuple_object_content(stack);
		cprintf(111,"\n==============Stack==========================\n");
		print_fbgc_object(stack);
		cprintf(111,"\n==================globals===================\n");
		print_field_object_locals(*field_obj);
		cprintf(111,"\n==============================================\n\n");
		#endif

		FETCH_NEXT();

	}

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"\n==============Stack==========================\n");
	print_fbgc_object(stack);

	cprintf(111,"\n==================Globals===================\n");
	print_field_object_locals(*field_obj);
	cprintf(111,"\n==============================================\n\n");
	#endif
	#ifdef INTERPRETER_DEBUG
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}
