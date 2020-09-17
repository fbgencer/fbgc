#include "fbgc.h"

struct fbgc_object * new_fbgc_class_object(){ 
  struct fbgc_class_object * o = (struct fbgc_class_object*) fbgc_malloc_object(sizeof_fbgc_class_object());
    o->base.type = CLASS;
    o->code = NULL;
	o->locals = new_fbgc_vector(1,sizeof(struct fbgc_identifier));
	o->variables = new_fbgc_map_object(1,80);
    return (struct fbgc_object*) o;
}


void inherit_from_another_class(struct fbgc_object * self,struct fbgc_object * father){

	struct fbgc_object * faom = cast_fbgc_object_as_class(father)->variables;
	struct fbgc_object * saom = cast_fbgc_object_as_class(self)->variables;

	//Now available rn, we need to write merge two maps function

	//allow merge function NOT to update existed keys, because new class defined them for itself
	fbgc_map_object_merge(saom,faom,false);

	// struct fbgc_vector * fao = cast_fbgc_object_as_class(father)->locals;
	// struct fbgc_vector * sao = cast_fbgc_object_as_class(self)->locals;
	// struct fbgc_vector * new_sao = sao;

	

	// for(size_t i = 0; i<size_fbgc_vector(fao); ++i){
	// 	struct fbgc_identifier * fid = (struct fbgc_identifier *) get_item_fbgc_vector(fao,i);
	// 	uint8_t match = 0;
	// 	FBGC_LOGV(CLASS_OBJECT,"Father id name:%s",content_fbgc_str_object(fid->name));

	// 	for(size_t j = 0; j<size_fbgc_vector(sao); ++j){
	// 		struct fbgc_identifier * sid = (struct fbgc_identifier *) get_item_fbgc_vector(sao,j);
	// 		FBGC_LOGV(CLASS_OBJECT,"Child id name:%s",content_fbgc_str_object(sid->name));
	// 		if(!my_strcmp(content_fbgc_str_object(fid->name),content_fbgc_str_object(sid->name))){
	// 			//If there is a match then write to flag;
	// 			FBGC_LOGV(CLASS_OBJECT,"Match!");
	// 			match = 1;
	// 			break;
	// 		}
	// 	}

	// 	if(!match){
	// 		push_back_fbgc_vector(new_sao,fid);
	// 	}
		
	// }


	// cast_fbgc_object_as_class(self)->locals = new_sao;
}



//===============================================================================================

struct fbgc_object * new_fbgc_instance_object(struct fbgc_object * _template){ 
  struct fbgc_instance_object * o = (struct fbgc_instance_object*) fbgc_malloc_object(sizeof(struct fbgc_instance_object));
    o->base.type = INSTANCE;
    o->template_class = _template;

	// struct fbgc_vector * ao = cast_fbgc_object_as_class(_template)->locals;
	// o->variables = new_fbgc_tuple_object(size_fbgc_vector(ao));
	// size_fbgc_tuple_object(o->variables) = size_fbgc_vector(ao);
	// for(int i = 0; i<size_fbgc_vector(ao); i++){
	// 	struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(ao,i);
	// 	set_object_in_fbgc_tuple_object(o->variables,temp_id->content,i);
	// }


	struct fbgc_object * mapo = cast_fbgc_object_as_class(_template)->variables;
	o->variables = new_fbgc_tuple_object(size_fbgc_map_object(mapo));
	size_fbgc_tuple_object(o->variables) = size_fbgc_map_object(mapo);

	for(size_t i = 0; i<capacity_fbgc_map_object(mapo); i++){
		struct fbgc_map_pair * p  = fbgc_map_object_get_pair(mapo,i);
		if(p->key != NULL){
			set_object_in_fbgc_tuple_object(o->variables,p->value,i);
		}
	}

    return (struct fbgc_object*) o;
}




struct fbgc_object * get_overloaded_member(struct fbgc_object * o, const char * str){
	char buf[10]; //Overloadble function names are always short
	sprintf(buf,"__%s__",str);
	return get_set_fbgc_instance_object_member(o,buf,NULL);
}

struct fbgc_object * operator_fbgc_instance_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){

	struct fbgc_object * inso = (a->type == INSTANCE) ? a : b;

