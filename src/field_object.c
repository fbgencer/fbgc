#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_stl.h"


static void load_module_in_field_object(struct fbgc_object * field_obj,const struct fbgc_object_property_holder * p){

	struct fbgc_object * cm = new_fbgc_cmodule_object(p);
	push_back_fbgc_tuple_object(cast_fbgc_object_as_field(field_obj)->modules,cm);

	//call function initializer
	int8_t w = _find_property(p->bits,_BIT_INITIALIZER);
	if(w != -1){
		//call initializer
		p->properties[w].initializer();
	}
}

struct fbgc_object * new_fbgc_field_object(void){

	struct fbgc_field_object * field = (struct fbgc_field_object *) fbgc_malloc_object(sizeof(struct fbgc_field_object ));
	//field->base.next = NULL;
	field->base.type = FIELD; 
	field->head = _new_fbgc_ll();
	field->modules = new_fbgc_tuple_object(2); 
	field->locals = new_fbgc_vector(1,sizeof(struct fbgc_identifier));
	field->variables = new_fbgc_map_object(2,80);
	
	load_module_in_field_object((struct fbgc_object *)field,&_fbgc_stl_property_holder);
	load_module_in_field_object((struct fbgc_object *)field,&_fbgc_io_property_holder);
	
	return (struct fbgc_object *) field;
};


struct fbgc_ll_base * add_variable_in_field_object(struct fbgc_object * field_obj,const char * var_name, struct fbgc_object * rhs){
		
		//if(rhs == NULL) return NULL;

	struct fbgc_ll_base * iter = new_fbgc_symbol_from_substr(var_name,var_name + strlen(var_name));
	//this location is from symbols, we need to find location in fields
	struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,_cast_fbgc_object_as_llidentifier(iter)->loc);


	struct fbgc_vector * local_array = cast_fbgc_object_as_field(field_obj)->locals;
	struct fbgc_identifier * temp_id; 
	int where = -1;

	//first search this in created variables
	for(int i = 0; i<size_fbgc_vector(local_array); i++){
		temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(local_array,i);
		if(temp_id->name == cstr_obj) {
			where = i;
			break;
		} 
	}

	if(where == -1){
		struct fbgc_identifier id;		
		id.name = cstr_obj; id.content = rhs;
		push_back_fbgc_vector(local_array,&id);
		where = size_fbgc_vector(local_array)-1;
	}else{

		_cast_fbgc_object_as_llidentifier(temp_id)->loc = where;
		temp_id->content = rhs;
	}
	set_id_flag_GLOBAL(iter);

	return iter;
}


uint8_t print_field_object_locals(struct fbgc_object * field_obj){
	struct fbgc_vector * ao = cast_fbgc_object_as_field(field_obj)->locals;
	for(int i = 0; i<size_fbgc_vector(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(ao,i);
		cprintf(010,"{"); print_fbgc_object(temp_id->name);
		cprintf(010,":"); print_fbgc_object(temp_id->content);
		cprintf(010,"}");
	}
	return 1;
}

struct fbgc_object * get_set_fbgc_field_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){
	if(nm!= NULL) return NULL;

	struct fbgc_vector * ao = cast_fbgc_object_as_field(o)->locals;
	for(unsigned int i = 0;  i<size_fbgc_vector(ao); ++i){	
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(ao,i);
		//cprintf(111,"temp_idname = %s, str:%s => %d\n",content_fbgc_str_object(temp_id->name),str,my_strcmp(content_fbgc_str_object(temp_id->name),str));
		if(!my_strcmp(content_fbgc_str_object(temp_id->name),str)){
			return temp_id->content;
		}
	}

	return NULL;
}

static inline size_t size_of_fbgc_field_object(struct fbgc_object * self){
    return sizeof(struct fbgc_field_object);
}



const struct fbgc_object_property_holder fbgc_field_object_property_holder = {
	.bits = 
	_BIT_GET_SET_MEMBER |
	_BIT_SIZE_OF
	,
	.properties ={
		{.get_set_member = &get_set_fbgc_field_object_member},
		{.size_of = &size_of_fbgc_field_object},
	}
};