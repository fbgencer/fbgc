// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"



struct iter_function_ptr_struct{
	struct fbgc_object * (* function)(struct fbgc_object *, int,struct fbgc_object *);
	struct fbgc_object * seq_ob;
	struct fbgc_object * holder;
	int iterator;
};

#define PROGRAM_STACK_SIZE 100


struct fun_call_packet{
	// maybe we can add later, struct fbgc_object * fun; //!< Called function
	struct fbgc_ll_base * last_pc;
	struct fbgc_object * last_scope;
	int frame_ctr;
	uint8_t pop_number;
};

struct fbgc_object * prepare_fun_call_packet(struct fbgc_ll_base * _pc, struct fbgc_object * _last_scope, int _frame_ctr , uint8_t _pop_number){
		struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) 
		new_fbgc_cstruct_object(sizeof(struct fun_call_packet), NULL);
		struct fun_call_packet * fcp = (struct fun_call_packet *) so->cstruct;
		
		//save our first next operation after this function call
		//After returning the value we will take this space and run the main code
		fcp->last_pc = _pc;
		fcp->frame_ctr = _frame_ctr;
		fcp->last_scope = _last_scope;
		fcp->pop_number = _pop_number;

		return (struct fbgc_object *) so;
}


struct interpreter_packet * global_interpreter_packet = NULL;



uint8_t interpreter(struct fbgc_object ** field_obj){
	current_field = * field_obj;
	struct fbgc_ll * head = _cast_llbase_as_ll( cast_fbgc_object_as_field(*field_obj)->code );

	//Drop the tail object, make it null so if we want to run a function we can just use this
	head->tail->next->next = NULL; 
	
	//This is the stack of all operations in this field object
	struct fbgc_object * stack = new_fbgc_tuple_object(PROGRAM_STACK_SIZE);
	//sp is just stack pointer
	struct fbgc_object ** sp = content_fbgc_tuple_object(stack);
	struct interpreter_packet ip = {	
			sp, //sp
			NULL, //current_scope
			head->base.next,  // pc
			0,//sctr
			-1 //fctr
	};
	//now run the code for
	if(run_code(&ip) != NULL){
		return 1;	
	}
	return 0;
}

struct fbgc_object * run_code(struct interpreter_packet * ip){

	global_interpreter_packet = ip;

	struct fbgc_object * current_scope = NULL;
	struct fbgc_object * last_called_function = NULL;
	size_t recursion_ctr = 0;

#define sctr ip->sctr
#define fctr ip->fctr
#define pc (ip->pc)
#define sp (ip->sp)	

// Stack layout for normal operations: 
// 1.sctr starts from zero so pushing shifts sctr and puts the object into 0th place. After three pushing the layout looks like the following
// ob1, ob2, ob3,	
//				 ^----sctr
//In order to pop 3 object immediately STACK_GOTO(-4) takes back sctr from 4 to zero. So the number of pop + 1 is needed for stack_goto
#define STACK_GOTO(i) 	(sctr += i)				//!< Just changes sctr poistion, instead of running pop this macro gives easy way to handle sctr 
#define PUSH(x)			(*(sp+ sctr++ ) = (x))	//!< Pushes object at sp and increases sctr by one
#define POP()			(sp[--sctr]) 			//!< Pops object at sp and decreases sctr by one
#define _POP()			(--sctr)				//!< Without returning object just decreases sctr by one
#define TOP()			(sp[sctr-1])			//!< Returns the last pushed element 
#define SECOND()		(sp[sctr-2])			//!< Returns the element before the last pushed element 
#define THIRD()			(sp[sctr-3])			//!< ditto
#define TOPN(i)			(sp[sctr-(i)])			//!< ditto
#define SET_TOP(x)		(sp[sctr-1] = (x))		//!< Changes top object to given object
#define SET_SECOND(x)	(sp[sctr-2] = (x))		//!< Changes second top object to given object
#define SET_THIRD(x)	(sp[sctr-3] = (x))		//!< Changes third top object to given object
#define SET_TOPN(i, x)	(sp[sctr-(i)] = (x))	//!< Changes Nth top object to given object

//FRAME MACROS
#define GET_AT_FP(n)		(sp[fctr+(n)])		//!< Detailed description after the member
#define SET_AT_FP(n, x)	(sp[fctr+(n)] = (x))	//!< Detailed description after the member
#define FETCH_NEXT()(pc = pc->next)				//!< Detailed description after the member,

#define RECURSION_LIMIT 1000					//!< Recursion depth of function calls


