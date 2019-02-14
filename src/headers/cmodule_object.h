#ifndef CMODULE_OBJECT_H
#define CMODULE_OBJECT_H

/*
	module name : math
	module functions : sin,cos,tan..
	module constants : pi, e,
*/

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_cfunction{
    const char *name;
    struct fbgc_object * (* function)(struct fbgc_object *);
};

struct fbgc_cmodule_object{
    struct fbgc_object base;
    const char *name;
    const struct fbgc_cfunction ** functions;
};


struct fbgc_cfun_object{
	struct fbgc_object base;
	struct fbgc_object * (* function)(struct fbgc_object *);
};

#define cast_fbgc_object_as_cfun(x)(((struct fbgc_cfun_object*) x))



#define declare_new_fbgc_cfunction(fun_name)\
struct fbgc_object * fun_name(struct fbgc_object *);\
const struct fbgc_cfunction fun_name##_struct;

#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
struct fbgc_object * fun_name(struct fbgc_object * arg)



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object *) );
void free_fbgc_cfun_object(struct fbgc_object * obj);

#define cfun_object_call(cfuno,obj)(cast_fbgc_object_as_cfun(cfuno)->function(obj))

#ifdef  __cplusplus
}
#endif

#endif