	struct fbgc_object * fun;
	switch(op)
	{   

	    case RSHIFT:{
	    	fun = get_overloaded_member(inso,">>");
	    	break;
	    }
	    case LSHIFT:{
	    	fun = get_overloaded_member(inso,"<<");
	    	break;
	    }
	    case STARSTAR:{
	    	fun = get_overloaded_member(inso,"**");
	    	break;
	    }
	    case SLASHSLASH:{
	    	fun = get_overloaded_member(inso,"//");
	    	break;
	    } 
	    case PLUS:{
	    	fun = get_overloaded_member(inso,"+");
	    	break;
	    }
	    case MINUS:{
	    	fun = get_overloaded_member(inso,"-");
	    	break;
	    }
	    case STAR:{
	    	fun = get_overloaded_member(inso,"*");
	    	break;
	    }
	    case SLASH:{
	    	fun = get_overloaded_member(inso,"//");
	    	break;
	    }
	    case CARET:{
	    	fun = get_overloaded_member(inso,"^");
	    	break;
	    }
	    case PERCENT:{
	    	fun = get_overloaded_member(inso,"%");
	    	break;
	    }
	    case LOEQ:{
	    	fun = get_overloaded_member(inso,"<=");
	    	break;
	    }
	    case GREQ:{
	    	fun = get_overloaded_member(inso,">=");
	    	break;
	    }
	    case EQEQ:{
	    	fun = get_overloaded_member(inso,"==");
	    	if(fun == NULL){
	    		//provide a default comparison
	    		return new_fbgc_logic_object(a == b);
	    	}
	    	break;
	    }
	    case NOTEQ:{
	    	fun = get_overloaded_member(inso,"!=");
	    	if(fun == NULL){
	    		//provide a default comparison
	    		return new_fbgc_logic_object(a != b);
	    	}
	    	break;	    	
	    }
	    case LOWER:{
	    	fun = get_overloaded_member(inso,"<");
	    	break;
	    }
	    case GREATER:{
	    	fun = get_overloaded_member(inso,">");
	    	break;
	    }
	    case PIPE:{
	    	fun = get_overloaded_member(inso,">");
	    	break;
	    }
	    case AMPERSAND:{
	    	fun = get_overloaded_member(inso,"&");
	    	break;
	    }
	    case EXCLAMATION:
	    case TILDE:
	    case UPLUS:
	    case UMINUS: break;
	}	

	if(fun != NULL){
		if(cast_fbgc_object_as_fun(fun)->no_arg != 2){
			FBGC_LOGE("Binary operators can only have two arguments, defined arg no :%d\n",cast_fbgc_object_as_fun(fun)->no_arg);
			return NULL;
		}

		fun = call_fun_object(fun);
		global_interpreter_packet->sctr += 1; //since we are called from binary op opcode we will increase sctr because interpreter will pop the arguments
		return fun;

	}
	

	printf("Operator is not overloaded!\n");
	return NULL;
}



void print_fbgc_class_object(struct fbgc_object * obj){
    struct fbgc_class_object * cls = cast_fbgc_object_as_class(obj);
  
    if(cls->code != NULL && !is_constant_and_token(cls->code,CLASS) && _cast_llbase_as_llconstant(cls->code)->content != obj ){
      cprintf(010,"CLASS[Local#:%d|",size_fbgc_vector(cls->locals));
      _print_fbgc_ll_code(cls->code,NULL);
      cprintf(010,"]");
    }
}

void fprintf_fbgc_class_object(struct fbgc_object * obj){
	print_fbgc_class_object(obj);
}


struct fbgc_object * get_set_fbgc_class_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){

	if(nm != NULL){
		//printf("Class variables cannot be changed!\n");
		//this is for the case if class defined before and user wants to change its definition
		return NULL;
	}

	// struct fbgc_vector * ao = cast_fbgc_object_as_class(o)->locals;
	// for(unsigned int i = 0;  i<size_fbgc_vector(ao); ++i){	
	// 	struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(ao,i);
	// 	//cprintf(111,"temp_idname = %s, str:%s => %d\n",content_fbgc_str_object(temp_id->name),str,my_strcmp(content_fbgc_str_object(temp_id->name),str));
	// 	if(!my_strcmp(content_fbgc_str_object(temp_id->name),str)){
	// 		//cprintf(111,"I am returning\n");
	// 		return temp_id->content;
	// 	}
	// }

	return fbgc_map_object_lookup_str(cast_fbgc_object_as_class(o)->variables,str);
}