	FBGC_LOGV(INTERPRETER,"==========[INTERPRETER]==========\n");
	int i = 0; 
	while(pc != NULL){

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

	FBGC_LOGV(INTERPRETER,"################ [%d] = {%s} ########################\n",i++,lltp2str(pc));

	//pc is just mnemonic for program counter, basically it is just an iterator for the code that this function is executing
	fbgc_token type = pc->type;
	switch(type){
		case CONSTANT:{
			//Constant means that push the contents of constant into stack
			PUSH(_cast_llbase_as_llconstant(pc)->content);
			break;
		}
		case ROW:{
			PUSH((struct fbgc_object *)pc);
			break;
		}
		case IDENTIFIER:{	

			if(is_id_flag_array_accessible(pc)){
				//If global then ask for field, if it is class then ask for current scope
				struct fbgc_object * variable_map = is_id_flag_GLOBAL(pc) ? 
									cast_fbgc_object_as_field(current_field)->variables:
									cast_fbgc_object_as_class(current_scope)->variables;

				

				struct fbgc_object * key = get_object_in_fbgc_tuple_object(fbgc_symbols,_cast_fbgc_object_as_llidentifier(pc)->loc);
				struct fbgc_object * value = fbgc_map_object_lookup(variable_map,key);

				//In parsing phase content of identifiers are set the NULL so it means they did not defined by the user
				if(value == NULL){
					printf("Undefined variable '%s'\n",content_fbgc_str_object(key));
					//fbgc_error(_FBGC_UNDEFINED_IDENTIFIER_ERROR,-1);
					return 0;
				}
								
				PUSH(value);
			} 

			else if(is_id_flag_LOCAL(pc)){
				//Local flag is for function locals, when args of a function pushed into stack their places are not changed 
				//rather we change fctr to assign the location where they start
				//For example fctr+0 gives 0th local variable 
				//FP macros must be used

				PUSH(GET_AT_FP(_cast_fbgc_object_as_llidentifier(pc)->loc));
			}
			else if(is_id_flag_MEMBER(pc)){
				
				FBGC_LOGV(INTERPRETER,"Member flag\n");
				//For the entries x.y, x pushed as identifier object and y is a cstr object so first member name is top value

				struct fbgc_object * member = POP();
				struct fbgc_object * self = POP();

				struct fbgc_object * object;

				if(is_id_flag_MEMBER_METHOD(pc) == 0){
					object = get_set_fbgc_object_member(self,content_fbgc_str_object(member), NULL);
					if(object == NULL){
						//! @todo more meaningfull error messages
						printf("Object does not have %s member\n",content_fbgc_str_object(member));
						goto INTERPRETER_ERROR_LABEL;
					}					
					PUSH(object);
				}
				else{
					object = get_fbgc_object_method(self,content_fbgc_str_object(member));
					if(object == NULL)
						object = get_set_fbgc_object_member(self,content_fbgc_str_object(member), NULL);

					if(object == NULL){
						//! @todo more meaningfull error messages
						printf("Object does not have %s method\n",content_fbgc_str_object(member));
						goto INTERPRETER_ERROR_LABEL;
					}

					if(self->type != CMODULE){
						PUSH(object);
						PUSH(self);
					}
					else{
						//When we call a cfun from cmodule it pushed cmodule as a first object
						//but for normal cfun calls we do not provide cmodule
						PUSH(self);
						PUSH(object);
						
					}					
				}



				//! @todo there is no need to put this in identifier, instead making new opcode is necessary 
			}
			break;
		}
		case BREAK:{
			struct fbgc_ll_base * loop_obj =  _cast_llbase_as_lljumper(pc)->content;
			if(loop_obj->type == FOR) {
				STACK_GOTO(-4); //clean the for loop remainders
			}
			pc = _cast_llbase_as_lljumper(loop_obj)->content;
			break;
		}
		case CONT:{
			pc = _cast_llbase_as_lljumper(pc)->content;
			continue;
		}				
		case RETURN:{

			struct fbgc_object * ret = POP();
			struct fun_call_packet * fcp = (struct fun_call_packet *) cast_fbgc_object_as_cstruct(POP())->cstruct;
			
			sctr -= fcp->pop_number;
			PUSH(ret);

			current_scope = fcp->last_scope;
			fctr = fcp->frame_ctr;

			if(fcp->last_pc == NULL){
				return TOP();
			}

			pc = fcp->last_pc;
			
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

			fbgc_token main_tok = MAX((TOP()->type),SECOND()->type);
			if(main_tok > INSTANCE && MIN(TOP()->type,SECOND()->type) < LOGIC) 
				goto INTERPRETER_ERROR_LABEL;

			struct fbgc_object * res = binary_operator_fbgc_object(SECOND(),TOP(),type);
			//struct fbgc_object * res =  call_fbgc_operator(main_tok,SECOND(),TOP(),type);
			if(res == NULL){
				FBGC_LOGE("Object %s does not support operator %s\n",object_name_array[main_tok],object_name_array[type]);
				return 0;
			}
			_POP();
			SET_TOP(res);		

			break;	

		}
		case EXCLAMATION:
		case TILDE:		
		case UPLUS:
		case UMINUS:{
			
			struct fbgc_object * res = unary_operator_fbgc_object(TOP(),type);
			if(res == NULL){
				printf("operator does not supported");
				return 0;
			}
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


			struct fbgc_object * rhs = POP();
			struct fbgc_object ** lhs = NULL;

			if(is_id_flag_array_accessible(pc)){

				//If global then ask for field, if it is class then ask for current scope
				struct fbgc_object * variable_map = is_id_flag_GLOBAL(pc) ? 
									cast_fbgc_object_as_field(current_field)->variables:
									cast_fbgc_object_as_class(current_scope)->variables;

				// struct fbgc_identifier * tmp = (struct fbgc_identifier *)
				// 								get_item_fbgc_vector(arr,_cast_fbgc_object_as_llidentifier(pc)->loc);
				// lhs = &tmp->content;

				struct fbgc_object * key = get_object_in_fbgc_tuple_object(fbgc_symbols,_cast_fbgc_object_as_llidentifier(pc)->loc);
				struct fbgc_map_pair * mp = fbgc_map_object_lookup_map_pair(variable_map,key);
				
				lhs = &mp->value;
			}
			else if(is_id_flag_LOCAL(pc)){
				lhs = &(GET_AT_FP(_cast_fbgc_object_as_llidentifier(pc)->loc));
			}			
			else if(is_id_flag_MEMBER(pc)){
				struct fbgc_object * member = POP();
				
				struct fbgc_object * obj = get_set_fbgc_object_member(TOP(),content_fbgc_str_object(member) , rhs);

				//<! @todo This function is only valid for instance objects so change it !
				if(obj == NULL ){
					printf("[%s] does not support [%s] member assignment\n",objtp2str(TOP()),content_fbgc_str_object(member));
					goto INTERPRETER_ERROR_LABEL;
				}
				_POP();

				break;
			}

			//Call other assignment types
			if(type != ASSIGN)
			{
				fbgc_token op_type = RSHIFT + type - RSHIFT_ASSIGN;
				rhs = binary_operator_fbgc_object(*lhs,rhs,op_type);
				//rhs = call_fbgc_operator(main_tok,*lhs,rhs,op_type);
				if(rhs == NULL){
					printf("assignment does not supported!");
					return 0;
				}
			}

			*lhs = rhs;

			// for x = y= 5
			if(is_id_flag_PUSH_ITSELF(pc)){
				PUSH(rhs);
			}	
			
			break;
		}
		case ASSIGN_SUBSCRIPT:
		{	
			struct fbgc_object * rhs = POP();
			int index_no = _cast_llbase_as_llopcode_int(pc)->content+1;
			struct fbgc_object * iterable = TOPN(index_no);
			
			
			//Stack layout: 
			// iterable, index1,index2,index3,rhs
			//index_no tells us how many iteration that we need to do, until it is zero we will try to get the next object
			//Finally when getting the object we will change its value
			

			for(int i = index_no-1; i>1; --i){
				iterable = subscript_operator_fbgc_object(iterable,TOPN(i));
				if(iterable == NULL){
					assert(0);
				}
			}
			iterable = subscript_assign_operator_fbgc_object(iterable,TOP(),rhs);
			if(iterable == NULL){
				assert(0);
			}
			STACK_GOTO(-index_no);
			break;
		}
		case LEN:
		{	
			struct fbgc_object * obj =  abs_operator_fbgc_object(TOP());
			assert(obj != NULL);
			SET_TOP(obj);
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

			struct fbgc_cstruct_object * so = (struct fbgc_cstruct_object *) 
			new_fbgc_cstruct_object(sizeof(struct iter_function_ptr_struct), NULL);
			struct iter_function_ptr_struct * ifps = (struct iter_function_ptr_struct *) so->cstruct;
			ifps->seq_ob = POP();

			PUSH((struct fbgc_object *) so);
			
			
			if(ifps->seq_ob->type == RANGE){

				fbgc_token range_type = get_fbgc_range_object_iter_type(ifps->seq_ob);

				if(range_type == INT){
					ifps->function = get_int_element_in_fbgc_range_object;
					ifps->holder = new_fbgc_int_object(0xFB6C);
					//PUSH(new_fbgc_int_object(888));  	
				}
				else if(range_type == DOUBLE){
					ifps->function = get_double_element_in_fbgc_range_object;
					ifps->holder = new_fbgc_double_object(8.8);
					//PUSH(new_fbgc_double_object(8.8));
				}
				//=====================================================
				//check the top is it sequential obj ?
				// this depends on seq obj type, if it's a tuple we do not need it
				//if range-int then it is int, range-double: double
				// string : string etc..
						
			}
			else if(ifps->seq_ob->type == STRING){
				ifps->function = get_char_from_fbgc_str_object;
				ifps->holder = new_fbgc_str_object("*");
				//PUSH(new_fbgc_str_object("*"));
			}
			else if(ifps->seq_ob->type == TUPLE){
				ifps->function = __get_object_in_fbgc_tuple_object;
				ifps->holder = NULL;
				//holder of tuple will never be used, we are sure of it but it is not a good idea to push null object
				//PUSH(NULL);

			}
			else assert(0);

			//this is always the same!
			//PUSH(new_fbgc_int_object(0)); //iterator starter
			ifps->iterator = 0;
			break;
		}
		case FOR:
		{	
			//Assume that FOR_BEGIN checked everything and prepared
			struct iter_function_ptr_struct * ifps = (struct iter_function_ptr_struct *) 
														cast_fbgc_object_as_cstruct(TOP())->cstruct;
			//call the pre-defined function
			struct fbgc_object * res = ifps->function(ifps->seq_ob,(ifps->iterator)++,ifps->holder);


			if(res != NULL){
				PUSH(res);
			}
			else{
				//finish the for loop, everything worked normally
				//pop struct
				_POP();
				pc = _cast_llbase_as_lljumper(pc)->content;
			}

			break;
		}
		case KWFUN_CALL:
		case FUN_CALL:{


			int arg_no = _cast_llbase_as_llopcode_int(pc)->content;
			struct fbgc_object * funo = TOPN(arg_no+1);

			uint8_t kwargs_flag = false;
			int pop_number = 0;

			if(type == KWFUN_CALL){
				kwargs_flag = true;
				--arg_no;
				--pop_number;
			}

			FBGC_LOGV(INTERPRETER,"Fun call arg no :%d || call type :%s \n",arg_no,objtp2str(funo));
			
			if(funo->type == CMODULE){
				//Call the second, pop the field
				funo = TOPN(arg_no--);
				--pop_number; 
			}
			

			if(funo->type == CFUN){
				STACK_GOTO(-arg_no - kwargs_flag);
				struct fbgc_cfun_arg cfarg = {.arg =sp+sctr, .argc = (uint8_t)arg_no, .kwargs_flag = kwargs_flag };
				struct fbgc_object * res = cast_fbgc_object_as_cfun(funo)->function(&cfarg);
				
				--pop_number;

				if(res == NULL){
					//const struct fbgc_cfunction * cc = cast_fbgc_object_as_cfun(funo);
					//cprintf(100,"Error in function %s\n",cc->name);
					goto INTERPRETER_ERROR_LABEL;
				} 
				STACK_GOTO(pop_number);	
				PUSH(res);
				break;
			}



			if(funo->type == CLASS){
				FBGC_LOGV(INTERPRETER,"Calling class constructor\n");

				struct fbgc_object * new_inst = new_fbgc_instance_object(funo);
				struct fbgc_object * constructor = get_set_fbgc_object_member(funo,"init",NULL);

				if(constructor == NULL){
					FBGC_LOGV(INTERPRETER,"Class constructor is created default\n");
					if(arg_no != 0){
						//Changed for nested classes, may need revise
						//arg_no = 0;
						//_POP();
						//this is also an error
						assert(0);
					}
					//No definition of constructor 
					--pop_number;
					STACK_GOTO(pop_number); //pop cls object
					PUSH(new_inst);
					break;
				}else {
					FBGC_LOGV(INTERPRETER,"User defined class constructor\n");
					//Check is it callable ? 

					funo = constructor;
					//Now class is already pushed we will change it with new_inst
					if(cast_fbgc_object_as_fun(funo)->no_arg != (arg_no+1) ){
						printf("Argument match error! funo->arg %d, arg_no %d\n",cast_fbgc_object_as_fun(funo)->no_arg,arg_no);
						goto INTERPRETER_ERROR_LABEL;
					}

					global_interpreter_packet = ip; //TODO why do we need to assign it again ?
					SET_TOPN(arg_no+1,new_inst);
					call_fun_object(constructor);
					STACK_GOTO(pop_number);
					SET_TOP(new_inst);
					break;
				}
			}

			if(funo->type != FUN){
				printf("Object is not callable, type %s\n",object_name_array[funo->type]);
				goto INTERPRETER_ERROR_LABEL;
			}

			int8_t funo_no_arg = cast_fbgc_object_as_fun(funo)->no_arg;

			struct fbgc_ll_base * code_start = cast_fbgc_object_as_fun(funo)->code;

			if(cast_fbgc_object_as_fun(funo)->flag_variadic){

				funo_no_arg *= -1;

				int pos_arg_no = -funo_no_arg-1;

				//Is variadic function ?,
				int nvararg = arg_no + funo_no_arg + 1;

				if(cast_fbgc_object_as_fun(funo)->flag_default_args == 0){
					if(pos_arg_no > arg_no){
						printf("Argument match error! funo->arg %d, arg_no %d\n",funo_no_arg,arg_no);
						goto INTERPRETER_ERROR_LABEL;
					}
					funo_no_arg = pos_arg_no;
					arg_no = funo_no_arg;
				}
				else{
					if(arg_no <= pos_arg_no){
						//We need to skip variadic part
						//delete variadic part
						funo_no_arg = pos_arg_no;
						nvararg = 0;
					}
					else{
						//We need to skip default arg part
						funo_no_arg = pos_arg_no;
						arg_no = funo_no_arg;
					}

					if(funo_no_arg != arg_no){
						struct fbgc_object * deftuple = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(funo)->code)->content;
						print_fbgc_object(deftuple);

						int push_no = funo_no_arg - arg_no;
						if(push_no != size_fbgc_tuple_object(deftuple)){
							assert(0);
						}

						for(int i = 0; i< push_no; ++i){
							cprintf(111,"%c:pushing\n",print_fbgc_object(content_fbgc_tuple_object(deftuple)[i]));
							PUSH(content_fbgc_tuple_object(deftuple)[i]);
						}
					}

					arg_no = funo_no_arg;
				}

				struct fbgc_object * argtple = new_fbgc_tuple_object_from_tuple_content(sp+sctr-nvararg,nvararg);
				SET_TOPN(nvararg,argtple);
				STACK_GOTO( -nvararg );
				//arg_no = -funo_no_arg;
				
				//print_fbgc_object(argtple);
				
				//return;
			}
			else if(cast_fbgc_object_as_fun(funo)->flag_default_args == 0 && funo_no_arg != arg_no ){
				
				printf("Argument match error! funo->arg %d, arg_no %d\n",funo_no_arg,arg_no);
				goto INTERPRETER_ERROR_LABEL;
				
			}

			if(last_called_function == funo) ++recursion_ctr;
			else {
				last_called_function = funo;
				recursion_ctr = 0;
			}

			STACK_GOTO(cast_fbgc_object_as_fun(funo)->no_locals - arg_no);

			if(cast_fbgc_object_as_fun(funo)->flag_default_args){
				if(funo_no_arg != arg_no){
					struct fbgc_object * deftuple = _cast_llbase_as_llconstant(cast_fbgc_object_as_fun(funo)->code)->content;
					print_fbgc_object(deftuple);

					int push_no = funo_no_arg - arg_no;
					if(push_no > size_fbgc_tuple_object(deftuple)){
						assert(0);
					}

					for(int i = 0; i< push_no; ++i){
						cprintf(111,"%c:pushing\n",print_fbgc_object(content_fbgc_tuple_object(deftuple)[i]));
						SET_TOPN(i+1,content_fbgc_tuple_object(deftuple)[i]);
					}
				}
				code_start = cast_fbgc_object_as_fun(funo)->code->next;
			}

			PUSH( prepare_fun_call_packet(pc,current_scope,fctr,cast_fbgc_object_as_fun(funo)->no_locals+1));

			//PUSH((struct fbgc_object *)pc->next);
			//hold old frame pointer location
			//PUSH(new_fbgc_int_object(fctr));
			//hold old position of sp with fp, assume that args already pushed into stack
			fctr = sctr-cast_fbgc_object_as_fun(funo)->no_locals-1;
			//execute function


			pc = code_start;
			//We should not break here because pc will start immeadiately from where is it assigned above
			//if we use break, fetch_next will change the order of execution
			continue;
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
		case BUILD_MAP:{
			int map_size = _cast_llbase_as_llopcode_int(pc)->content;
			struct fbgc_object * map = new_fbgc_map_object(map_size,50);
			//Instead of pop we can give starting address and insert everything automatically
			while(map_size--){
				struct fbgc_object * value = POP();
				struct fbgc_object * key = POP();
				fbgc_map_object_insert(map,key,value);
			}

			PUSH(map);
			break;
		}
		case POP_TOP:
		{
			_POP();
			break;
		}
		case LOAD_SUBSCRIPT:
		{
			int index_no = _cast_llbase_as_llopcode_int(pc)->content+1;
			struct fbgc_object * iterable = TOPN(index_no);


			for(int i = index_no-1; i>0; --i){
				iterable = subscript_operator_fbgc_object(iterable,TOPN(i));
				if(iterable == NULL){
					FBGC_LOGE("Iterable is NULL");
					assert(0);
				}
			}
			STACK_GOTO(-index_no);
			PUSH(iterable);
			break;
		}		
		case CLASS_CALL:{

			int arg_no = _cast_llbase_as_llopcode_int(pc)->content;
			FBGC_LOGV(INTERPRETER,"CLASS_CALL : Arg no :%d\n",arg_no);

			struct fbgc_object * cls = TOPN(arg_no+1);
			
			while(arg_no--){
				inherit_from_another_class(cls,POP());
			}

			//Just pop the function object when returning 
			PUSH( prepare_fun_call_packet(pc,current_scope,fctr,1) );

			current_scope = cls;
			fctr = sctr-1;
			
			pc = cast_fbgc_object_as_class(cls)->code;
			continue;
		}
		case BUILD_FUN_DEFAULT_ARGS:{
			
			struct fbgc_object * tp = POP();
			struct fbgc_object * funo = TOP();		

			struct fbgc_ll_base * n = _new_fbgc_ll_constant(tp);
			n->next = cast_fbgc_object_as_fun(funo)->code;
			cast_fbgc_object_as_fun(funo)->code = n;
			

			FBGC_LOGV(INTERPRETER,"%c\n",_print_fbgc_ll_code(cast_fbgc_object_as_fun(funo)->code,NULL));
			//FBGC_LOGV(INTERPRETER,"Default arg no :%d\n",arg_no);

			//struct fbgc_object * funo = TOPN(arg_no+1);
			//

			//struct fbgc_object * tp = new_fbgc_tuple_object(arg_no);


			break;
		}
		
		default:
		{
			cprintf(101,"Undefined token in interpreter [%s]\n",object_name_array[type]);
			return 0;						
		}
	}


	//cast_fbgc_object_as_tuple(stack)->size = sctr; //sp - content_fbgc_tuple_object(stack);
		
	FBGC_LOGV(INTERPRETER,"{");
	for(size_t i = 0; i<sctr; i++){
		FBGC_LOGV(INTERPRETER,"%c",print_fbgc_object(sp[i]));
		FBGC_LOGV(INTERPRETER,", ");
	}
	FBGC_LOGV(INTERPRETER,"}\n");
	FBGC_LOGV(INTERPRETER,"~~~~~~Field Globals~~~~~\n");
	FBGC_LOGV(INTERPRETER,"%d",print_fbgc_map_object(cast_fbgc_object_as_field(current_field)->variables));

	// struct fbgc_vector * globals;
	// if(current_scope != NULL && current_scope->type == CLASS) globals = cast_fbgc_object_as_class(current_scope)->locals;
	// else globals = cast_fbgc_object_as_field(current_field)->locals;

	// for(int i = 0; i<size_fbgc_vector(globals); i++){
	// 	struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(globals,i);
	// 	FBGC_LOGV(INTERPRETER,"%c:",print_fbgc_object(temp_id->name));
	// 	FBGC_LOGV(INTERPRETER,"%c",print_fbgc_object(temp_id->content));
	// 	FBGC_LOGV(INTERPRETER,"}");
	// }
	FBGC_LOGV(INTERPRETER,"\n==============================================\n\n\n");
	

	FETCH_NEXT();

	}

