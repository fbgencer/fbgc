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
			cprintf(100,"Reached Recursion limit!\n");
			break;
		}

		if(sctr > PROGRAM_STACK_SIZE){
			cprintf(100,"Stack Overflow!");
			break;
		}

		fbgc_token type = get_fbgc_object_type(pc);
		

		#ifdef INTERPRETER_DEBUG
		cprintf(010,"################ [%d] = {%s} ########################",i,object_type_as_str(pc));
		print_fbgc_object(pc);
		cprintf(100,"sctr:%d, fctr:%d\n",sctr,fctr);
		#endif

		// print_fbgc_memory_block();

		switch(type){
			case INT:
			case DOUBLE:
			case STRING:
			case COMPLEX:
			case FUN:
			case NUPLE:
			case MONUPLE:
			case CFUN:
			{
				_PUSH(pc);
				break;
			}

			case IDENTIFIER:
			{	
				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = (struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					assert(tmp->content != NULL);
						_PUSH(tmp->content);
					//_PUSH(globals[cast_fbgc_object_as_id_opcode(pc)->loc]);	
				} 
				else if(is_id_flag_LOCAL(pc))  _PUSH(GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc));
				break;
			}
			case BUILD_TUPLE:
			{	

				int tuple_size = cast_fbgc_object_as_int(TOP())->content;
				struct fbgc_object * to = new_fbgc_tuple_object(tuple_size);
				size_fbgc_tuple_object(to) = tuple_size;
				_POP();

				while(--tuple_size >= 0){
					struct fbgc_object * ob  = _POP();
					//to = push_back_fbgc_tuple_object(to,ob);
					set_object_in_fbgc_tuple_object(to,ob,tuple_size);
				}

				_PUSH(to);
				break;
			} 
			case PLUS:
			case MINUS:
			case STAR:
			case SLASH:
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
			{
				struct fbgc_object * rhs = _POP();
				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = (struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					tmp->content = rhs;
				} 

				else if(is_id_flag_LOCAL(pc))  GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc) = rhs;	
				break;
			}
			/*case ASSIGN_SUBSCRIPT:
			{
				//return 0;
				struct fbgc_object * rhs = _POP();
				struct fbgc_object * obj = _POP();

				int arg_no = cast_fbgc_object_as_int(TOP())->content; 
				_POP();
				int index = 0;

				_STACK_GOTO(-arg_no+1);	

				cprintf(110,"arg no:%d\n",arg_no);
				print_fbgc_object(TOPN(0));
				
				for( int i = 0; i<arg_no; i++ ){
					if(obj->type == TUPLE){
						index = cast_fbgc_object_as_int(TOPN(-i+1))->content;
						cprintf(111,"Index: %d\n",index);
						if(arg_no>1){
							//arg_no--;
							cprintf(111,"Now object...\n");
							obj = get_object_in_fbgc_tuple_object(obj,index);
						}
					}
					else {
						cprintf(100,"Error returning 0\n");
						break;
						//return 0;
					}					
				}
				
				set_object_in_fbgc_tuple_object(obj,rhs,index);

				break;	
			}*/
			case FUN_CALL:
			{
				struct fbgc_fun_object * funo = cast_fbgc_object_as_fun(_POP());
				int arg_no = cast_fbgc_object_as_int(TOP())->content;
				_POP();


				if(funo->base.type == CFUN){
					//return 0;
					struct fbgc_object * arg_tuple =  new_fbgc_tuple_object_from_tuple_content(sp+sctr-arg_no,arg_no);
					_STACK_GOTO(-arg_no);
					//print_fbgc_object(cto);
					struct fbgc_object * res = cfun_object_call(funo,arg_tuple);
					//assert(res != NULL);
					if(res != NULL) _PUSH(res);
					break;
					//return 0;
				}

				if(last_called_function == funo) recursion_ctr++;
				else {
					last_called_function = (struct fbgc_object *) funo;
					recursion_ctr = 0;
				}

				if(funo->no_arg != arg_no ){
					cprintf(100,"Argument match error!");
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
				break;
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
		print_field_object_locals(*field_obj);
		cprintf(111,"\n==============================================\n\n");
		#endif

		FETCH_NEXT();

	}

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"\n==============Stack==========================\n");
	print_fbgc_object(stack);
	cprintf(111,"\n==================globals===================\n");
	print_field_object_locals(*field_obj);
	cprintf(111,"\n==============================================\n\n");
	#endif

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}
