#include "fbgc.h"


struct fbgc_object * new_fbgc_object(const fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) fbgc_malloc(sizeof(struct fbgc_object));
    o->type = token;
    return (struct fbgc_object*) o;
}
const char * objtp2str(struct fbgc_object * obj){
	return object_name_array[obj->type];
}

void printf_fbgc_object(struct fbgc_object * self){

	assert(self != NULL);
	switch(get_fbgc_object_type(self))
	{
		case LOGIC:
		{
			fprintf(stdout, "%s", (cast_fbgc_object_as_logic(self)->content) ? "true" :"false");
			break;
		}
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
		    fprintf(stdout,"%s",cast_fbgc_object_as_str(self)->content);   
			break;
		}
		case TUPLE:
		{
			struct fbgc_object ** contents = content_fbgc_tuple_object(self);
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

			    double * contents = content_fbgc_matrix_object(self);

			    int width = 8;
			    int slash_widht = m->column*width+1;
				if(m->sub_type == COMPLEX)
					slash_widht += 5;

			    //fprintf(stdout,"%2s%*s\n%1s","/",slash_widht,"\\","|");
				char iscomplex = (m->sub_type == COMPLEX);
			
			    for(int i = 0; i<m->row; ++i){
			        for(int j = 0; j<m->column; ++j){

			        	size_t index = (i * m->column + j)<<iscomplex;

			    		fprintf(stdout,"%*g",width,contents[index]);
        			    
        			    if(iscomplex){
			    			fprintf(stdout,"%+gj%*s",contents[index+1],width," " );
			    		}
			        }
			        if(i!=m->row-1) 
			        	fprintf(stdout,"\n");
			        //if(i!= m->row-1) fprintf(stdout,"%3s\n%1s","|","|");
			    }
			    //fprintf(stdout,"%3s\n%2s%*s","|","\\",slash_widht,"/");
			    #undef m 
			break;
		}

		case FUN:
		{	
			fprintf_fbgc_fun_object(self);
			//fprintf(stdout,"[Function object<%p>]",self);
			break;
		}
		case CLASS:
		{	
			fprintf_fbgc_class_object(self);
			//fprintf(stdout,"[Function object<%p>]",self);
			break;
		}
		case RANGE:{
			printf_fbgc_object(cast_fbgc_object_as_range(self)->start);
			printf(":");
			if(cast_fbgc_object_as_range(self)->step != NULL){
				printf_fbgc_object(cast_fbgc_object_as_range(self)->step);
				printf(":");
			}
			printf_fbgc_object(cast_fbgc_object_as_range(self)->end);
			break;
		}
		default:
				fprintf(stdout,"Error undefined object %s!\n",object_name_array[self->type]);
				assert(0); 
		break;

	}	
}


