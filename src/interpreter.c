#include "fbgc.h"

uint8_t interpreter(struct fbgc_object ** field_obj){

	cprintf(110,"---------------interpreter-------------------\n");

	struct fbgc_ll_object * head = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(*field_obj)->head );
	struct fbgc_object * iter = head->base.next;
	struct fbgc_object * iter_prev = (struct fbgc_object *)head;
	struct fbgc_object * stack = new_fbgc_ll_object(STACK);
	

	for(int i = 0; i<100 && (iter != head->tail); i++){
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

			stack = push_front_fbgc_ll_object(
				stack,
				call_fbgc_binary_op(
					get_fbgc_object_type(iter),
					get_var_from_fbgc_ref_object(o1),
					get_var_from_fbgc_ref_object(o2))
				);
			iter_prev->next = iter->next;

			free_fbgc_object(o1);
			free_fbgc_object(o2);
			free_fbgc_object(iter);

		}
		else if(is_fbgc_ASSIGNMENT_OPERATOR(get_fbgc_object_type(iter))){
			struct fbgc_object * o2 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);
			struct fbgc_object * o1 = top_fbgc_ll_object(stack);
			stack = pop_front_fbgc_ll_object(stack);

			call_fbgc_assignment_op(get_fbgc_object_type(iter),o1,o2);
			
			iter_prev->next = iter->next;
			// x = 5;

			free_fbgc_object(o1);
			if(o2->type == REFERENCE) free_fbgc_object(o2);
			free_fbgc_object(iter);			
		}
		else {
			cprintf(101,"Undefined token in interpreter\n");
		}

		iter = iter_prev->next;
		
		print_fbgc_ll_object(head,"Main");
		print_fbgc_ll_object(stack,"Stack");
		print_fbgc_symbol_table(cast_fbgc_object_as_field(*field_obj)->global_table);
		cprintf(111,"-------------------------------------------\n");
	}


	free_fbgc_ll_object(stack);
	return 1;
}