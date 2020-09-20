#include "fbgc.h"

//FIELD OBJECT

struct fbgc_object * get_fbgc_field_cmodules(struct fbgc_object * o){
	return fbgc_map_object_lookup_str(cast_fbgc_object_as_class(o)->variables,"_cmodules");
}

static void load_module_in_field(struct fbgc_object * fobj,const struct fbgc_object_property_holder * p){

	struct fbgc_object * cm = new_fbgc_cmodule_object(p);
	//C-modules are called _lib
	struct fbgc_object * _lib_tuple = get_fbgc_field_cmodules(fobj);
	
	push_back_fbgc_tuple_object(_lib_tuple,cm);

	//call function initializer
	int8_t w = _find_property(p->bits,_BIT_INITIALIZER);
	if(w != -1){
		//call initializer
		p->properties[w].initializer();
	}
}



struct fbgc_object * new_fbgc_field(uint8_t no_module, ...){

	struct fbgc_class_object * field = cast_fbgc_object_as_class(new_fbgc_class_object());
	field->code = _new_fbgc_ll();

	//library map size is constant(or will not contain so much data) so making its rlf 100 is not a bad idea
	fbgc_map_object_insert_str(field->variables,"_cmodules",new_fbgc_tuple_object(2));
	

	va_list modules;
  	va_start( modules,no_module);
  	while(no_module--){
  		load_module_in_field((struct fbgc_object *)field,va_arg( modules,  struct fbgc_object_property_holder * ));
  	}
  	va_end( modules );

	return (struct fbgc_object *) field;
};


struct fbgc_ll_base * add_variable_in_field(struct fbgc_object * fobj,const char * var_name, struct fbgc_object * rhs){
	struct fbgc_ll_base * iter = new_fbgc_symbol_from_substr(var_name,var_name + strlen(var_name));
	fbgc_map_object_insert_str(cast_fbgc_object_as_class(fobj)->variables,var_name,rhs);
	return iter;
}

uint8_t print_field_locals(struct fbgc_object * fobj){
	return print_fbgc_map_object(cast_fbgc_object_as_class(fobj)->variables);
}

