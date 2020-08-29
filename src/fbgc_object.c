#include "fbgc.h"


struct fbgc_object * new_fbgc_object(const fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) fbgc_malloc_object(sizeof(struct fbgc_object));
    o->type = token;
    return (struct fbgc_object*) o;
}
const char * objtp2str(struct fbgc_object * obj){
	return object_name_array[obj->type];
}



const struct fbgc_object_property_holder * get_fbgc_object_property_holder(struct fbgc_object * o){
	switch(o->type){		
		case INT: return &fbgc_int_object_property_holder;
		case DOUBLE: return &fbgc_double_object_property_holder;
		case COMPLEX: return &fbgc_complex_object_property_holder;
		case STRING: return &fbgc_str_object_property_holder;
		case MATRIX: return &fbgc_matrix_object_property_holder;
		case MAP: return &fbgc_map_object_property_holder;
		case TUPLE: return &fbgc_tuple_object_property_holder;
		case LOGIC: return &fbgc_logic_object_property_holder;
		case INSTANCE : return &fbgc_instance_object_property_holder;
		case RANGE: return &fbgc_range_object_property_holder;
		case CMODULE: return cast_fbgc_object_as_cmodule(o)->properties;
		case CSTRUCT: return cast_fbgc_object_as_cstruct(o)->properties;
		case FUN : return &fbgc_fun_object_property_holder;
		case CLASS : return &fbgc_class_object_property_holder;
		case FIELD : return &fbgc_field_object_property_holder;
		

	}
	FBGC_LOGE("Type %s does not have property holder\n",objtp2str(o));
	assert(0);
}


uint32_t swar(uint32_t i) {
  i = i - ((i >> 1) & 0x55555555);
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
  return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}


int8_t _find_property(uint32_t bit, uint32_t bit_conf){
	if( (bit & bit_conf) == 0) return -1;
	
	bit &= (bit_conf-1);
	return __builtin_popcount(bit);
	//return swar(bit);
}

uint8_t myprint_fbgc_object(struct fbgc_object * self){

	const struct fbgc_object_property_holder * p = NULL;
	if(self != NULL){
		p = get_fbgc_object_property_holder(self);
	}
	else{
		return cprintf(111,"[NULL]");
	}

	//return p->properties[2].print(self);

	int8_t w = _find_property(p->bits,_BIT_PRINT);
	if(w != -1){
		return p->properties[w].print(self);
	}
	else return 1;
}

uint8_t print_fbgc_object(struct fbgc_object * self){
	//We only use this function for debug

	if(self != NULL){ 
		switch(self->type){
			case LOGIC:
			{
				cprintf(110, "%s", (cast_fbgc_object_as_logic(self)->content) ? "true" :"false");
				break;				
			}
			case INT:
				print_fbgc_int_object(self);
			break;
			case DOUBLE:
				print_fbgc_double_object(self);
			break;
			case COMPLEX:
				print_fbgc_complex_object(self);
			break;
			case STRING:
				print_fbgc_str_object(self);
			break;
			case MAP:
				print_fbgc_map_object(self);
			break;
			case TUPLE:
				print_fbgc_tuple_object(self);
			break;
			case MATRIX:
				print_fbgc_matrix_object(self);
			break;
			case FUN:
				print_fbgc_fun_object(self);
			break;
			case RANGE:
				print_fbgc_range_object(self);
			break;
			default:
				cprintf(100,"[%s]",object_type_as_str(self));  
			break;

		}
	}
	else cprintf(111,"[NULL]");

	return 1;
}

char convert_fbgc_object_to_logic(struct fbgc_object * obj){
	
	switch(obj->type){
		case LOGIC:
			return cast_fbgc_object_as_logic(obj)->content;
		case INT:
			return (char)(cast_fbgc_object_as_int(obj)->content);
		case DOUBLE:
			return (char)(cast_fbgc_object_as_double(obj)->content);
		default :
			printf_fbgc_object(obj);
			cprintf(111,"Error at logic conversion! type %s\n",object_name_array[obj->type]);
			assert(0);
	}
	
	return 0;
}

int convert_fbgc_object_to_int(struct fbgc_object * obj){

	switch(obj->type){
		case LOGIC:
			return (int)(cast_fbgc_object_as_logic(obj)->content);
		case INT:
			return cast_fbgc_object_as_int(obj)->content;
		case DOUBLE: 
			return (int)(cast_fbgc_object_as_double(obj)->content);
		default :
			cprintf(111,"Error at int conversion! type %s\n",object_name_array[obj->type]);
			assert(0);					
	}
	
	
	return 0;
}

