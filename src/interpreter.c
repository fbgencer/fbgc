#include "fbgc.h"



struct iter_function_ptr_struct{
	struct fbgc_object * (* function)(struct fbgc_object *, int,struct fbgc_object *);
};

#define PROGRAM_STACK_SIZE 100
uint8_t interpreter(struct fbgc_object ** field_obj){
	current_field = * field_obj;
	struct fbgc_ll * head = _cast_llbase_as_ll( cast_fbgc_object_as_field(*field_obj)->head );

	//Drop the tail object, make it null so if we want to run a function we can just use this
	head->tail->next->next = NULL; 
	
	struct fbgc_object * stack = new_fbgc_tuple_object(PROGRAM_STACK_SIZE);
	struct fbgc_object ** sp = content_fbgc_tuple_object(stack);
	if(run_code(head->base.next, sp,0, -1) != NULL){
		return 1;	
	}
	return 0;
}

struct fbgc_object * run_code(struct fbgc_ll_base * pc, struct fbgc_object ** sp,int sctr, int fctr){

	//struct fbgc_ll_base * pc = code; //program counter
	
	struct fbgc_object * current_scope = NULL;

	struct fbgc_object * last_called_function = NULL;
	size_t recursion_ctr = 0;

	struct fbgc_ll_base * __dummy = _new_fbgc_ll_base(NIL);

#define STACK_GOTO(i) 	(sctr += i)
#define PUSH(x)			(*(sp+ sctr++ ) = (x))
#define POP()			(sp[--sctr]) /**< Detailed description after the member */
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
#define RECURSION_LIMIT 1000


	_info("==========[INTERPRETER]==========\n");

