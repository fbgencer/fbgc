#include "fbgc.h"

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
			case ASSIGN_GLOBAL:
			{
				struct fbgc_object * rhs = _POP();
				globals[cast_fbgc_object_as_int(pc)->content] = rhs;
				break;				
			}
			case ASSIGN_LOCAL:
			{
				struct fbgc_object * rhs = _POP();
				GET_AT_FP(cast_fbgc_object_as_int(pc)->content) = rhs;
				break;
			}
			case ASSIGN_SUBSCRIPT:
			{
				//return 0;
				/*struct fbgc_object * rhs = _POP();
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
				
				set_object_in_fbgc_tuple_object(obj,rhs,index);*/

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
				//##Solve this pc->next problem!!!!!!!!
				stack->next = cast_fbgc_object_as_fun(funo)->code;
				pc = stack;

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
				//##Solve this pc->next problem!!!!!!!!
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