	return sctr > 0 ? TOP():NULL;

	INTERPRETER_ERROR_LABEL:
		cprintf(100,"Execution stopped!\n");
		return NULL;


#undef sctr
#undef fctr
#undef pc
#undef sp


}


struct fbgc_object * call_fun_object(struct fbgc_object * fun){
	FBGC_LOGV(INTERPRETER,"Calling function no_arg:%d, no_locals:%d\n",cast_fbgc_object_as_fun(fun)->no_arg,cast_fbgc_object_as_fun(fun)->no_arg);
	
	struct interpreter_packet * old_global_ip = global_interpreter_packet;

	struct interpreter_packet tip = { 	
		global_interpreter_packet->sp, //sp
		global_interpreter_packet->current_scope, //current_scope
		cast_fbgc_object_as_fun(fun)->code, //pc
		global_interpreter_packet->sctr + cast_fbgc_object_as_fun(fun)->no_locals - cast_fbgc_object_as_fun(fun)->no_arg, //sctr
		global_interpreter_packet->sctr - cast_fbgc_object_as_fun(fun)->no_arg, // fctr
		
	};

	//fprintf_fbgc_fun_object(fun);

	FBGC_LOGV(INTERPRETER,"Calculated fctr:%d\n",tip.fctr);

	tip.sp[tip.sctr++] = prepare_fun_call_packet(NULL, tip.current_scope, -1, cast_fbgc_object_as_fun(fun)->no_locals);
	
	struct fbgc_object * result = run_code(&tip);

	global_interpreter_packet = old_global_ip;
	global_interpreter_packet->sctr = tip.sctr;

	FBGC_LOGV(INTERPRETER,"%c:returning\n",print_fbgc_object(result));
	FBGC_LOGV(INTERPRETER,"{");
	for(size_t i = 0; i<tip.sctr; i++){
		FBGC_LOGV(INTERPRETER,"%c",print_fbgc_object(global_interpreter_packet->sp[i]));
		FBGC_LOGV(INTERPRETER,", ");
	}
	FBGC_LOGV(INTERPRETER,"}\n");

	FBGC_LOGV(INTERPRETER,"finished\n");	

	return result;
}



//@TODO
//Error when calling nested class constructor as self.class_name()
//It gives some error see following code
/*
car = class()
	engine = class()
		rpm = 0
		power = 0

		init = fun(r,p,self)
			self.rpm = r
			self.power = p
			return self
		end
	end

	eng = 0
	gas = 10

	init = fun(r,p,g,self)
		self.eng = self.engine(r,p)
		self.gas = g
		print(self.gas)
		return self
	end
end



audi = car(3,4,5)
*/