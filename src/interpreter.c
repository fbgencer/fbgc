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

#define RECURSION_LIMIT 1000

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
						index = cast_fbgc_object_as_int(TOPN(index_no-i))->content;
						if(dummy->type == TUPLE){
							//cprintf(111,"Current index %d\n",index);
							dummy = get_object_in_fbgc_tuple_object(dummy,index);
							//print_fbgc_object(dummy); cprintf(111,"<<<\n");
						}
						else if(dummy->type == STRING){
							dummy = subscript_fbgc_str_object(dummy,index,index+1);
							assert(dummy != NULL);
						}
						else {
							cprintf(111,"Not index accessable!\n");
							print_fbgc_object(dummy); printf("\n");
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
			case CONT:
			{
				stack->next = cast_fbgc_object_as_jumper(pc)->content;
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
				//cprintf(111,"Stack next :");
				//print_fbgc_object(stack->next);

				recursion_ctr = 0;

				break;
			}			
			case PLUS_ASSIGN:
			case MINUS_ASSIGN:
			case STAR_ASSIGN:
			case SLASH_ASSIGN:
			{
				struct fbgc_object * rhs = _POP();

				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = 
					(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					if(type == PLUS_ASSIGN){
						fbgc_token main_tok = 
						get_fbgc_object_type(tmp->content) > get_fbgc_object_type(rhs)  ? 
						get_fbgc_object_type(tmp->content) : 
						get_fbgc_object_type(rhs) ;

						tmp->content = call_fbgc_binary_op(main_tok,tmp->content,rhs,PLUS);						
					}
					
				} 
				else if(is_id_flag_LOCAL(pc)){
					GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc) = rhs;
				}					
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


				if(is_fbgc_binary_op_null(main_tok)){
					cprintf(111,"This type does not support operation.\n");
					return 0;
				}
				//struct fbgc_object * res =  call_fbgc_binary_op(main_tok,_POP(),_POP(),type);

				struct fbgc_object * res =  safe_call_fbgc_binary_op(_POP(),_POP(),main_tok,type);
				
/* func array ile
 42d:	4d 8d 6c 0c f0       	lea    r13,[r12+rcx*1-0x10]
 432:	ff cb                	dec    ebx
 434:	49 8b 7d 00          	mov    rdi,QWORD PTR [r13+0x0]
 438:	ff d0                	call   rax

switch case rec func ile
 42f:	4d 8d 6c 04 f0       	lea    r13,[r12+rax*1-0x10]
 434:	ff cb                	dec    ebx
 436:	49 8b 7d 00          	mov    rdi,QWORD PTR [r13+0x0]
 43a:	e8 00 00 00 00       	call   43f <interpreter+0x43f>	43b: R_X86_64_PLT32	safe_call_fbgc_binary_op-0x4
 434:	49 8b 7d 00          	mov    rdi,QWORD PTR [r13+0x0]
 438:	ff d0                	call   rax
*/
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
						}
						else {
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
				//_POP();
				
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
			case BUILD_MATRIX:
			{	
				

				int ctr = cast_fbgc_object_as_int(pc)->content;
				if(ctr == 1 && TOP()->type == MATRIX) break;


				int row = 0;
				int col = 0;
				int msize = 0;

				//traverse reverse!

				//cprintf(111,"sctr %d, ctr %d\n",sctr,ctr);
				/*cast_fbgc_object_as_tuple(stack)->size = sctr;//sp - tuple_object_content(stack);
				cprintf(111,"\n==============Stack==========================\n");
				print_fbgc_object(stack);*/				
				

				for( int i = 1; i<=sctr && i <= ctr; ++i){
					//cprintf(011,"i = %d ctr %d\n",i,ctr);
					switch(TOPN(i)->type){
						case INT:
						case DOUBLE:
						{
							++msize;
							//cprintf(111,"int/db row:%d col:%d\n",row,col);
							break;
						}	
						case MATRIX:
						{
							int r = cast_fbgc_object_as_matrix(TOPN(i))->row;
							int c = cast_fbgc_object_as_matrix(TOPN(i))->column;
							//cprintf(011,"top is matrix %dx%d\n",r,c);
							msize +=  r*c;
							//row += r;
							break;					
						}
						default:
						{
							cprintf(111,"Type %s in matrix is cannot be located\n",object_name_array[TOPN(i+1)->type]);
							return 0;
						}
					}	
					
					if(i < sctr && TOPN(i+1)->type == ROW){
						++row;
						ctr += 1+cast_fbgc_object_as_int(TOPN(i+1))->content;
						//cprintf(010,"Top is row, msize: %d, ctr :%d ,row :%d\n",msize,ctr,row);
						++i;
					}
				}

				//cprintf(010,"Output of loop, row:%d , col :%d ctr:%d\n",row,old_col,ctr);
				
				//return 0;

				
				//struct fbgc_object * m = new_fbgc_matrix_object(ctr-row);
				struct fbgc_object * m =
				 matrix_creation_from_stack(sp+sctr-ctr ,ctr, msize, row);
				assert(m != NULL);
				//print_fbgc_matrix_object(m);



				_STACK_GOTO(-ctr);
				_PUSH(m);

				break;
				//return 0;
			}
			
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
