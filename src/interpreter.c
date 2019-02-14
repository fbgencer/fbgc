#include "fbgc.h"

uint8_t interpreter(struct fbgc_object ** field_obj){

	cprintf(110,"---------------interpreter-------------------\n");

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head;
	struct fbgc_object * stack = new_fbgc_ll_object(STACK);
	

	for(int i = 0; i<3000 && (iter != head->tail); i++){
		cprintf(010,"----------------------[%d] = {%s}-----------------------\n",i,object_name_array[iter->type]);
		if(is_fbgc_ATOM(get_fbgc_object_type(iter))){

			iter_prev->next = iter->next;
			stack = push_front_fbgc_ll_object(stack,iter);
		}
		else if( get_fbgc_object_type(iter) == REFERENCE){
			iter_prev->next = iter->next;
			stack = push_front_fbgc_ll_object(stack,iter);
		}

		else if(is_fbgc_BINARY_OPERATOR(get_fbgc_object_type(iter))){
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
		else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(iter))){
			struct fbgc_object * o2 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * o1 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);

			call_fbgc_assignment_op(get_fbgc_object_type(iter),cast_fbgc_object_as_field(*field_obj)->global_table,o1,o2);
			
			iter_prev->next = iter->next;

			
			if(o2->type == REFERENCE) free_fbgc_object(o2);
			else if(o1->type == TUPLE && o2->type == TUPLE){
				cast_fbgc_object_as_tuple(o2)->size = 0;
				free_fbgc_object(o2);
			}
			free_fbgc_object(o1);
			free_fbgc_object(iter);			
		}
		else if(get_fbgc_object_type(iter) == RAW_TUPLE){
			struct fbgc_object * to = new_fbgc_tuple_object(cast_fbgc_object_as_int(top_fbgc_ll_object(stack))->content);
			stack = delete_front_fbgc_ll_object(stack);
			for(int q = cast_fbgc_object_as_tuple(to)->size - 1; q>=0; q--){
				set_object_in_fbgc_tuple_object(to,top_fbgc_ll_object(stack),q);
				stack = pop_front_fbgc_ll_object(stack);
			}
			iter_prev->next = iter->next;
			free_fbgc_object(iter);
			stack = push_front_fbgc_ll_object(stack,to);
		}
		else if(get_fbgc_object_type(iter) == MONUPLE){
			
			iter_prev->next = iter->next;
			free_fbgc_object(iter);
		}
		/*else if(get_fbgc_object_type(iter) == RAW_MATRIX){

			struct fbgc_object * mato = new_fbgc_matrix_object(cast_fbgc_object_as_int(top_fbgc_ll_object(stack))->content);
			append_row_to_fbgc_matrix_object(mato,top_fbgc_ll_object(stack));
			stack = push_front_fbgc_ll_object(stack,mato);
		}*/

		else if(get_fbgc_object_type(iter) == CFUN){
			struct fbgc_object * o2 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * o1 = cfun_object_call(iter,o2);

			stack = push_front_fbgc_ll_object(stack,o1);
			iter_prev->next = iter->next;
			free_fbgc_object(o2);
			free_fbgc_object(iter);

		}
		else {
			fbgc_assert(get_fbgc_object_type(iter) == TUPLE,"tuple fuck !\n");
			cprintf(101,"Undefined token in interpreter\n");
			break;
		}

		iter = iter_prev->next;
		
		print_fbgc_ll_object(cast_fbgc_object_as_field(*field_obj)->head,"Main");
		print_fbgc_ll_object(stack,"Stack");
		print_fbgc_symbol_table(cast_fbgc_object_as_field(*field_obj)->global_table);
		cprintf(111,"-------------------------------------------\n");
	}

	//make the linked list connection proper
	head->tail->next = iter_prev;

	free_fbgc_ll_object(stack);
	cprintf(111,"--------------[INTERPRETER END]-------------\n");

	return 1;
}