#ifndef FUN_OBJECT_H
#define FUN_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_fun_object{ 
	struct fbgc_object base;
	struct fbgc_ll_base * code; //function linked list code
	//struct fbgc_object * defaults; //default tuple
	int8_t no_arg;
	int8_t no_locals;
};

extern const struct fbgc_object_property_holder fbgc_fun_object_property_holder;

#define cast_fbgc_object_as_fun(x)(((struct fbgc_fun_object*) x))
#define sizeof_fbgc_fun_object()(sizeof(struct fbgc_fun_object)) 

uint8_t print_fbgc_fun_object(struct fbgc_object *);
void fprintf_fbgc_fun_object(struct fbgc_object *);

struct fbgc_object * new_fbgc_fun_object();


#ifdef  __cplusplus
}
#endif

#endif