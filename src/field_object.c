#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_stl.h"

struct fbgc_object * get_fbgc_field_object_cmodules(struct fbgc_object * field_obj){
	return fbgc_map_object_lookup_str(cast_fbgc_object_as_field(field_obj)->variables,"_cmodules");
}

static void load_module_in_field_object(struct fbgc_object * field_obj,const struct fbgc_object_property_holder * p){

	struct fbgc_object * cm = new_fbgc_cmodule_object(p);
	//C-modules are called _lib
	struct fbgc_object * _lib_tuple = get_fbgc_field_object_cmodules(field_obj);
	
	push_back_fbgc_tuple_object(_lib_tuple,cm);

	//call function initializer
	int8_t w = _find_property(p->bits,_BIT_INITIALIZER);
	if(w != -1){
		//call initializer
		p->properties[w].initializer();
	}
}

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = cast_fbgc_object_as_field(new_fbgc_class_object());
	field->base.type = FIELD;
	field->code = _new_fbgc_ll();

	//library map size is constant(or will not contain so much data) so making its rlf 100 is not a bad idea
	fbgc_map_object_insert_str(field->variables,"_cmodules",new_fbgc_tuple_object(2));
	
	load_module_in_field_object((struct fbgc_object *)field,&_fbgc_stl_property_holder);
	load_module_in_field_object((struct fbgc_object *)field,&_fbgc_io_property_holder);
	
	return (struct fbgc_object *) field;
};


struct fbgc_ll_base * add_variable_in_field_object(struct fbgc_object * field_obj,const char * var_name, struct fbgc_object * rhs){
	struct fbgc_ll_base * iter = new_fbgc_symbol_from_substr(var_name,var_name + strlen(var_name));
	fbgc_map_object_insert_str(cast_fbgc_object_as_field(field_obj)->variables,var_name,rhs);
	return iter;
}

uint8_t print_field_object_locals(struct fbgc_object * field_obj){
	return print_fbgc_map_object(cast_fbgc_object_as_field(field_obj)->variables);
}