uint8_t print_fbgc_object(struct fbgc_object * self){

	if(self != NULL){ 
		switch(get_fbgc_object_type(self)){
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
			case CSTRING:
				print_fbgc_cstr_object(self);
			break;
			case IDENTIFIER:
			{
				if(is_id_flag_GLOBAL(self)) cprintf(011,"%s{G<%d>}","ID",_cast_fbgc_object_as_llidentifier(self)->loc);
            	else if(is_id_flag_LOCAL(self)) cprintf(011,"%s{L<%d>}","ID",_cast_fbgc_object_as_llidentifier(self)->loc);
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
	return 1;
}



size_t get_fbgc_object_size(struct fbgc_object * obj){
	fbgc_token type = (get_fbgc_object_type(obj));
	size_t sz = 0;
switch(type){

	case UNKNOWN : sz = sizeof_fbgc_object(); break;  
	case NIL : sz = sizeof_fbgc_object(); break;
	case LOGIC: sz = sizeof_fbgc_logic_object(); break; 
	case INT : sz = sizeof_fbgc_int_object(); break; 
	case DOUBLE : sz = sizeof_fbgc_double_object(); break; 
	case COMPLEX : sz = sizeof_fbgc_complex_object(); break; 
	case STRING : sz = sizeof_fbgc_str_object(obj); break; 
	case MATRIX : sz = sizeof_fbgc_matrix_object(obj); break; 
	case TUPLE : sz = sizeof_fbgc_tuple_object(obj); break; 
	case CFUN : sz = sizeof_fbgc_cfun_object(); break; 
	case FUN : sz = sizeof_fbgc_fun_object(); break; 
	case CSTRUCT : sz = sizeof_fbgc_cstruct_object(obj); break; 
	case IDENTIFIER : sz = sizeof_fbgc_ll_identifier(); break; 
	case RANGE : sz = sizeof_fbgc_range_object(); break; 
	case CSTRING : sz = sizeof_fbgc_cstr_object(obj); break; 
	case ARRAY : sz = sizeof_fbgc_array_object(obj); break; 
	//case LINKED_LIST : sz = sizeof_fbgc_ll_object(); break; 
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
	//case CONT : sz = sizeof_fbgc_jumper_object(); break; 
	//case IF : sz = sizeof_fbgc_jumper_object(); break; 
	case RETURN : sz = sizeof_fbgc_object(); break; 
	case NEWLINE : 
	case LPARA :
	case RPARA :
	case LBRACK :
	case RBRACK :
	case LBRACE :
	case RBRACE :
	case STARSTAR :
	case SLASHSLASH :
	case LOEQ :
	case GREQ :
	case EQEQ :
	case NOTEQ :
	case RSHIFT :
	case LSHIFT :
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
	case JUMP :
	case ROW : sz = sizeof_fbgc_int_object(); break;
	case ASSIGN_SUBSCRIPT :
	case LOAD_SUBSCRIPT : break;
	case LEN : sz = sizeof_fbgc_object();
	//case FOR_BEGIN : sz = sizeof_fbgc_jumper_object(); break;
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

char convert_fbgc_object_to_logic(struct fbgc_object * obj){
	
	switch(get_fbgc_object_type(obj)){
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

	switch(get_fbgc_object_type(obj)){
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

	switch(get_fbgc_object_type(obj)){
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
			struct fbgc_object ** aa = content_fbgc_tuple_object(sz_tuple);
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


struct fbgc_object * get_set_fbgc_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){

	//! \todo arrange the case order 
	switch(o->type)
	{
		//case INT: return get_set_fbgc_int_object_member(o,str,nm);
		//case DOUBLE: return get_set_fbgc_double_object_member(o,str,nm);
		case COMPLEX: return get_set_fbgc_complex_object_member(o,str,nm);
		case CSTRUCT:
		{
			struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(o);
			const struct fbgc_cmodule * cm = so->parent;
			for (int i = 0; ; ++i){
				const struct fbgc_cfunction * cc = cm->functions[i];
				if(cc == NULL) break;
				
				if(!my_strcmp(str,cc->name)){
					return new_fbgc_cfun_object(cc->function);
				} 
				
			}
			return NULL;
		}
		case CMODULE:{
			struct fbgc_cmodule_object * cm = cast_fbgc_object_as_cmodule(o);
			const struct fbgc_cfunction * cc = cm->module->functions[0];
			for (int i = 1; cc!=NULL; ++i){
				if(!my_strcmp(str,cc->name) ){
					return new_fbgc_cfun_object(cc->function);
				} 
				//cprintf(101,"{%s}\n",cc->name);
				cc = cm->module->functions[i];
			}
			
			return NULL;
		}
		case FIELD:{
			if(nm!= NULL) return NULL;

			struct fbgc_object * ao = cast_fbgc_object_as_field(o)->locals;
			for(unsigned int i = 0;  i<size_fbgc_array_object(ao); ++i){	
				struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
				//cprintf(111,"temp_idname = %s, str:%s => %d\n",content_fbgc_cstr_object(temp_id->name),str,my_strcmp(content_fbgc_cstr_object(temp_id->name),str));
				if(!my_strcmp(content_fbgc_cstr_object(temp_id->name),str)){
					//cprintf(111,"I am returning\n");
					if(nm != NULL){
						temp_id->content = nm;	
					}
					return temp_id->content;
				}
			}

			return NULL;
		}
		case CLASS:{

			if(nm != NULL){
				//printf("Class variables cannot be changed!\n");
				//this is for the case if class defined before and user wants to change its definition
				return NULL;
			}
			struct fbgc_object * ao = cast_fbgc_object_as_class(o)->locals;
			for(unsigned int i = 0;  i<size_fbgc_array_object(ao); ++i){	
				struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
				//cprintf(111,"temp_idname = %s, str:%s => %d\n",content_fbgc_cstr_object(temp_id->name),str,my_strcmp(content_fbgc_cstr_object(temp_id->name),str));
				if(!my_strcmp(content_fbgc_cstr_object(temp_id->name),str)){
					//cprintf(111,"I am returning\n");
					return temp_id->content;
				}
			}

			return NULL;
		}
		case INSTANCE:{
			struct fbgc_object ** adr = get_set_fbgc_instance_object_member_address(o,str);
			return *adr;
			//return get_set_fbgc_instance_object_member(o,str,nm);
		}

		default:
			cprintf(100,"[%s] cannot accessible\n",object_name_array[o->type]);
			assert(0);

		return NULL;
	}
} 



struct fbgc_object ** get_address_fbgc_object_member(struct fbgc_object * o, const char * str){
	switch(o->type)
	{
		//case INT: return get_set_fbgc_int_object_member(o,str,nm);
		//case DOUBLE: return get_set_fbgc_double_object_member(o,str,nm);
		/*case COMPLEX: return get_set_fbgc_complex_object_member(o,str,nm);
		case CSTRUCT:
		{
			struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(o);
			const struct fbgc_cmodule * cm = so->parent;
			for (int i = 0; ; ++i){
				const struct fbgc_cfunction * cc = cm->functions[i];
				if(cc == NULL) break;
				
				if(!my_strcmp(str,cc->name)){
					return new_fbgc_cfun_object(cc->function);
				} 
				
			}
			return NULL;
		}
		case FIELD:{
			if(nm!= NULL) return NULL;

			struct fbgc_object * ao = cast_fbgc_object_as_field(o)->locals;
			for(unsigned int i = 0;  i<size_fbgc_array_object(ao); ++i){	
				struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
				//cprintf(111,"temp_idname = %s, str:%s => %d\n",content_fbgc_cstr_object(temp_id->name),str,my_strcmp(content_fbgc_cstr_object(temp_id->name),str));
				if(!my_strcmp(content_fbgc_cstr_object(temp_id->name),str)){
					//cprintf(111,"I am returning\n");
					if(nm != NULL){
						temp_id->content = nm;	
					}
					return temp_id->content;
				}
			}

			return NULL;
		}*/

		case INSTANCE:{
			return get_set_fbgc_instance_object_member_address(o,str);
		}

		default:
			cprintf(100,"[%s] cannot accessible\n",object_name_array[o->type]);
			break;

		return NULL;
	}
}


struct fbgc_object * iterator_get_fbgc_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){
	switch(iterable->type){
		case COMPLEX:{

			return NULL;
		}
		case STRING:{

			if(index_obj->type == INT){
				iterable = get_object_in_fbgc_str_object(iterable,cast_fbgc_object_as_int(index_obj)->content,1);	
			}
			else if(index_obj->type == RANGE){
				if(get_fbgc_range_object_iter_type(index_obj) != INT) return NULL;
				int i1 = cast_fbgc_object_as_int(cast_fbgc_object_as_range(index_obj)->start)->content;
				int len = cast_fbgc_object_as_int(cast_fbgc_object_as_range(index_obj)->end)->content - i1;
				iterable = get_object_in_fbgc_str_object(iterable,i1,len);
			}
			else{
				FBGC_LOGE("Index value must be integer");
				return NULL;
			}
			
			break;
		}
		case TUPLE:{
			if(index_obj->type != INT){
				FBGC_LOGE("Index value must be integer");
				return NULL;
			}
			iterable = get_object_in_fbgc_tuple_object(iterable,cast_fbgc_object_as_int(index_obj)->content);
			break;
		}
		case MATRIX:{
			return NULL;
		}
		case INSTANCE:{

			
		}
		default:{
			return NULL;
		}
	}

	if(iterable == NULL){
		FBGC_LOGE("Index value out of range");
		return NULL;	
	}
	else return iterable;
	
}


struct fbgc_object * iterator_set_fbgc_object(struct fbgc_object * iterable,struct fbgc_object * index_obj, struct fbgc_object * rhs){
	switch(iterable->type){
		case COMPLEX:{

			break;
		}
		case STRING:{

			if(index_obj->type != INT){
				FBGC_LOGE("Index value must be integer");
				return NULL;
			}
			return set_object_in_fbgc_str_object(iterable,cast_fbgc_object_as_int(index_obj)->content,1,rhs);
			
		}
		case TUPLE:{
			if(index_obj->type != INT){
				FBGC_LOGE("Index value must be integer");
				return NULL;
			}
			iterable = set_object_in_fbgc_tuple_object(iterable,rhs,cast_fbgc_object_as_int(index_obj)->content);
			if(iterable == NULL){
				FBGC_LOGE("Index value out of range");
				return NULL;	
			}
			return iterable;
		}
		case MATRIX:{
			break;
		}

		default:{
			break;
		}
	}
	return NULL;
}
