#include "../src/fbgc.h"
#include "fbgc_stl.h"

/*
#define declare_new_fbgc_cfunction(fun_name)\
struct fbgc_object * fun_name(struct fbgc_object * sm);\
extern const struct fbgc_cfunction fun_name##_struct

#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\
*/

//new_fbgc_cfunction(fbgc_print,"print")


new_fbgc_cfunction(fbgc_len,"len")
{
	if(argc == 1){
		return get_length_fbgc_object(arg[0]);
	}
	else cprintf(100,"<len> takes only 1 argument, %d given !\n",argc);
	return NULL;
}

new_fbgc_cfunction(fbgc_load,"load")
{	
	// load('module_name') : just returns module 
	// load('module_name','*') : loads all in the field object,
	// load('module_name','fun1','fun2','funN') : loads desired functions
	// 


	struct fbgc_object * res = NULL;

	if(argc == 1){
		if(arg[0]->type == STRING)
			return fbgc_load_module(content_fbgc_str_object(arg[0]),NULL,0);	
	}
	else if(argc > 1){
		for(uint8_t i = 0; i<argc; ++i){
			if(arg[i]->type != STRING) return NULL;
		}

		if(!my_strcmp(content_fbgc_str_object(arg[1]),"*")){
			res = fbgc_load_module(content_fbgc_str_object(arg[0]),NULL,1);
		}
		else{
			//fbgc_load_module_specific(const char * module_name, const char * fun_name){
			for(uint8_t i = 1; i<argc; ++i)
				res = fbgc_load_module(content_fbgc_str_object(arg[0]),content_fbgc_str_object(arg[i]),2);	
		}
	

	}
	
	return res;
	//load_module_in_field_object(main_field,&fbgc_math_module);
}



new_fbgc_cfunction(fbgc_id,"id"){
	if(argc == 1){
		fprintf(stdout,"%p\n",arg[0]);
	}
	else cprintf(100,"<id> takes only 1 argument, %d given !\n",argc);
	return NULL;
}


new_fbgc_cfunction(fbgc_type,"type"){
	if(argc == 1){
		return new_fbgc_int_object(arg[0]->type);
	}
	else cprintf(100,"<id> takes only 1 argument, %d given !\n",argc);
	return NULL;
}

new_fbgc_cfunction(fbgc_int,"int"){
	if(argc == 1){
		int d = convert_fbgc_object_to_int(arg[0]);
		return new_fbgc_int_object(d);
	}
	else cprintf(100,"<int> takes only 1 argument, %d given !\n",argc);
	return NULL;
}

new_fbgc_cfunction(fbgc_double,"double"){
	if(argc == 1){
		double d = convert_fbgc_object_to_double(arg[0]);
		return new_fbgc_double_object(d);
	}
	else cprintf(100,"<int> takes only 1 argument, %d given !\n",argc);
	return NULL;
}


new_fbgc_cfunction(fbgc_tuple,"tuple"){
	if(argc == 1){
		switch(arg[0]->type )
		{
			case INT:{
				return new_fbgc_tuple_object(cast_fbgc_object_as_int(arg[0])->content);
			}
			case STRING:
			{
				struct fbgc_object * s = arg[0];
				struct fbgc_object * t =  new_fbgc_tuple_object( length_fbgc_str_object(s) );
				for(int i = 0; i < length_fbgc_str_object(s) ; ++i )
					set_object_in_fbgc_tuple_object(t,get_object_in_fbgc_str_object(s,i,i+1), i);

				size_fbgc_tuple_object(t) = length_fbgc_str_object(s) ;				

				return t;
			}
			case RANGE:
			{
				//assume everything is integer..
				struct fbgc_range_object * ran = (struct fbgc_range_object*) arg[0];
				int start = cast_fbgc_object_as_int(ran->start)->content;
				int end = cast_fbgc_object_as_int(ran->end)->content;
				int step = ran->step == NULL ? 1 : cast_fbgc_object_as_int(ran->step)->content;

				int sz = (double)(end-start)/(step * 1.0);

				struct fbgc_object * tp = new_fbgc_tuple_object(sz);
				for(int i = start; i<end; i+=step){
					set_object_in_fbgc_tuple_object(tp,new_fbgc_int_object(i), i);
				}

				size_fbgc_tuple_object(tp) = sz;

				return tp;

			}
		}
	}
	else cprintf(100,"<tuple> takes only 1 argument, %d given !\n",argc);
	return NULL;
}

new_fbgc_cfunction(fbgc_matrix,"matrix"){
	
	if(argc == 3){
		size_t r = convert_fbgc_object_to_int(arg[0]);
		size_t c = convert_fbgc_object_to_int(arg[1]);
		return new_fbgc_matrix_object(DOUBLE,r,c,convert_fbgc_object_to_int(arg[2]));
	}
	else cprintf(100,"<int> takes only 3 argument, %d given !\n",argc);
	return NULL;
}


new_fbgc_cfunction(fbgc_mem,"mem"){
	if(argc == 0){
		print_fbgc_memory_block();
	}
	else cprintf(100,"<mem> takes only 0 argument, %d given !\n",argc);
	return NULL;
}


const struct fbgc_cfunction fbgc_stl_initializer_struct = {"stl",fbgc_stl_initializer};
extern struct fbgc_object * fbgc_stl_initializer (struct fbgc_object ** arg,int argc){
	return NULL;
}



//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_stl_module = 
{
	.initializer = &fbgc_stl_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_len_struct,
		&fbgc_load_struct,
		&fbgc_id_struct,
		&fbgc_int_struct,
		&fbgc_double_struct,
		&fbgc_tuple_struct,	
		&fbgc_matrix_struct,			
		&fbgc_type_struct,
		&fbgc_mem_struct,
		NULL
	}
};



/*
struct fbgc_object * fbgc_io_module_init(){
	struct fbgc_cmodule_object * fbgc_io_module = (struct fbgc_cmodule_object *)malloc(sizeof(fbgc_cmodule_object));
	fbgc_io_module.base->next = NULL;
	fbgc_io_module.base->type = UNKNOWN;
	fbgc_io_module->name; 
}*/
 