	for(int i = 0;  (pc != NULL); i++){

		/*if(i > 1000){
			printf("Too much iteration. Breaking the loop\n");
			goto INTERPRETER_ERROR_LABEL;
		}*/

		if(recursion_ctr>RECURSION_LIMIT){
			printf("Reached Recursion limit!\n");
			break;
		}

		if(sctr > PROGRAM_STACK_SIZE){
			printf("Stack Overflow!");
			break;
		}

		fbgc_token type = get_fbgc_object_type(pc);
		
		_print("################ [%d] = {%s} ########################\n",i,_obj2str(pc));

		switch(type){
			case CONSTANT:{
				PUSH(_cast_llbase_as_llconstant(pc)->content);
				break;
			}
			case ROW:{
				PUSH(pc);
				break;
			}
			case IDENTIFIER:{	

				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = (struct fbgc_identifier *) 
						get_address_in_fbgc_array_object(cast_fbgc_object_as_field(current_field)->locals,
							_cast_fbgc_object_as_llidentifier(pc)->loc);

					//Check undefined variable
					if(tmp->content == NULL){
						struct fbgc_object * name = tmp->name;
						printf("Undefined variable '%s'\n",content_fbgc_cstr_object(name));
						//fbgc_error(_FBGC_UNDEFINED_IDENTIFIER_ERROR,-1);
						return 0;
					}
					
					PUSH(tmp->content);
				} 
				else if(is_id_flag_LOCAL(pc))  PUSH(GET_AT_FP(_cast_fbgc_object_as_llidentifier(pc)->loc));
				else if(is_id_flag_MEMBER(pc)){
					//if(TOP()->type == CSTRING)
					struct fbgc_object * member = POP();
					struct fbgc_object * self = POP();
					struct fbgc_object * object = get_set_fbgc_object_member(self,content_fbgc_cstr_object(member), NULL);

					if(object == NULL){
						printf("Object does not have %s member\n",content_fbgc_cstr_object(member));
						goto INTERPRETER_ERROR_LABEL;
					}

					PUSH(object);

					if(is_id_flag_MEMBER_METHOD(pc)){
						//if(self->type != FIELD)
						PUSH(self);
					}
				}
				else if(is_id_flag_CLASS(pc)){

					
					struct fbgc_identifier * tmp = (struct fbgc_identifier *) 
						get_address_in_fbgc_array_object(cast_fbgc_object_as_class(current_scope)->locals
								,_cast_fbgc_object_as_llidentifier(pc)->loc);

					//Check undefined variable
					if(tmp->content == NULL){
						struct fbgc_object * name = tmp->name;
						printf("Undefined variable '%s'\n",content_fbgc_cstr_object(name));
						//fbgc_error(_FBGC_UNDEFINED_IDENTIFIER_ERROR,-1);
						return 0;
					}
					
					PUSH(tmp->content);					
				}
				break;
			}
			case BREAK:{

				struct fbgc_ll_base * loop_obj =  _cast_llbase_as_lljumper(pc)->content;
				if(loop_obj->type == FOR) {
					assert(0);
					STACK_GOTO(-2); //clean the for loop remainders
				}
				pc = _cast_llbase_as_lljumper(loop_obj)->content;
				break;
			}
			case CONT:{

				__dummy->next = _cast_llbase_as_lljumper(pc)->content;
				pc = __dummy;

				break;
			}				
			case RETURN:{

				//(LOCALS..., OLD_SP,FRAME_CTR,scope,RETURN_VALUE)
				//Notice that global array is just an indicator to collect properly

				struct fbgc_object * ret = POP();
				//while(POP() != cast_fbgc_object_as_field(*field_obj)->locals);

				current_scope = POP();

				
				//This is a new feature to solve class scope issue
				//Now after globals we will have field object to assign correct scopes
				
				int old_fctr = fctr;
				fctr = cast_fbgc_object_as_int(TOP())->content;
				__dummy->next = SECOND();
				pc = __dummy;
				sctr = old_fctr; // Instead of using stack goto we'll manipulate stack with sctr
				//_print_object("sctr deneme",TOP());
				//_print("sctr:%d\n",sctr);
				SET_TOP(ret);

				recursion_ctr = 0;
				break;
			}			
			case COLON:{

				if(SECOND()->type == STRING){
					//This is named tuple entry
					;
				}

				struct fbgc_object * x = new_fbgc_range_object(SECOND(),TOP());
				_POP();
				SET_TOP(x);
				break;
			}
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
			{

				fbgc_token main_tok = MAX(get_fbgc_object_type(TOP()),get_fbgc_object_type(SECOND()));
				if(main_tok > INSTANCE && MIN(get_fbgc_object_type(TOP()),get_fbgc_object_type(SECOND())) < LOGIC) 
					goto INTERPRETER_ERROR_LABEL;

				struct fbgc_object * res =  call_fbgc_operator(main_tok,SECOND(),TOP(),type);

				
				if(res->type == FUN){
					//ornek olarak burada dursun, fun objyi return kendisi sildiği için burada çağıramıyoruz
					//Ama fonks basip gecici bir cozum elde edilebilir
					//Ok now lets cheat
					struct fbgc_object * arg2 = POP();
					struct fbgc_object * arg1 = POP();
					PUSH(res);
					PUSH(arg1);
					PUSH(arg2);

					_print("\n\nOperator overload.\n\n");
					//(LOCALS..., OLD_SP,FRAME_CTR,scope,RETURN_VALUE)
					STACK_GOTO(cast_fbgc_object_as_fun(res)->no_locals - 2);
					PUSH(NULL);
					PUSH(new_fbgc_int_object(fctr));
					
					PUSH(current_scope);
					struct fbgc_object ** __sp = sp+sctr-6;
					
					_print("sctr:%d\n",sctr);
					res = run_code(cast_fbgc_object_as_fun(res)->code,sp,sctr,sctr-cast_fbgc_object_as_fun(res)->no_locals-3);

					
					_println_object("Result:",res);
					sctr -= 4;

					_println_object("overload return value:\n",res);
				}

				_POP();
				//struct fbgc_object * res =  safe_call_fbgc_binary_op(POP(),POP(),main_tok,type);

				assert(res != NULL);
				SET_TOP(res);		

				break;	

			}
			case EXCLAMATION:
			case TILDE:		
			case UPLUS:
			case UMINUS:{
				
				assert(TOP() != NULL);
				struct fbgc_object * res =  call_fbgc_operator(get_fbgc_object_type(TOP()),TOP(),NULL,type);
				assert(res != NULL);
				SET_TOP(res);
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

				_info("Assignment op [%x]ID<%d>\n",get_ll_identifier_flag(pc),get_ll_identifier_loc(pc));

				struct fbgc_object * rhs = POP();
				struct fbgc_object ** lhs = NULL;

				if(is_id_flag_GLOBAL(pc)){
					struct fbgc_identifier * tmp = 
					(struct fbgc_identifier *) get_address_in_fbgc_array_object(cast_fbgc_object_as_field(current_field)->locals,_cast_fbgc_object_as_llidentifier(pc)->loc);
					lhs = &tmp->content;
				}
				else if(is_id_flag_CLASS(pc)){
					
					struct fbgc_identifier * tmp = 
					(struct fbgc_identifier *) get_address_in_fbgc_array_object(cast_fbgc_object_as_class(current_scope)->locals,_cast_fbgc_object_as_llidentifier(pc)->loc);
					lhs = &tmp->content;
				}
				else if(is_id_flag_MEMBER(pc)){
					struct fbgc_object * member = POP();
					struct fbgc_object * object = get_set_fbgc_object_member(TOP(),content_fbgc_cstr_object(member) , rhs);

					if(object == NULL){
						printf("[%s] does not support [%s] member assignment\n",object_name_array[TOP()->type],content_fbgc_cstr_object(member));
						goto INTERPRETER_ERROR_LABEL;
					}

					_POP();
					break;

					//why do we have break here? this type of operation only supports assign, not the other types.
					//TODO make it general					
				}
				else if(is_id_flag_LOCAL(pc)){
					lhs = &(GET_AT_FP(_cast_fbgc_object_as_llidentifier(pc)->loc));
				}


				//Call other assignment types
				if(type != ASSIGN)
				{
					fbgc_token op_type = RSHIFT + type - RSHIFT_ASSIGN;
					assert(lhs != NULL && *lhs != NULL);

					fbgc_token main_tok = MAX(get_fbgc_object_type( (*lhs) ),get_fbgc_object_type(rhs));
					rhs = call_fbgc_operator(main_tok,*lhs,rhs,op_type);
					assert(rhs != NULL);				
				}

				*lhs = rhs;

				//_println_object("assignment done lhs = ",*lhs);
				

				
				// for x = y= 5
				if(is_id_flag_PUSH_ITSELF(pc)){
					PUSH(rhs);
				}	
				
				break;
			}
			case ASSIGN_SUBSCRIPT:
			{
				struct fbgc_object * rhs = POP();
				int index_no = cast_fbgc_object_as_int(POP())->content;
				struct fbgc_object ** lhs = NULL;
				
				struct fbgc_object * temp = TOPN(index_no+1);
				int index = 0;
				int stck_goto = index_no;
				while(index_no > 0){
					if(temp->type == TUPLE){
						index = cast_fbgc_object_as_int(TOPN(index_no))->content;
						if(index_no > 1){
							temp = get_object_in_fbgc_tuple_object(temp,index);
							index_no--;
						}
						else
						{
							index = cast_fbgc_object_as_int(TOPN(index_no))->content;
							_cprint(110,"Here index %d\n",index);
							lhs = (struct fbgc_object **)get_object_address_in_fbgc_tuple_object(temp,index);
							break;
						}
					}
					else if(temp->type == STRING){
						assert(0);
						/*index = cast_fbgc_object_as_int(TOPN(index_no))->content;
						lhs =  &set_object_in_fbgc_str_object(temp,index,index+1,rhs);
						rhs = *lhs;
						*/
						break;
					}
					else if(temp->type == MATRIX){
						assert(index_no > 1);
						index = cast_fbgc_object_as_int(TOPN(index_no))->content;
						int index_no2 = cast_fbgc_object_as_int(TOPN(--index_no))->content;
						lhs = set_object_in_fbgc_matrix_object(temp,index,index_no2,rhs);
						//rhs = lhs; ???
						assert(lhs != NULL);
						break;
					}
					else {
						printf("Not index accessable!\n");
						goto INTERPRETER_ERROR_LABEL;
					}
				}
				*lhs=rhs;
				break;

			}

			case LEN:
			{
				SET_TOP( get_length_fbgc_object(TOP()) );
				break;
			}			
			case JUMP:
			{
				pc = _cast_llbase_as_lljumper(pc)->content;
				break;
			}		
			case IF:
			case ELIF:
			case WHILE:
			{
				if(!convert_fbgc_object_to_logic(TOP())){
					pc = _cast_llbase_as_lljumper(pc)->content;
				}
				_POP();
				break;
			}
			case FOR_BEGIN:
			{


				struct fbgc_object * seq_ob = TOP();

				struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) 
				new_fbgc_cstruct_object(sizeof(struct iter_function_ptr_struct), NULL);
				struct iter_function_ptr_struct * ifps = (struct iter_function_ptr_struct *) so->cstruct;
				
				PUSH((struct fbgc_object *) so);
				
				if(seq_ob->type == RANGE){

					fbgc_token range_type = get_fbgc_range_object_iter_type(seq_ob);

					if(range_type == INT){
						ifps->function = get_int_element_in_fbgc_range_object;
						PUSH(new_fbgc_int_object(888));  	
					}
					else if(range_type == DOUBLE){
						ifps->function = get_double_element_in_fbgc_range_object;
						PUSH(new_fbgc_double_object(8.8));
					}
					//=====================================================
					//check the top is it sequential obj ?
					// this depends on seq obj type, if it's a tuple we do not need it
					//if range-int then it is int, range-double: double
					// string : string etc..
							
				}
				else if(seq_ob->type == STRING){
					ifps->function = get_char_from_fbgc_str_object;
					PUSH(new_fbgc_str_object("*"));
				}
				else if(seq_ob->type == TUPLE){
					ifps->function = __get_object_in_fbgc_tuple_object;
					//holder of tuple will never be used, we are sure of it but it is not a good idea to push null object
					PUSH(NULL);
				
				}

				//this is always the same!
				PUSH(new_fbgc_int_object(0)); //iterator starter
				break;
			}
			case FOR:
			{	
				//Assume that FOR_BEGIN checked everything so no need to check in this case
				//if we check something, it may introduce latency

				struct fbgc_object * seq_ob = TOPN(4);
				//this is out index iterator and we will increase it inplace
				int i = (cast_fbgc_object_as_int(TOP())->content)++;			
				struct fbgc_object * holder = SECOND();
				
				struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(TOPN(3));
				struct iter_function_ptr_struct * fs = (struct iter_function_ptr_struct *) so->cstruct; 
				
				//call the pre-determined function				
				seq_ob = fs->function(seq_ob,i,holder);


				if(seq_ob != NULL){
					PUSH(seq_ob);
				}
				else{
					//finish the for loop, everything worked normally
					//pop iterator,holder,sequence object and function struct
					STACK_GOTO(-4);
					pc = _cast_llbase_as_lljumper(pc)->content;
				}

				break;
			}
			case FUN_CALL:{

				int arg_no = _cast_llbase_as_llopcode_int(pc)->content;
				
				struct fbgc_object * funo = TOPN(arg_no+1);

				struct fbgc_object * last_scope = current_scope;
				//Geçici olarak burada..
				if(funo->type == CLASS){
					_info("Calling class constructor...\n");

					struct fbgc_object * new_inst = new_fbgc_instance_object(funo);

					//instance aldıktan sonra bu class'ın const'u var mi bunu soralım

					struct fbgc_object * constructor = get_set_fbgc_object_member(funo,"init",NULL);
					if(constructor == NULL){
						if(arg_no != 0){
							//this is also an error
							assert(0);
						}
						//No definition of constructor 
						_POP(); //pop cls object
						PUSH(new_inst);
						break;
					}else {
						
						//Lets just not support this part.
						arg_no++;
						funo = constructor;
						PUSH(new_inst);
						//assert(0);

						/*++arg_no;
						SET_TOPN(arg_no,new_inst);
						//print_fbgc_object(TOPN(arg_no));
						//print_fbgc_object(constructor);
						funo = constructor;
						//return 0;
						cprintf(100,"sctr %d\n",sctr);
						if(cast_fbgc_object_as_fun(funo)->no_arg != arg_no) {
							printf("Argument match error! funo->arg %d, arg_no %d\n",cast_fbgc_object_as_fun(funo)->no_arg,arg_no);
							goto INTERPRETER_ERROR_LABEL;		
						}

						STACK_GOTO(cast_fbgc_object_as_fun(funo)->no_locals - arg_no);
						PUSH(pc->next);
						PUSH(new_fbgc_int_object(fctr));
						fctr = sctr-cast_fbgc_object_as_fun(funo)->no_locals-2;
						PUSH(last_scope);
						PUSH(cast_fbgc_object_as_field(current_scope)->locals);
						__dummy->next = cast_fbgc_object_as_fun(funo)->code;
						pc = __dummy;
						break;*/
					}
				}

				if(funo->type != FUN && funo->type != CFUN){
					printf("Object is not callable, type %s\n",object_name_array[funo->type]);
					goto INTERPRETER_ERROR_LABEL;
				}
				

				if(funo->type == CFUN){
					STACK_GOTO(-arg_no);
					struct fbgc_object * res = cfun_object_call(funo, sp+sctr, arg_no);
					//#define cfun_object_call(cfuno,args,argc)(cast_fbgc_object_as_cfun(cfuno)->function(args,argc))
					_POP(); // pop cfun object 

					if(res == NULL){
						//const struct fbgc_cfunction * cc = cast_fbgc_object_as_cfun(funo);
						//cprintf(100,"Error in function %s\n",cc->name);
						goto INTERPRETER_ERROR_LABEL;
					} 
						
					//XXX solve this issue
					if(res != __fbgc_nil)
						PUSH(res);
					break;
				}


				if(cast_fbgc_object_as_fun(funo)->no_arg < 0){
					//Is variadic function ?,
					int nvararg = arg_no + cast_fbgc_object_as_fun(funo)->no_arg + 1;
					struct fbgc_object * argtple = new_fbgc_tuple_object_from_tuple_content(sp+sctr-nvararg,nvararg);

					SET_TOPN(nvararg--,argtple);
					STACK_GOTO( -nvararg );
					arg_no = -cast_fbgc_object_as_fun(funo)->no_arg;
					//print_fbgc_object(argtple);
					
					//return;
				}
				else if(cast_fbgc_object_as_fun(funo)->no_arg != arg_no ){
					printf("Argument match error! funo->arg %d, arg_no %d\n",cast_fbgc_object_as_fun(funo)->no_arg,arg_no);
					goto INTERPRETER_ERROR_LABEL;
				}

				if(last_called_function == funo) ++recursion_ctr;
				else {
					last_called_function = funo;
					recursion_ctr = 0;
				}

				STACK_GOTO(cast_fbgc_object_as_fun(funo)->no_locals - arg_no);
				//save our first next operation after this function call
				//After returning the value we will take this space and run the main code
				PUSH(pc->next);
				//hold old frame pointer location
				PUSH(new_fbgc_int_object(fctr));
				//hold old position of sp with fp, assume that args already pushed into stack
				fctr = sctr-cast_fbgc_object_as_fun(funo)->no_locals-2;
				//execute function
				//--------------------------------
				//This next push added after first version of the fun call operations
				//when we have a for loop, it changes stack and return simply makes three basic pop operations
				//but for loops leave additional objects in the stack so we need to push something as an indicator that 
				//return will pop everything till see this object.
				//Nothing from the user side can push globals so we will push global key-value array
				PUSH(last_scope);
				
				//xx PUSH(cast_fbgc_object_as_field(current_scope)->locals);
				
				//--------------------------------

				//##Solve this pc->next problem!!!!!!!!
				__dummy->next = cast_fbgc_object_as_fun(funo)->code;
				pc = __dummy;
				break;
			}
			case BUILD_TUPLE:
			{	

				int tuple_size = _cast_llbase_as_llopcode_int(pc)->content;
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


				int ctr = _cast_llbase_as_llopcode_int(pc)->content;
				if(ctr == 1 && TOP()->type == MATRIX) break;


				int row = 0;
				int col = 0;
				int msize = 0;

				//traverse reverse!

				//cprintf(111,"sctr %d, ctr %d\n",sctr,ctr);
				/*cast_fbgc_object_as_tuple(stack)->size = sctr;//sp - content_fbgc_tuple_object(stack);
				cprintf(111,"\n==============Stack==========================\n");
				print_fbgc_object(stack);*/				
				
				fbgc_token matrix_sub_type = DOUBLE;

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
						case COMPLEX:{
							++msize;
							matrix_sub_type = COMPLEX;
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
							printf("Type %s in matrix is cannot be located\n",object_name_array[TOPN(i+1)->type]);
							goto INTERPRETER_ERROR_LABEL;
						}
					}	
					