const struct fbgc_object_property_holder fbgc_class_object_property_holder = {
	.bits = 
	_BIT_GET_SET_MEMBER
	,
	.properties ={
		{.get_set_member = &get_set_fbgc_class_object_member},			
	}
};




struct fbgc_object * get_set_fbgc_instance_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs){
	
	struct fbgc_object * mapo = cast_fbgc_object_as_class(cast_fbgc_object_as_instance(o)->template_class)->variables;
	ssize_t where = fbgc_map_object_get_key_index_str(mapo,str);

	if(where == -1) return NULL;

	if(rhs == NULL)
		return content_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables)[where];
	else 
		set_object_in_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables,rhs,where);

	// struct fbgc_vector * ao = cast_fbgc_object_as_class(cast_fbgc_object_as_instance(o)->template_class)->locals;

	// for(int i = 0; i<size_fbgc_vector(ao); i++){
	// 	struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_item_fbgc_vector(ao,i);
		
	// 	if(!my_strcmp(content_fbgc_str_object(temp_id->name),str)){
	// 		if(rhs == NULL) return content_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables)[i];
	// 		else set_object_in_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables,rhs,i);
	// 		return o;
	// 	}
	// }


	return NULL;
}

struct fbgc_object * subscript_operator_fbgc_instance_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){
	struct fbgc_object * fun = get_overloaded_member(iterable,"[]");
	if(fun != NULL && fun->type == FUN){
		if(cast_fbgc_object_as_fun(fun)->no_arg == 2){
			//we need only two arguments, self and index
			global_interpreter_packet->sp[global_interpreter_packet->sctr++] = iterable;
			global_interpreter_packet->sp[global_interpreter_packet->sctr++] = index_obj;
			fun = call_fun_object(fun);
			global_interpreter_packet->sctr--;
			return fun;
		}
		
	}
	
	//We can return the element of class, ith element e.g
	FBGC_LOGE("Iterator not overloaded");
	return NULL;
			
			
}

struct fbgc_object * subscript_assign_operator_fbgc_instance_object(struct fbgc_object * iterable,struct fbgc_object * index_obj, struct fbgc_object * rhs){
	struct fbgc_object * fun = get_overloaded_member(iterable,"[]=");
	if(fun != NULL && fun->type == FUN){
		if(cast_fbgc_object_as_fun(fun)->no_arg == 3){
			//we need only two arguments, self and index

			global_interpreter_packet->sp[global_interpreter_packet->sctr++] = iterable;
			global_interpreter_packet->sp[global_interpreter_packet->sctr++] = index_obj;
			global_interpreter_packet->sp[global_interpreter_packet->sctr++] = rhs;
			fun = call_fun_object(fun);
			global_interpreter_packet->sctr--;
			//global_interpreter_packet->sctr += 2;
			return fun;
		}	
	}
	FBGC_LOGE("Iterator not overloaded");
	return NULL;
}

static struct fbgc_object * abs_operator_fbgc_instance_object(struct fbgc_object * self){
	struct fbgc_object * fun = get_overloaded_member(self,"||");
	if(fun != NULL)
		fun = call_fun_object(fun);
	return fun;
}

uint8_t print_fbgc_instance_object(struct fbgc_object * self){
	//printf("called me\n");
	struct fbgc_object * fun = get_overloaded_member(self,"print");
	
	//fprintf_fbgc_fun_object(fun);
	if(fun != NULL){
		global_interpreter_packet->sctr += 1;
		fun = call_fun_object(fun);
		
	}
	return 1;
}


static inline size_t size_of_fbgc_instance_object(struct fbgc_object * self){
    return sizeof(struct fbgc_instance_object);
}



const struct fbgc_object_property_holder fbgc_instance_object_property_holder = {
	.bits = 
	_BIT_PRINT |
	_BIT_GET_SET_MEMBER |
	_BIT_SUBSCRIPT_OPERATOR |
	_BIT_SUBSCRIPT_ASSIGN_OPERATOR |
	_BIT_ABS_OPERATOR | 
	_BIT_SIZE_OF
	,
	.properties ={
		{.print = &print_fbgc_instance_object},
		{.get_set_member = &get_set_fbgc_instance_object_member},
        {.subscript_operator = &subscript_operator_fbgc_instance_object},
        {.subscript_assign_operator = &subscript_assign_operator_fbgc_instance_object},
        {.abs_operator = &abs_operator_fbgc_instance_object},
        {.size_of = &size_of_fbgc_instance_object},
	}
};