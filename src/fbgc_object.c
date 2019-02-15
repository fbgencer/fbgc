#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) malloc(sizeof(struct fbgc_object));
    o->type = token;
    o->next = NULL;  
    return (struct fbgc_object*) o;
}



struct
fbgc_object * new_fbgc_object_from_substr(struct fbgc_object * field_obj,const char *str1, const char*str2, fbgc_token token){
	struct fbgc_object *obj = NULL;

	switch(token){
		case INT: 
		case INT2:
		case INT16:
					obj = new_fbgc_int_object_from_substr(str1,str2,token);
					return obj;
		
		case DOUBLE: 
					obj = new_fbgc_double_object_from_substr(str1,str2); 
					return obj;
		case STRING: 
					obj = new_fbgc_str_object_from_substr(str1+1,str2-1); 
					return obj;
		case LPARA:			
		case RPARA:
		case LBRACE:
		case RBRACE:
		case LBRACK:
		case RBRACK:
		case OP: 
					obj = new_fbgc_object(get_operator_code_from_substr(str1,str2));
					if(obj->type == UNKNOWN){
						cprintf(100,"Undefined operator!\n");
					}
					return obj;
		case WORD:
					token = get_reserved_word_code_from_substr(str1,str2);
					if(token == UNKNOWN){
						//cprintf(111,"\nThis is not a keyword! \n"); it must be something idor function or class etc.
						//is this substring refers to a c function ?

						obj = new_fbgc_symbol_from_substr(field_obj,str1,str2);
						//handle symbol table!
					}else {
						return new_fbgc_object(token);
						 
					}
		break;
		case COMMA:
			return new_fbgc_object(COMMA);

		default:
			cprintf(111,"Undefined token inside new object creation !\n\n");
		break;
		
	}

    return (struct fbgc_object*) obj;
}


void print_fbgc_object(struct fbgc_object * self){
	
	if(self != NULL){ 

		switch(self->type){
			case INT:
				print_fbgc_int_object(self);
			break;
			case DOUBLE:
				print_fbgc_double_object(self);
			break;
			case STRING:
			case WORD:
				print_fbgc_str_object(self);
			break;
			case REFERENCE:
				print_fbgc_ref_object(self);
			break;
			case TUPLE:
				print_fbgc_tuple_object(self);
			break;			
			default:
				printf("\033[1;31m[%s]\033[0m",object_name_array[self->type]);  
			break;

		}
	}
	else cprintf(111,"NULL object cannot be printed!\n");
}


int convert_fbgc_object_to_int(struct fbgc_object * obj){

	if(get_fbgc_object_type(obj) == INT) return cast_fbgc_object_as_int(obj)->content;
	
	if(obj != NULL){
		switch(obj->type){
			case DOUBLE: 
				return (int)(cast_fbgc_object_as_double(obj)->content);
			default :
				cprintf(111,"Error at int conversion!\n");
				return -1;					
		}
	} 
	
	return 0;
}

double convert_fbgc_object_to_double(struct fbgc_object * obj){
	if(get_fbgc_object_type(obj) == DOUBLE) return cast_fbgc_object_as_double(obj)->content;


	if(obj != NULL){
		switch(obj->type){
			case INT:
				return (double)(cast_fbgc_object_as_int(obj)->content);
			default :
				cprintf(111,"Error at double conversion!\n");
				return -1;	
		}
	}
	return 0;
}


void free_fbgc_object(struct fbgc_object * self){
	cprintf(111,"Free["); print_fbgc_object(self); cprintf(111,"]\n");
	if(self != NULL){
		if(self->type == STRING || self->type == WORD) free_fbgc_str_object(self);
		else if(self->type== TUPLE) free_fbgc_tuple_object(self);
		else if(self->type == REFERENCE) free_fbgc_ref_object(self);
		else free(self);		
	}

}
