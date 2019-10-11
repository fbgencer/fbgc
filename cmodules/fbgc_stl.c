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
			case INT:
				return new_fbgc_tuple_object(cast_fbgc_object_as_int(arg[0]));
			case STRING:
			{
				struct fbgc_object * s = arg[0];
				struct fbgc_object * t =  new_fbgc_tuple_object( length_fbgc_str_object(s) );
				for(int i = 0; i < length_fbgc_str_object(s) ; ++i )
					set_object_in_fbgc_tuple_object(t,subscript_fbgc_str_object(s,i,i+1), i);

				size_fbgc_tuple_object(t) = length_fbgc_str_object(s) ;				

				return t;
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
	return arg;
}



//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_stl_module = 
{
	.initializer = &fbgc_stl_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_len_struct,
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
 
