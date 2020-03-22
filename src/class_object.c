#include "fbgc.h"

struct fbgc_object * new_fbgc_class_object(){ 
  struct fbgc_class_object * o = (struct fbgc_class_object*) fbgc_malloc(sizeof_fbgc_class_object());
    o->base.type = CLASS;
    o->code = NULL;
	o->locals = new_fbgc_array_object(1,sizeof(struct fbgc_identifier));
    return (struct fbgc_object*) o;
}

struct fbgc_object * new_fbgc_instance_object(struct fbgc_object * template){ 
  struct fbgc_instance_object * o = (struct fbgc_instance_object*) fbgc_malloc(sizeof(struct fbgc_instance_object));
    o->base.type = INSTANCE;
    o->template_class = template;

	struct fbgc_object * ao = cast_fbgc_object_as_class(template)->locals;
	o->variables = new_fbgc_tuple_object(size_fbgc_array_object(ao));

	size_fbgc_tuple_object(o->variables) = size_fbgc_array_object(ao);

	for(int i = 0; i<size_fbgc_array_object(ao); i++){
		struct fbgc_identifier * temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(ao,i);
		set_object_in_fbgc_tuple_object(o->variables,temp_id->content,i);
	}

    return (struct fbgc_object*) o;
}


struct fbgc_object * get_set_fbgc_instance_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){
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
	if(str_loc == -1){
		//Couldn't find
		return NULL;
	}

	if(nm == NULL){
		return get_object_in_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables,str_loc);
	}

	//So nm is not null, set the new value
	set_object_in_fbgc_tuple_object(cast_fbgc_object_as_instance(o)->variables,nm,str_loc);
	return nm;

}



void print_fbgc_class_object(struct fbgc_object * obj){
    struct fbgc_class_object * cls = cast_fbgc_object_as_class(obj);
  
    if(cls->code != NULL){
      cprintf(010,"CLASS[Local#:%d|",size_fbgc_array_object(cls->locals));
      _print_fbgc_ll_code(cls->code,NULL);
      cprintf(010,"]");
    }
}

void fprintf_fbgc_class_object(struct fbgc_object * obj){

}