double convert_fbgc_object_to_double(struct fbgc_object * obj){

	switch(obj->type){
		case LOGIC:
			return (double)(cast_fbgc_object_as_logic(obj)->content);
		case INT:
			return (double)(cast_fbgc_object_as_int(obj)->content);
		case DOUBLE:
			return cast_fbgc_object_as_double(obj)->content;
		default :

			cprintf(111,"Error at double conversion! type %s\n",object_name_array[obj->type]);
			assert(0);
	}
	
	return 0;
}



struct raw_complex convert_fbgc_object_to_complex(struct fbgc_object * obj){
	if(obj->type == COMPLEX) return cast_fbgc_object_as_complex(obj)->z;
	
	struct raw_complex z = {0,0};

	switch(obj->type){
		case INT:
			z.real = (double)(cast_fbgc_object_as_int(obj)->content);
			break;
		case DOUBLE:
			z.real = cast_fbgc_object_as_double(obj)->content;
			break;
		default :
			cprintf(111,"Error at double conversion! type %s\n",object_name_array[obj->type]);
			assert(0);
	}
	
	return z;
}


void printf_fbgc_object(struct fbgc_object * self){

	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(self);

	int8_t w = _find_property(p->bits,_BIT_PRINT);
	if(w != -1){
		p->properties[w].print(self);
		return;
	}
	else{
		//put an error
		assert(0);
	}
}

struct fbgc_object * abs_operator_fbgc_object(struct fbgc_object * self){

	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(self);

	int8_t w = _find_property(p->bits,_BIT_ABS_OPERATOR);
	if(w != -1){
		return p->properties[w].abs_operator(self);
	}
	else{
		FBGC_LOGE("%s does not satisfy || operator\n",objtp2str(self));
		return NULL;
	}
} 


struct fbgc_object * get_set_fbgc_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){
	
	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(o);

	int8_t w = _find_property(p->bits,_BIT_GET_SET_MEMBER);
	if(w != -1){
		return p->properties[w].get_set_member(o,str,nm);
	}
	else{
			w = _find_property(p->bits,_BIT_MEMBERS);
			if(w != -1){
				const struct fbgc_object_member *members = p->properties[w].members;
				uint8_t len = members->len;
				
				while(len--){
					if(!strcmp(members->member[len].name,str)){
						return members->member[len].function(o,nm);
					}
				}
			}
	}
	return NULL;
} 


struct fbgc_object * get_fbgc_object_method(struct fbgc_object * o, const char * str){
	
	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(o);

	int8_t w = _find_property(p->bits,_BIT_METHODS);
	if(w != -1){
		const struct fbgc_object_method * methods = p->properties[w].methods;
		uint8_t len = methods->len;
		while(len--){
			if(strcmp(methods->method[len].name,str) == 0){
				return new_fbgc_cfun_object(methods->method[len].function);
			}
		}
	}
	return NULL;
} 



struct fbgc_object * subscript_operator_fbgc_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){

	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(iterable);

	int8_t w = _find_property(p->bits,_BIT_SUBSCRIPT_OPERATOR);
	if(w != -1){
		return p->properties[w].subscript_operator(iterable,index_obj);
	}
	FBGC_LOGE("%s does not satisfy [] operator\n",objtp2str(iterable));
	return NULL;
	
}


struct fbgc_object * subscript_assign_operator_fbgc_object(struct fbgc_object * iterable,struct fbgc_object * index_obj, struct fbgc_object * rhs){

	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(iterable);

	int8_t w = _find_property(p->bits,_BIT_SUBSCRIPT_ASSIGN_OPERATOR);
	if(w != -1){
		return p->properties[w].subscript_assign_operator(iterable,index_obj,rhs);
	}
	FBGC_LOGE("%s does not satisfy []= operator\n",objtp2str(iterable));
	return NULL;
	
}

size_t size_of_fbgc_object(struct fbgc_object * obj){

	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(obj);

	int8_t w = _find_property(p->bits,_BIT_SIZE_OF);
	if(w != -1){
		return p->properties[w].size_of(obj);
	}
	FBGC_LOGE("%s does not satisfy size_of operator\n",objtp2str(obj));
	assert(0);
}

uint8_t gc_mark_fbgc_object(struct fbgc_object * obj){

	const struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(obj);


	int8_t w = _find_property(p->bits,_BIT_GC_MARK);
	if(w != -1){
		p->properties[w].gc_mark(obj);
		return 1;
	}
	return 0;
	FBGC_LOGW("No gc mark available for %s\n",objtp2str(obj));
}
