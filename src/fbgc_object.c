#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) fbgc_malloc(sizeof(struct fbgc_object));
    o->type = token;
    o->next = NULL;  
    return (struct fbgc_object*) o;
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
			case IDENTIFIER:
			{
				if(is_id_flag_GLOBAL(self)) cprintf(011,"%s{G<%d>}","ID",cast_fbgc_object_as_id_opcode(self)->loc);
            	else if(is_id_flag_LOCAL(self)) cprintf(011,"%s{L<%d>}","ID",cast_fbgc_object_as_id_opcode(self)->loc);
            	else if(is_id_flag_SUBSCRIPT(self)) cprintf(011,"%s{S<%d>}","ID",cast_fbgc_object_as_id_opcode(self)->loc);
				
				break;
			}
			case TUPLE:
				print_fbgc_tuple_object(self);
			break;
			case FUN:
				print_fbgc_fun_object(self);
			break;			
			default:
				cprintf(100,"[%s]",object_type_as_str(self));  
			break;

		}
	}
	else cprintf(111,"[NULL]");
}



size_t get_fbgc_object_size(struct fbgc_object * obj){
	fbgc_token type = (get_fbgc_object_type(obj));

	switch(type){
		case INT: return sizeof(struct fbgc_int_object);
		case DOUBLE: return sizeof(struct fbgc_double_object);
		case STRING: return cast_fbgc_object_as_str(obj)->len+size_fbgc_str_object+1;
		case CSTRING: return size_fbgc_cstr_object+1+length_fbgc_cstr_object(obj);
		case TUPLE: return sizeof(struct fbgc_tuple_object)+(capacity_fbgc_tuple_object(obj) * sizeof(struct fbgc_object*)) ;
		//case REFERENCE : return sizeof(struct fbgc_ref_object);
		case FIELD: return sizeof(struct fbgc_field_object);
		case LINKED_LIST: return sizeof(struct fbgc_ll_object);
		case LINKED_LIST_TAIL: return sizeof(struct fbgc_object);
		case GARBAGE: return cast_fbgc_object_as_garbage(obj)->size;
		case ARRAY: return sizeof_fbgc_array_object(obj); 
		default: break;
	}

	if(type>THREE_DOT) return sizeof(struct fbgc_object);

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
