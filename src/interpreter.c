#include "fbgc.h"

uint8_t interpreter(struct fbgc_object ** field_obj){

	#ifdef INTERPRETER_DEBUG
	cprintf(111,"==========[INTERPRETER]==========\n");
	#endif


	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );

	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head;
	struct fbgc_object * stack = new_fbgc_ll_object(STACK);
	


	for(int i = 0; i<30000 && (iter != head->tail); i++){

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

			//take the o1's object maybe this object is referenced just by o1, tmp = o1->object
			//then assign o1 
			//set tmp's mark bit to zero
			//then run mark algorithm to say every live object to set their objects mark bit 1
			//free gc
			//abandon_ownership(temp) // now this will set temp object mark bit to zero
			//now we can easily free the all objects because if the mark bit is 1, its not gonna be freed

			//struct fbgc_object * temp = get_var_from_fbgc_ref_object(o1);
			//abandon_ownership(o1);

			//call_fbgc_assignment_op(get_fbgc_object_type(iter),cast_fbgc_object_as_field(*field_obj)->global_table,o1,o2);
			call_fbgc_assignment_op(type,o1,o2);
			
			//claim_ownership_in_symbol_table(cast_fbgc_object_as_field(*field_obj)->global_table);
			
			//if(get_fbgc_object_type(o1) == TUPLE && get_fbgc_object_type(o2) == TUPLE){
			//	cast_fbgc_object_as_tuple(o2)->size = 0;
			//	free_fbgc_object(o2);
			//}


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
		
		else if(type == RAW_TUPLE){
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