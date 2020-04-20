#include "fbgc.h"

struct fbgc_object * new_fbgc_class_object(){ 
  struct fbgc_class_object * o = (struct fbgc_class_object*) fbgc_malloc(sizeof_fbgc_class_object());
    o->base.type = CLASS;
    o->code = NULL;
	o->locals = new_fbgc_array_object(1,sizeof(struct fbgc_identifier));
    return (struct fbgc_object*) o;
}


void inherit_from_another_class(struct fbgc_object * self,struct fbgc_object * father){

	struct fbgc_object * fao = cast_fbgc_object_as_class(father)->locals;
	struct fbgc_object * sao = cast_fbgc_object_as_class(self)->locals;
	struct fbgc_object * new_sao = sao;

	

	for(size_t i = 0; i<size_fbgc_array_object(fao); ++i){
		struct fbgc_identifier * fid = (struct fbgc_identifier *) get_address_in_fbgc_array_object(fao,i);
		uint8_t match = 0;
		FBGC_LOGV(CLASS_OBJECT,"Father id name:%s",content_fbgc_cstr_object(fid->name));

		for(size_t j = 0; j<size_fbgc_array_object(sao); ++j){
			struct fbgc_identifier * sid = (struct fbgc_identifier *) get_address_in_fbgc_array_object(sao,j);
			FBGC_LOGV(CLASS_OBJECT,"Child id name:%s",content_fbgc_cstr_object(sid->name));
			if(!my_strcmp(content_fbgc_cstr_object(fid->name),content_fbgc_cstr_object(sid->name))){
				//If there is a match then write to flag;
				FBGC_LOGV(CLASS_OBJECT,"Match!");
				match = 1;

			}
		}

		if(!match){
			new_sao = push_back_fbgc_array_object(new_sao,fid);
		}
		
	}


	cast_fbgc_object_as_class(self)->locals = new_sao;
}



//===============================================================================================

struct fbgc_object * new_fbgc_instance_object(struct fbgc_object * _template){ 
  struct fbgc_instance_object * o = (struct fbgc_instance_object*) fbgc_malloc(sizeof(struct fbgc_instance_object));
    o->base.type = INSTANCE;
    o->template_class = _template;

	struct fbgc_object * ao = cast_fbgc_object_as_class(_template)->locals;
	o->variables = new_fbgc_tuple_object(size_fbgc_array_object(ao));

	size_fbgc_tuple_object(o->variables) = size_fbgc_array_object(ao);

	for(int i = 0; i<size_fbgc_array_object(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		set_object_in_fbgc_tuple_object(o->variables,temp_id->content,i);
	}

    return (struct fbgc_object*) o;
}


struct fbgc_object ** get_set_fbgc_instance_object_member_address(struct fbgc_object * o, const char * str){
	//First get the location of this string

	int str_loc = -1;
	struct fbgc_object * ao = cast_fbgc_object_as_class(cast_fbgc_object_as_instance(o)->template_class)->locals;


	for(int i = 0; i<size_fbgc_array_object(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		
		if(!my_strcmp(content_fbgc_cstr_object(temp_id->name),str)){
			str_loc = i;
			break;
		}
	}


	return (str_loc != -1) ? content_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables)+str_loc : NULL;

}


struct fbgc_object * get_overloaded_member(struct fbgc_object * o, const char * str){
	char buf[10]; //Overloadble function names are always short
	sprintf(buf,"__%s__",str);
	return *(get_set_fbgc_instance_object_member_address(o,buf));
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
      cprintf(010,"CLASS[Local#:%d|",size_fbgc_array_object(cls->locals));
      _print_fbgc_ll_code(cls->code,NULL);
      cprintf(010,"]");
    }
}

void fprintf_fbgc_class_object(struct fbgc_object * obj){
	print_fbgc_class_object(obj);
}
