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

#define STACK_GOTO(i) 	(sctr += i)
#define PUSH(x)		(*(sp+ sctr++ ) = (x))
#define POP()			(sp[--sctr])
#define _POP()			(--sctr)
#define TOP()			(sp[sctr-1])
#define SECOND()		(sp[sctr-2])
#define THIRD()			(sp[sctr-3])
#define TOPN(i)			(sp[sctr-(i)])
#define SET_TOP(x)		(sp[sctr-1] = (x))
#define SET_SECOND(x)	(sp[sctr-2] = (x))
#define SET_THIRD(x)	(sp[sctr-3] = (x))
#define SET_TOPN(i, x)	(sp[sctr-(i)] = (x))	

//FRAME MACROS
#define GET_AT_FP(n)		(sp[fctr+(n)])
#define SET_AT_FP(n, x)	(sp[fctr+(n)] = (x))

#define FETCH_NEXT()(pc = pc->next)

#define MAX(a,b)( a > b ? a : b )


#define RECURSION_LIMIT 1000

	for(int i = 0;  (pc != head->tail) ; i++){

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
		#endif

		switch(type){
			case NIL:
			case LOGIC:
			case INT:
			case DOUBLE:
			case COMPLEX:			
			case STRING:
			case CSTRING:
			case CFUN:
			case FUN:
			case CSTRUCT:
			case ROW:
			{
				PUSH(pc);
				break;
			}
			case IDENTIFIER:
			{	

				if(is_id_flag_MEMBER(pc)){
					//if(TOP()->type == CSTRING)
					struct fbgc_object * name = cast_fbgc_object_as_id_opcode(pc)->member_name;
					SET_TOP( get_set_fbgc_object_member(TOP(),&cast_fbgc_object_as_cstr(name)->content , NULL) );
					break;
				}

				if(is_id_flag_SUBSCRIPT(pc)){
					//first pop the number of indexes
					int index_no = cast_fbgc_object_as_int(POP())->content;
					//take index values one by one and finally left last index 
					struct fbgc_object * temp;

					if(is_id_flag_GLOBAL(pc)){
						//cprintf(111,"Globalde subscript\n");
						struct fbgc_identifier * tmp = 
						(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
						temp = tmp->content;
						
					}
					else if(is_id_flag_LOCAL(pc)){
						//cprintf(111,"Localde subscript\n");
						temp = GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc);
					}	

					int index = 0;
					for(int i = 0; i<index_no; i++){
						index = cast_fbgc_object_as_int(TOPN(index_no-i))->content;
						if(temp->type == TUPLE){
							//cprintf(111,"Current index %d\n",index);
							temp = get_object_in_fbgc_tuple_object(temp,index);
							//print_fbgc_object(dummy); cprintf(111,"<<<\n");
						}
						else if(temp->type == COMPLEX){
							temp = subscript_fbgc_complex_object(temp,index);
						}
						else if(temp->type == STRING){
							temp = subscript_fbgc_str_object(temp,index,index+1);
						}
						else if(temp->type == MATRIX){
							if(index_no == 2){
								int index_no2 = cast_fbgc_object_as_int(TOPN(index_no-1))->content;
								temp = subscript_fbgc_matrix_object(temp,index,index_no2);
								assert(temp != NULL);
								break;
							}

						}
						else {
							cprintf(111,"Not index accessible!\n");
							print_fbgc_object(temp); printf("\n");
							return 0;
						}
						
						assert(temp != NULL);

					}
					//Since this is the top index we can just use top
					//index = cast_fbgc_object_as_int(TOP())->content;
					//cprintf(111,"Son index %d\n",index);

					STACK_GOTO(-index_no);
					PUSH(temp);
					break;
				}
				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = (struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					//Check undefined variable
					if(tmp->content == NULL){
						struct fbgc_object * name = tmp->name;
						cprintf(100,"Undefined variable %s\n",&cast_fbgc_object_as_cstr(name)->content);
						assert(0);
					}
					
					PUSH(tmp->content);
					//PUSH(globals[cast_fbgc_object_as_id_opcode(pc)->loc]);	
				} 
				else if(is_id_flag_LOCAL(pc))  PUSH(GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc));
				break;
			}
			case BREAK:
			{
				struct fbgc_object * loop_obj =  cast_fbgc_object_as_jumper(pc)->content;
				if(loop_obj->type == FOR_BEGIN) STACK_GOTO(-2); //clean the for loop remainders
				pc = cast_fbgc_object_as_jumper(loop_obj)->content;
				break;
			}
			case CONT:
			{
				stack->next = cast_fbgc_object_as_jumper(pc)->content;
				pc = stack;
				break;
			}				
			case RETURN:
			{

				//(LOCALS..., OLD_SCTR,FRAME_CTR,GLOBAL_ARRAY,RETURN_VALUE)
				//Notice that global array is just an indicator to collect properly

				struct fbgc_object * ret = POP();
				while(POP() != globals);
				int old_fctr = fctr;
				fctr = cast_fbgc_object_as_int(TOP())->content;
				stack->next = SECOND();
				sctr = old_fctr;
				//STACK_GOTO(-2);
				
				//##Solve this pc->next problem!!!!!!!!
				pc = stack;

				PUSH(ret);

				//if(ret->type == NIL && pc->next->type != ASSIGN) 
				//	_POP();
				

				recursion_ctr = 0;

				break;
			}			

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
			{
				
				assert(TOP() != NULL && SECOND() != NULL);

				fbgc_token main_tok = MAX(get_fbgc_object_type(TOP()),get_fbgc_object_type(SECOND()));

				if(is_fbgc_binary_op_null(main_tok)){
					cprintf(111,"This type does not support operation.\n");
					return 0;
				}
				struct fbgc_object * res =  call_fbgc_binary_op(main_tok,SECOND(),TOP(),type);
				STACK_GOTO(-1);
				//struct fbgc_object * res =  safe_call_fbgc_binary_op(POP(),POP(),main_tok,type);

				assert(res != NULL);
				SET_TOP(res);		

				break;	

			}
			case COLON:
			{
				struct fbgc_object * x;

				if(SECOND()->type == RANGE){
					x = SECOND();
					cast_fbgc_object_as_range(x)->step = cast_fbgc_object_as_range(x)->end;
					cast_fbgc_object_as_range(x)->end = POP();
					//range_obj_set_step(SECOND(),POP());
				}
				else {
					x = new_fbgc_range_object(SECOND(),TOP());
					STACK_GOTO(-1);
					SET_TOP(x);

					//Old version that works well with GCC compiler
					//PUSH(new_fbgc_range_object(POP(),POP()));
					//SET_TOP( new_fbgc_range_object(TOP(),POP()) );
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

				#ifdef INTERPRETER_DEBUG
		            if(is_id_flag_GLOBAL(pc) ) cprintf(011,"%s{G<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
		            else if(is_id_flag_LOCAL(pc) ) cprintf(011,"%s{L<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
		            else if(is_id_flag_MEMBER(pc) ) cprintf(011,"%s{M<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
		            else if(is_id_flag_SUBSCRIPT(pc) ) cprintf(011,"%s{S<%d>}","ID",cast_fbgc_object_as_id_opcode(pc)->loc);
		            else{
		                cprintf(111,"Undefined ID!\n"); 
		            }
		            cprintf(111,"\n");     				
				#endif

				struct fbgc_object * rhs = POP();

				struct fbgc_object ** lhs = NULL;

				if(is_id_flag_MEMBER(pc)){
					struct fbgc_object * name = cast_fbgc_object_as_id_opcode(pc)->member_name;
					struct fbgc_object * ok = get_set_fbgc_object_member(TOP(),&cast_fbgc_object_as_cstr(name)->content , rhs);

					if(ok == NULL){
						cprintf(100,"[%s] does not support [%s] member assignment\n",object_name_array[TOP()->type],&cast_fbgc_object_as_cstr(name)->content);
						return 0;
					}

					_POP();

					break;
					
				}
				else if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = 
					(struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,cast_fbgc_object_as_id_opcode(pc)->loc);
					lhs = &tmp->content;
				} 
				else if(is_id_flag_LOCAL(pc)){
					lhs = &(GET_AT_FP(cast_fbgc_object_as_id_opcode(pc)->loc));
				}


				if(is_id_flag_SUBSCRIPT(pc)){
					//first pop the number of indexes
					int index_no = cast_fbgc_object_as_int(POP())->content;
					//take index values one by one and finally left last index 
					struct fbgc_object * temp = *lhs;

					int index = 0;
					for(int i = 0; i<index_no-1; i++){
						if(temp->type == TUPLE){
							index = cast_fbgc_object_as_int(TOPN(index_no-i))->content;
							temp = get_object_in_fbgc_tuple_object(temp,index);
						}
						else {
							cprintf(111,"Not index accessable!\n");
							return 0;
						}
					}

					//Since this is the top index we can just use top
					index = cast_fbgc_object_as_int(TOP())->content;
					lhs = get_object_address_in_fbgc_tuple_object(temp,index);

					//temp solution but it looks very bad..
					/*if(type != ASSIGN){
						fbgc_token op_type = R_SHIFT + type - R_SHIFT_ASSIGN;
						assert(lhs != NULL && *lhs != NULL);

						fbgc_token main_tok = MAX(get_fbgc_object_type( (*lhs) ),get_fbgc_object_type(rhs));
						rhs = call_fbgc_binary_op(main_tok,*lhs,rhs,op_type);
						assert(rhs != NULL);					
					}

					
					if(temp->type == TUPLE) 
						set_object_in_fbgc_tuple_object(temp,rhs,index);
					else 
						assert(0);*/

					STACK_GOTO(-index_no);
					//break;
				}

				//Call other assignment types
				if(type != ASSIGN)
				{
					fbgc_token op_type = R_SHIFT + type - R_SHIFT_ASSIGN;
					assert(lhs != NULL && *lhs != NULL);

					fbgc_token main_tok = MAX(get_fbgc_object_type( (*lhs) ),get_fbgc_object_type(rhs));
					rhs = call_fbgc_binary_op(main_tok,*lhs,rhs,op_type);
					assert(rhs != NULL);					
				}

				/*if(is_id_flag_SUBSCRIPT(pc)){
					*lhs = rhs;
					STACK_GOTO(-index_no);
				}*/

				*lhs = rhs;

				if(is_id_flag_PUSH_ITSELF(pc)){
					PUSH(rhs);
				}	
				
				break;
			}
			case LEN:
			{
				SET_TOP( get_length_fbgc_object(TOP()) );
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
				struct fbgc_object * cond = POP();
				char c = convert_fbgc_object_to_logic(cond);
				if(!c){
					pc = cast_fbgc_object_as_jumper(pc)->content;
				}
				break;
			}
			case FOR_BEGIN:
			{	

				if(TOP()->type != INT){
					cprintf(100,"For cannot start , type %s\n",object_name_array[TOP()->type]);
					assert(0);
				}

				int i = cast_fbgc_object_as_int(TOP())->content;
				struct fbgc_object * seq_ob = SECOND();

				if(i == -1){
					//New construciont of the for loop
					//change the top put the new iterator
					SET_TOP(new_fbgc_int_object(i = 0));
				}
				else {
					//this is iterator and we will increase it inplace
					i = ++(cast_fbgc_object_as_int(TOP())->content);
				}


				if(seq_ob->type == RANGE){
					seq_ob = get_element_in_fbgc_range_object(seq_ob,i);					
				}
				else if(seq_ob->type == STRING){
					seq_ob = subscript_fbgc_str_object(seq_ob,i,i+1);
				}
				else if(seq_ob->type == TUPLE){
					seq_ob = get_object_in_fbgc_tuple_object(seq_ob,i);
				}
				else assert(0);


				if(seq_ob != NULL){
					PUSH(seq_ob);
				}
				else{
					//finish the for loop
					//pop iterator and sequence object
					STACK_GOTO(-2);
					pc = cast_fbgc_object_as_jumper(pc)->content;
				}


				//return 0;
				break;
			}
			case FUN_CALL:
			{
				
				struct fbgc_fun_object * funo = cast_fbgc_object_as_fun(POP());
				int arg_no = cast_fbgc_object_as_int(POP())->content;
				//assert(funo->base.type == FUN || funo->base.type == CFUN);

				if(funo->base.type == CFUN){
					
					STACK_GOTO(-arg_no);
					struct fbgc_object * res = cfun_object_call(funo, sp+sctr, arg_no);
					if(res != NULL) PUSH(res);
					break;

					// In order to increase speed, DELETE new tuple creation it causes 2sec for 100,000 print('ffdfds') code
					
					//return 0;
				}

				if(last_called_function == (struct fbgc_object * ) funo) recursion_ctr++;
				else {
					last_called_function = (struct fbgc_object *) funo;
					recursion_ctr = 0;
				}

				if(funo->no_arg != arg_no ){
					cprintf(100,"Argument match error! funo->arg %d, arg_no %d\n",funo->no_arg,arg_no);
					return 0;
				}

				STACK_GOTO(funo->no_locals - arg_no);
				//save our first next operation after this function call
				//After returning the value we will take this space and run the main code
				PUSH(pc->next);
				//hold old frame pointer location
				PUSH(new_fbgc_int_object(fctr));
				//hold old position of sp with fp, assume that args already pushed into stack
				fctr = sctr-funo->no_locals-2;
				//execute function
				//--------------------------------
				//This next push added after first version of the fun call operations
				//when we have a for loop, it changes stack and return simply makes three basic pop operations
				//but for loops leave additional objects in the stack so we need to push something as an indicator that 
				//return will pop everything till see this object.
				//Nothing from the user side can push globals so we will push global key-value array
				PUSH(globals);
				//--------------------------------

				//##Solve this pc->next problem!!!!!!!!
				stack->next = cast_fbgc_object_as_fun(funo)->code;
				pc = stack;
				break;
			}
			case METHOD_CALL:
			{
				
				//increase arg_no so we can count object as well
				int arg_no = cast_fbgc_object_as_int(POP())->content;
				arg_no++;

				struct fbgc_object * name = cast_fbgc_object_as_id_opcode(pc)->member_name;

				struct fbgc_object * method = get_set_fbgc_object_member(TOPN(arg_no),&cast_fbgc_object_as_cstr(name)->content , NULL);
				
				if(method->type == CFUN){
					STACK_GOTO(-arg_no);
					struct fbgc_object * res = cfun_object_call(method, sp+sctr, arg_no);
					if(res!= NULL)
						PUSH(res);
				}
				else assert(0);

				break;
			}
			case BUILD_TUPLE:
			{	

				int tuple_size = cast_fbgc_object_as_int(pc)->content;
				struct fbgc_object * to = new_fbgc_tuple_object(tuple_size);
				size_fbgc_tuple_object(to) = tuple_size;
				
				//XXXXXXXXXXXXXXXXXX
				//Instead of pop, give the starting address and size
				//XXXXXXXXXXXXXXXXXXX 	
				while(--tuple_size >= 0){
					set_object_in_fbgc_tuple_object(to,POP(),tuple_size);
				}

				PUSH(to);
				break;
			}
			case BUILD_MATRIX:
			{	
				
				if(TOP()->type == RANGE)
				{
					SET_TOP(new_fbgc_matrix_object_from_range(TOP()));
					break;
				}


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
				struct fbgc_object * m = matrix_creation_from_stack(sp+sctr-ctr ,ctr, msize, row);
				assert(m != NULL);
				//print_fbgc_matrix_object(m);



				STACK_GOTO(-ctr);
				PUSH(m);

				break;
				//return 0;
			}
			case POP_TOP:
			{
				_POP();
				break;
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
