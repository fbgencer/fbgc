#include "fbgc.h"

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = (struct fbgc_field_object *) fbgc_malloc(sizeof(struct fbgc_field_object ));
	field->base.next = NULL;
	field->base.type = FIELD; 
	field->head = new_fbgc_ll_object(LINKED_LIST);
	field->modules = new_fbgc_ll_object(LINKED_LIST);
	field->locals = new_fbgc_array_object(1,sizeof(struct fbgc_identifier));
	return (struct fbgc_object *) field;
};

void load_module_in_field_object(struct fbgc_object * field_obj, const struct fbgc_cmodule * module){

	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *) fbgc_malloc(sizeof(struct fbgc_cmodule_object ));
	cm->module = module;
	cm->base.type = CMODULE;//CHANGE THIS
	push_front_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->modules,(struct fbgc_object *)cm);

}


void print_field_object_locals(struct fbgc_object * field_obj){
	struct fbgc_object * ao = cast_fbgc_object_as_field(field_obj)->locals;
	for(int i = 0; i<size_fbgc_array_object(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		cprintf(010,"{"); print_fbgc_object(temp_id->name);
		cprintf(010,":"); print_fbgc_object(temp_id->content);
		cprintf(010,"}");
	}
}

void free_fbgc_field_object(struct fbgc_object * field_obj){
/*	free_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->head);
	free_fbgc_ll_object(cast_fbgc_object_as_field(field_obj)->modules);
	free_fbgc_symbol_table(cast_fbgc_object_as_field(field_obj)->global_table);
	free(field_obj);*/
}