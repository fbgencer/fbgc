#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) malloc(sizeof(struct fbgc_object));
    o->type = token;
    o->next = NULL;  
    return (struct fbgc_object*) o;
}

struct
fbgc_object * new_fbgc_object_from_str(char *str, fbgc_token token){
	struct fbgc_object *obj = NULL;

	switch(token){
		case INT: 
		case BIN:
		case HEX:
					obj = new_fbgc_int_object_from_str(str); 
					break;
		
		case DOUBLE: 
					obj = new_fbgc_double_object_from_str(str); 
					break;
		case STRING: 
					obj = new_fbgc_str_object(str); 
					break;
		case OP: 
					obj = new_fbgc_object(get_operator_code(str));
					if(obj->type == UNKNOWN){
						cprintf(100,"Undefined operator!\n");
					}
					break;
		case WORD:
					obj = new_fbgc_object(get_reserved_word_code(str));
					if(obj->type == UNKNOWN){
						obj->type = WORD;
						//handle symbol table!
					}
		break;
	}

    return (struct fbgc_object*) obj;
}

struct
fbgc_object * new_fbgc_object_from_substr(char *str1,char*str2, fbgc_token token){
	struct fbgc_object *obj = NULL;

	switch(token){
		case INT: 
		case BIN:
		case HEX:
					obj = new_fbgc_int_object_from_substr(str1,str2); 
					break;
		
		case DOUBLE: 
					obj = new_fbgc_double_object_from_substr(str1,str2); 
					break;
		case STRING: 
					obj = new_fbgc_str_object_from_substr(str1,str2); 
					break;
		case OP: 
					obj = new_fbgc_object(get_operator_code_from_substr(str1,str2));
					if(obj->type == UNKNOWN){
						cprintf(100,"Undefined operator!\n");
					}
					break;
		case WORD:
					obj = new_fbgc_object(get_reserved_word_code_from_substr(str1,str2));
					if(obj->type == UNKNOWN){
						obj->type = WORD;
						//handle symbol table!
					}
		break;
	}

    return (struct fbgc_object*) obj;
}


void print_fbgc_object(struct fbgc_object * self){
	
	switch(self->type){
		case INT:
			print_fbgc_int_object(self);
		break;
		case DOUBLE:
			print_fbgc_double_object(self);
		break;
		case STRING:
			print_fbgc_str_object(self);
		break;
		default:
			printf("\033[1;31m[%s]\033[0m\n",object_name_array[self->type]);  
		break;

	}
}

void free_fbgc_object(struct fbgc_object * self){
	if(self->type == STRING) free_fbgc_str_object(self);
	else free(self);
}
