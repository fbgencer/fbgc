#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) fbgc_malloc(sizeof(struct fbgc_object));
    o->type = token;
    //o->next = NULL;  
    return (struct fbgc_object*) o;
}


void printf_fbgc_object(struct fbgc_object * self){

	assert(self != NULL);
	switch(get_fbgc_object_type(self))
	{
		case INT:
		{
			fprintf(stdout,"%d",cast_fbgc_object_as_int(self)->content);
			break;			
		}	
		case DOUBLE:
		{
			fprintf(stdout,"%g",cast_fbgc_object_as_double(self)->content);
			break;			
		}
		case COMPLEX:
		{
			fprintf(stdout,"%g%+gj",cast_fbgc_object_as_complex(self)->z.real,cast_fbgc_object_as_complex(self)->z.imag); 
			break;
		}
		case STRING:
		{
		    fprintf(stdout,"%s",&cast_fbgc_object_as_str(self)->content);   
			break;
		}
		case TUPLE:
		{
			struct fbgc_object ** contents = tuple_object_content(self);
			fprintf(stdout,"(");
			for(size_t i = 0; i<size_fbgc_tuple_object(self); i++){
				printf_fbgc_object(contents[i]);
				if(i < size_fbgc_tuple_object(self)-1 ) 
					fprintf(stdout,",");
			}
			fprintf(stdout,")");
			break;
		}
		case MATRIX:
		{
			    #define m cast_fbgc_object_as_matrix(self)

			    double * contents = matrix_object_content(self);

			    printf("[");
			    for(int i = 0; i<m->row; i++){
			        
			        for(int j = 0; j<m->column; j++){
			            printf("%f",contents[i * m->column + j]);
			            if(j != m->column-1) printf(",");
			        }
			        if(i!= m->row-1) printf("\n");
			    }
			    printf("]");
			    #undef m 
			break;
		}	
		case FUN:
		{
			fprintf(stdout,"[Function object<%p>]",self);
			break;
		}			
		default:
				fprintf(stdout,"Error undefined object!\n"); 
		break;

	}	
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
			case COMPLEX:
				print_fbgc_complex_object(self);
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
}



size_t get_fbgc_object_size(struct fbgc_object * obj){
	fbgc_token type = (get_fbgc_object_type(obj));

	switch(type){
		case INT: return sizeof(struct fbgc_int_object);
		case DOUBLE: return sizeof(struct fbgc_double_object);
		case STRING: return cast_fbgc_object_as_str(obj)->len+size_fbgc_str_object+1;
		case MATRIX: return sizeof(struct fbgc_matrix_object)+(cast_fbgc_object_as_matrix(obj)->column+cast_fbgc_object_as_matrix(obj)->row)*sizeof(double);
		case CSTRING: return size_fbgc_cstr_object+1+length_fbgc_cstr_object(obj);
		case TUPLE: return sizeof(struct fbgc_tuple_object)+(capacity_fbgc_tuple_object(obj) * sizeof(struct fbgc_object*)) ;
		//case REFERENCE : return sizeof(struct fbgc_ref_object);
		case FIELD: return sizeof(struct fbgc_field_object);
		case LINKED_LIST: return sizeof(struct fbgc_ll_object);
		case GARBAGE: return cast_fbgc_object_as_garbage(obj)->size;
		case ARRAY: return sizeof_fbgc_array_object(obj); 
		default: break;
	}

	if(type>THREE_DOT) return sizeof(struct fbgc_object);

	return 0;
}


int convert_fbgc_object_to_int(struct fbgc_object * obj){

	if(get_fbgc_object_type(obj) == INT) return cast_fbgc_object_as_int(obj)->content;
	
	switch(get_fbgc_object_type(obj)){
		case DOUBLE: 
			return (int)(cast_fbgc_object_as_double(obj)->content);
		default :
			cprintf(111,"Error at int conversion!\n");
			assert(0);					
	}
	
	
	return 0;
}

double convert_fbgc_object_to_double(struct fbgc_object * obj){
	if(get_fbgc_object_type(obj) == DOUBLE) return cast_fbgc_object_as_double(obj)->content;

	
	switch(get_fbgc_object_type(obj)){
		case INT:
			return (double)(cast_fbgc_object_as_int(obj)->content);
		default :

			cprintf(111,"Error at double conversion! type %s\n",object_name_array[obj->type]);
			assert(0);
	}
	
	return 0;
}


struct raw_complex convert_fbgc_object_to_complex(struct fbgc_object * obj){
	if(get_fbgc_object_type(obj) == COMPLEX) return cast_fbgc_object_as_complex(obj)->z;
	
	struct raw_complex z = {0,0};

	switch(get_fbgc_object_type(obj)){
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

/*
char convert_fbgc_object_to_str(struct fbgc_object * obj){
	if(get_fbgc_object_type(obj) == STRING) return &cast_fbgc_object_as_str(obj)->content;

	switch(get_fbgc_object_type(obj)){
		case INT:
				//new_fbgc_int_object(strtol(int_str_begin, NULL,base));
			return strtol(cast_fbgc_object_as_int(obj)->content);
		default :

			cprintf(111,"Error at double conversion! type %s\n",object_name_array[obj->type]);
			assert(0);	
	}
	
	return 0;
}*/


struct fbgc_object * get_length_fbgc_object(struct fbgc_object * t){
	switch(t->type){
		case INT:
		{
			return new_fbgc_int_object(abs(cast_fbgc_object_as_int(t)->content));
		}
		case DOUBLE:
		{
			return new_fbgc_double_object(fabs(cast_fbgc_object_as_double(t)->content));
		}
		case STRING:
		{
			return new_fbgc_int_object(cast_fbgc_object_as_str(t)->len);
		}				
		case TUPLE:
		{

			return new_fbgc_int_object(size_fbgc_tuple_object(t)) ;
		}
		case MATRIX:
		{
			struct fbgc_object * sz_tuple = new_fbgc_tuple_object(2);
			struct fbgc_object ** aa = tuple_object_content(sz_tuple);
			aa[0] = new_fbgc_int_object(cast_fbgc_object_as_matrix(t)->row);
			aa[1] = new_fbgc_int_object(cast_fbgc_object_as_matrix(t)->column);
			size_fbgc_tuple_object(sz_tuple) = 2;
			return sz_tuple;
		}
		default:
		{
			cprintf(100,"Not an iterable object type %s\n",object_name_array[t->type]);
			assert(0);
		}	
	}
} 


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