					if(i < sctr && TOPN(i+1)->type == ROW){
						++row;

						ctr += 1+ _cast_llbase_as_llopcode_int(TOPN(i+1))->content;
						//cprintf(010,"Top is row, msize: %d, ctr :%d ,row :%d\n",msize,ctr,row);
						++i;
					}
				}

				//cprintf(010,"Output of loop, row:%d , col :%d ctr:%d\n",row,old_col,ctr);
				
				//return 0;

				
				//struct fbgc_object * m = new_fbgc_matrix_object(ctr-row);
				struct fbgc_object * m = matrix_creation_from_stack(sp+sctr-ctr ,ctr, msize, row,matrix_sub_type);
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
			case LOAD_SUBSCRIPT:
			{
				int index_no = cast_fbgc_object_as_int(POP())->content;
				//take index values one by one and finally left last index 
				struct fbgc_object * temp = TOPN(index_no+1);
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
						temp = get_object_in_fbgc_str_object(temp,index,index+1);
					}
					else if(temp->type == MATRIX){
						if(index_no == 2){
							int index_no2 = cast_fbgc_object_as_int(TOPN(index_no-1))->content;
							temp = get_object_in_fbgc_matrix_object(temp,index,index_no2);
							assert(temp != NULL);
							break;
						}

					}
					else {
						printf("Not index accessible!\n");
						print_fbgc_object(temp); printf("\n");
						return 0;
					}
					
					assert(temp != NULL);

				}

				STACK_GOTO(-index_no-1);
				PUSH(temp);
				break;
			}
			case CLASS_CALL:{

				int arg_no = _cast_llbase_as_llopcode_int(pc)->content;
				FBGC_LOGV(INTERPRETER,"CLASS_CALL : Arg no :%d\n",arg_no);

				struct fbgc_object * cls = TOPN(arg_no+1);
				
				while(arg_no--){
					inherit_from_another_class(cls,POP());
				}
				cls = POP(); //RETURN'de class kaldigi icin yazdik, degistir

				//print_fbgc_class_object(cls);
				struct fbgc_object * last_scope = current_scope;
				current_scope = cls;
				PUSH(pc->next);
				PUSH(new_fbgc_int_object(fctr));
				fctr = sctr-1;
				PUSH(last_scope);
				//PUSH(cast_fbgc_object_as_field(*field_obj)->locals);
				__dummy->next = cast_fbgc_object_as_class(cls)->code;
				pc = __dummy;

				break;
			}
			
			default:
			{
				cprintf(101,"Undefined token in interpreter [%s]\n",object_name_array[type]);
				return 0;						
			}
		}


		//cast_fbgc_object_as_tuple(stack)->size = sctr; //sp - content_fbgc_tuple_object(stack);
			
		_info("{");
		for(size_t i = 0; i<sctr; i++){
			_print_object("",sp[i]);
			_cprint(011,", ");
		}
		_info("}\n");
		_info("~~~~~~Field Globals~~~~~\n");

		struct fbgc_object * globals;
		if(current_scope != NULL && current_scope->type == CLASS) globals = cast_fbgc_object_as_class(current_scope)->locals;
		else globals = cast_fbgc_object_as_field(current_field)->locals;

		for(int i = 0; i<size_fbgc_array_object(globals); i++){
			struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(globals,i);
			_print_object("{",temp_id->name);
			_print_object(":",temp_id->content);
			_cprint(010,"}");
		}
		_info("\n==============================================\n\n");
		

		FETCH_NEXT();

	}

	return sctr > 0 ? TOP():NULL;

	INTERPRETER_ERROR_LABEL:
		cprintf(100,"Execution stopped!\n");
		return NULL;

}
