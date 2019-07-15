#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) fbgc_malloc(sizeof(struct fbgc_object));
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
					break;
		case DOUBLE: 
					obj = new_fbgc_double_object_from_substr(str1,str2); 
					break;
		case STRING: 
					obj = new_fbgc_str_object_from_substr(str1+1,str2-1); 
					break;
		case LPARA:			
		case RPARA:
		case LBRACE:
		case RBRACE:
		case LBRACK:
		case RBRACK:
		case OP: 
					obj = new_fbgc_object(get_operator_code_from_substr(str1,str2));
					if(get_fbgc_object_type(obj) == UNKNOWN){
						cprintf(100,"Undefined operator!\n");
						obj = NULL;
					}
					break;
		case WORD:
					token = get_reserved_word_code_from_substr(str1,str2);
					if(token == UNKNOWN){
						//cprintf(111,"\nThis is not a keyword! \n"); it must be something idor function or class etc.
						//is this substring refers to a c function ?

						obj = new_fbgc_symbol_from_substr(field_obj,str1,str2);
						//handle symbol table!
					}else {
						obj  = new_fbgc_object(token);
						 
					}
		break;
		case COMMA:
			obj = new_fbgc_object(COMMA);
			break;
		default:
			cprintf(111,"Undefined token inside new object creation !\n\n");
		break;
		
	}

    return (struct fbgc_object*) obj;
}


void print_fbgc_object(struct fbgc_object * self){

	if(self != NULL){ 
		switch(get_fbgc_object_type(self)){
			case INT:
				print_fbgc_int_object(self);
			break;
			case DOUBLE:
				print_fbgc_double_object(self);
			break;
			case STRING:
				print_fbgc_str_object(self);
			break;
			case CSTRING:
				print_fbgc_cstr_object(self);
			break;
			case REFERENCE:
				print_fbgc_ref_object(self);
			break;
			case TUPLE:
				print_fbgc_tuple_object(self);
			break;			
			default:
				cprintf(100,"[%s]",object_type_as_str(self));  
			break;

		}
	}
	else cprintf(111,"NULL object cannot be printed!\n");
}



size_t get_fbgc_object_size(struct fbgc_object * obj){
	fbgc_token type = (get_fbgc_object_type(obj));

	switch(type){
		case INT: return sizeof(struct fbgc_int_object);
		case DOUBLE: return sizeof(struct fbgc_double_object);
		case STRING: return cast_fbgc_object_as_str(obj)->len+size_fbgc_str_object+1;
		case CSTRING: return size_fbgc_cstr_object+1+length_fbgc_cstr_object(obj);
		case TUPLE: return sizeof(struct fbgc_tuple_object)+(capacity_fbgc_tuple_object(obj) * sizeof(struct fbgc_object*)) ;
		case REFERENCE : return sizeof(struct fbgc_ref_object);
		case FIELD: return sizeof(struct fbgc_field_object);
		case LINKED_LIST: return sizeof(struct fbgc_ll_object);
		case LINKED_LIST_TAIL: return sizeof(struct fbgc_object);
		case GARBAGE: return cast_fbgc_object_as_garbage(obj)->size;
		default: break;
	}

	if(type>OP) return sizeof(struct fbgc_object);

	return 0;
}


int convert_fbgc_object_to_int(struct fbgc_object * obj){

	if(get_fbgc_object_type(obj) == INT) return cast_fbgc_object_as_int(obj)->content;
	
	if(obj != NULL){
		switch(get_fbgc_object_type(obj)){
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
		switch(get_fbgc_object_type(obj)){
			case INT:
				return (double)(cast_fbgc_object_as_int(obj)->content);
			default :
				cprintf(111,"Error at double conversion!\n");
				return -1;	
		}
	}
	return 0;
}


/*
void claim_ownership(struct fbgc_object * self){
	
	if(self != NULL){ 
		cprintf(011,"claim_ownership for [%s]\t",object_type_as_str(self)); print_fbgc_object(self); printf("\n");
		switch(get_fbgc_object_type(self)){
			case TUPLE:
				self->type |= 0x80;
				for(unsigned int i = 0; i<cast_fbgc_object_as_tuple(self)->size; i++){
					cprintf(101,"\tChildren:");
					claim_ownership(cast_fbgc_object_as_tuple(self)->contents[i]);
				}
			break;	
			case REFERENCE:
				claim_ownership(cast_fbgc_object_as_ref(self)->content->next);
			break;		
			default:
				self->type |= 0x80; 
			break;

		}
	}
	else cprintf(111,"NULL object cannot !!\n");
}
*/

void free_fbgc_object(struct fbgc_object * self){
/*		#ifdef FREE_DEBUG
			if(self == NULL) cprintf(100,"Null deleting..\n");
		#endif			

	if(self != NULL && (self->type & 0x80) != 0x80 ){
		#ifdef FREE_DEBUG
		cprintf(110,"Freeing["); print_fbgc_object(self); cprintf(110,"]\n");
		#endif			
		switch(self->type) {
			case STRING:
			case WORD:
				free_fbgc_str_object(self);
				break;
			case TUPLE:
				free_fbgc_tuple_object(self);
				break;
			default:
				free(self);
				break;
		}
	}
*/
}
