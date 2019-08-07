#include "fbgc.h"

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

	print_fbgc_symbol_table(cast_fbgc_object_as_field(*field_obj)->symbol_table);

	//claim_ownership_in_symbol_table(cast_fbgc_object_as_field(*field_obj)->global_table);
	//make the linked list connection proper
	head->tail->next = iter_prev;

	free_fbgc_ll_object(stack);
	#ifdef INTERPRETER_DEBUG
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}


uint8_t interpreter(struct fbgc_object ** field_obj){

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"==========[INTERPRETER]==========\n");
	#endif


	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * pc = head->base.next; //program counter

	#define PROGRAM_STACK_SIZE 255 
	struct fbgc_object * stack = new_fbgc_tuple_object(PROGRAM_STACK_SIZE);
	struct fbgc_object ** sp = tuple_object_content(stack);


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


	for(int i = 0; (pc != head->tail); i++){

		fbgc_token type = get_fbgc_object_type(pc);

		#ifdef INTERPRETER_DEBUG
		cprintf(010,"################ [%d] = {%s} ########################\n",i,object_type_as_str(pc));
		#endif

		// print_fbgc_memory_block();

		if(is_fbgc_ATOM(type) || type == REFERENCE){
			_PUSH(pc);
		}
		else if(is_fbgc_BINARY_OPERATOR(type)){

			struct fbgc_object * res = call_fbgc_binary_op(
					type,
					get_var_from_fbgc_ref_object(_POP()),
					get_var_from_fbgc_ref_object(_POP()));

			if(res != NULL) {
				_PUSH(res);				
			}
			else {
				cprintf(100,"NULL operator return!\n");				
				break;
			}
		}
		else if(is_fbgc_ASSIGNMENT_OPERATOR(type)){
			//TOP1 = TOP2
			call_fbgc_assignment_op(type,_POP(),_POP());
		}
		else if(type == IF_BEGIN || type == ELIF_BEGIN || type == WHILE_BEGIN){
			struct fbgc_object * cond = _POP();
			//struct fbgc_object * obj = pc;
			if( get_fbgc_object_type(is_fbgc_object_true(cond)) == FALSE  ){
				pc = cast_fbgc_object_as_ref(pc)->content;
			}
		}
		else if(type == JUMP){
			pc = cast_fbgc_object_as_ref(pc)->content;
		}	
		else if(type == BREAK){
			struct fbgc_object * loop_obj =  cast_fbgc_object_as_ref(pc)->content;
			pc = cast_fbgc_object_as_ref(loop_obj)->content;
		}				
		else {
			cprintf(101,"Undefined token in interpreter\n");
			break;			
		}

		//#############TODO#############################
		//This part is required for the stack size increment, stack is a tuple object and it has a defined size range
		//In order to increase operation speed we don't wanna increase the stack size after each push but it has to be
		//checked periodically to handle big objects like constant value tuples/lists etc.
		//Now just left for debug but later this problem has to be solved.
		#ifdef INTERPRETER_DEBUG
			cast_fbgc_object_as_tuple(stack)->size = sp - tuple_object_content(stack);
		#endif

		#ifdef INTERPRETER_DEBUG
		cprintf(111,"==============Stack==========================\n");
		print_fbgc_object(stack);
		cprintf(111,"\n===========================================\n");	
		#endif

		pc = pc->next;

	}


	print_fbgc_symbol_table(cast_fbgc_object_as_field(*field_obj)->symbol_table);

	


	#ifdef INTERPRETER_DEBUG
	cprintf(111,"^^^^^^^^^^^^^^^^^^^^\n");
	#endif
	return 1;
}
