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
	size_t sz = 0;
switch(type){

	case UNKNOWN : sz = sizeof_fbgc_object(); break;  
	case NIL : sz = sizeof_fbgc_object(); break; 
	case INT : sz = sizeof_fbgc_int_object(); break; 
	case DOUBLE : sz = sizeof_fbgc_double_object(); break; 
	case COMPLEX : sz = sizeof_fbgc_complex_object(); break; 
	case STRING : sz = sizeof_fbgc_str_object(obj); break; 
	case MATRIX : sz = sizeof_fbgc_matrix_object(obj); break; 
	case TUPLE : sz = sizeof_fbgc_tuple_object(obj); break; 
	case CFUN : sz = sizeof_fbgc_cfun_object(); break; 
	case FUN : sz = sizeof_fbgc_fun_object(); break; 
	case NAME : sz = sizeof_fbgc_object(); break; 
	case IDENTIFIER : sz = sizeof_fbgc_id_opcode(); break; 
	case RANGE : sz = sizeof_fbgc_range_object(); break; 
	case REFERENCE : sz = sizeof_fbgc_object(); break; 
	case CSTRING : sz = sizeof_fbgc_cstr_object(obj); break; 
	case MONATRIX : sz = sizeof_fbgc_object(); break; 
	case NUPLE : sz = sizeof_fbgc_object(); break; 
	case MONUPLE : sz = sizeof_fbgc_object(); break; 
	case ARRAY : sz = sizeof_fbgc_array_object(obj); break; 
	case LINKED_LIST : sz = sizeof_fbgc_ll_object(); break; 
	case CMODULE : sz = sizeof_fbgc_cmodule_object(); break; 
	case GARBAGE : sz = sizeof_fbgc_garbage_object(obj); break; 
	case FIELD : sz = sizeof_fbgc_field_object(obj); break; 
	case END : sz = sizeof_fbgc_object(); break; 
	case FUN_MAKE : 
	case ELIF : 
	case ELSE : 
	case WHILE : 
	case FOR : 
	case BREAK : 
	case CONT : sz = sizeof_fbgc_jumper_object(); break; 
	case LOAD : sz = sizeof_fbgc_object(); break; 
	case TRUE : sz = sizeof_fbgc_object(); break; 
	case FALSE : sz = sizeof_fbgc_object(); break; 
	case IF : sz = sizeof_fbgc_jumper_object(); break; 
	case RETURN : sz = sizeof_fbgc_object(); break; 
	case NEWLINE : 
	case LPARA :
	case RPARA :
	case LBRACK :
	case RBRACK :
	case LBRACE :
	case RBRACE :
	case PLUS_ASSIGN :
	case MINUS_ASSIGN :
	case STAR_ASSIGN :
	case SLASH_ASSIGN :
	case PLUSPLUS :
	case MINUSMINUS :
	case STARSTAR :
	case SLASHSLASH :
	case LO_EQ :
	case GR_EQ :
	case EQ_EQ :
	case NOT_EQ :
	case R_SHIFT :
	case L_SHIFT :
	case CARET :
	case PERCENT :
	case LOWER :
	case GREATER :
	case PIPE :
	case AMPERSAND :
	case SLASH :
	case STAR :
	case MINUS :
	case PLUS :
	case EXCLAMATION :
	case TILDE :
	case SEMICOLON :
	case COLON : 
	case ASSIGN : sz = sizeof_fbgc_object(); break;  
	case COMMA :
	case DOT :
	case UMINUS :
	case UPLUS :
	case START :
	case JUMP :
	case AND :
	case OR :
	case NOT : sz = sizeof_fbgc_object(); break; 
	case ROW : sz = sizeof_fbgc_int_object(); break; 
	case EXPRESSION :
	case ASSIGNMENT :
	case STATEMENT :
	case ASSIGNMENT_EXPRESSION :
	case UNARY_EXPRESSION :
	case BINARY_EXPRESSION :
	case BALANCED_EXPRESSION_LIST :
	case UNBALANCED_EXPRESSION_LIST : sz = sizeof_fbgc_object(); break; 
	case ASSIGN_SUBSCRIPT :
	case LOAD_SUBSCRIPT : break;
	case LEN : sz = sizeof_fbgc_object();
	case IF_BEGIN : 
	case ELIF_BEGIN :
	case WHILE_BEGIN : 
	case FOR_BEGIN : sz = sizeof_fbgc_jumper_object(); break;
	case FUN_CALL : sz = sizeof_fbgc_object(); break;
	case BUILD_TUPLE :
	case BUILD_MATRIX : sz = sizeof_fbgc_int_object(); break;

}

	/*switch(type){
		case INT: return sizeof_fbgc_int_object();
		case DOUBLE: return sizeof_fbgc_double_object();
		case STRING: return sizeof_fbgc_str_object(obj);
		case MATRIX: return sizeof_fbgc_matrix_object(obj);
		case CSTRING: return sizeof_fbgc_cstr_object(obj);
		case TUPLE: return sizeof_fbgc_tuple_object(obj);
		case FIELD: return sizeof_fbgc_field_object();
		case LINKED_LIST: return sizeof_fbgc_ll_object();
		case GARBAGE: return sizeof_fbgc_garbage_object(obj);
		case ARRAY: return sizeof_fbgc_array_object(obj); 
		default: break;
	}

	if(type>THREE_DOT) return sizeof(struct fbgc_object);*/

	return sz;
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
