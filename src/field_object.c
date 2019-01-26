#include "fbgc.h"

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = (struct fbgc_field_object *) malloc(sizeof(struct fbgc_field_object ));
	field->base.next = NULL;
	field->base.type = UNKNOWN;
	field->head = new_fbgc_ll_object(LINKED_LIST);
	field->global_table = new_fbgc_symbol_table();

	return (struct fbgc_object *) field;
};

void free_fbgc_field_object(struct fbgc_object * field_obj){
	struct fbgc_field_object * field = (struct fbgc_field_object *) field_obj;
	free_fbgc_ll_object(field->head);
	free_fbgc_symbol_table(field->global_table);
	free(field);
}
