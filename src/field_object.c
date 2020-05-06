#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_stl.h"

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = (struct fbgc_field_object *) fbgc_malloc_object(sizeof(struct fbgc_field_object ));
	//field->base.next = NULL;
	field->base.type = FIELD; 
	field->head = _new_fbgc_ll();
	field->modules = _new_fbgc_ll();
	field->locals = new_fbgc_array_object(1,sizeof(struct fbgc_identifier));
	
	load_module_in_field_object((struct fbgc_object *)field,&fbgc_io_module);
	load_module_in_field_object((struct fbgc_object *)field,&fbgc_stl_module);
	return (struct fbgc_object *) field;
};

void load_module_in_field_object(struct fbgc_object * field_obj, const struct fbgc_cmodule * module){

	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *) fbgc_malloc_object(sizeof(struct fbgc_cmodule_object ));
	cm->module = module;
	cm->base.type = CMODULE;//CHANGE THIS

	if(module->initializer.function != NULL)
		module->initializer.function(&field_obj, 1);

	_push_front_fbgc_ll(cast_fbgc_object_as_field(field_obj)->modules,_new_fbgc_ll_constant((struct fbgc_object *)cm));	
}

struct fbgc_ll_base * add_variable_in_field_object(struct fbgc_object * field_obj,const char * var_name, struct fbgc_object * rhs){
		
		//if(rhs == NULL) return NULL;

	struct fbgc_ll_base * iter = new_fbgc_symbol_from_substr(var_name,var_name + strlen(var_name));
	//this location is from symbols, we need to find location in fields
	struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,_cast_fbgc_object_as_llidentifier(iter)->loc);


	struct fbgc_object * local_array = cast_fbgc_object_as_field(field_obj)->locals;
	struct fbgc_identifier * temp_id; 
	int where = -1;

	//first search this in created variables
	for(int i = 0; i<size_fbgc_array_object(local_array); i++){
		temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
		if(temp_id->name == cstr_obj) {
			where = i;
			break;
		} 
	}

	if(where == -1){
		struct fbgc_identifier id;		
		id.name = cstr_obj; id.content = rhs;
		local_array = push_back_fbgc_array_object(local_array,&id);
		where = size_fbgc_array_object(local_array)-1;
		cast_fbgc_object_as_field(field_obj)->locals = local_array;
	}else{

		_cast_fbgc_object_as_llidentifier(temp_id)->loc = where;
		temp_id->content = rhs;
	}
	set_id_flag_GLOBAL(iter);

	return iter;
}


uint8_t print_field_object_locals(struct fbgc_object * field_obj){
	struct fbgc_object * ao = cast_fbgc_object_as_field(field_obj)->locals;
	for(int i = 0; i<size_fbgc_array_object(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		cprintf(010,"{"); print_fbgc_object(temp_id->name);
		cprintf(010,":"); print_fbgc_object(temp_id->content);
		cprintf(010,"}");
	}
	return 1;
}

struct fbgc_object * get_set_fbgc_field_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){
	if(nm!= NULL) return NULL;

	struct fbgc_object * ao = cast_fbgc_object_as_field(o)->locals;
	for(unsigned int i = 0;  i<size_fbgc_array_object(ao); ++i){	
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		//cprintf(111,"temp_idname = %s, str:%s => %d\n",content_fbgc_cstr_object(temp_id->name),str,my_strcmp(content_fbgc_cstr_object(temp_id->name),str));
		if(!my_strcmp(content_fbgc_cstr_object(temp_id->name),str)){
			return temp_id->content;
		}
	}

	return NULL;
}

const struct fbgc_object_property_holder fbgc_field_object_property_holder = {
	.bits = 
	_BIT_GET_SET_MEMBER
	,
	.properties ={
		{.get_set_member = &get_set_fbgc_field_object_member},			
	}
};