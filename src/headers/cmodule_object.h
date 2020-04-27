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

struct _fbgc_cmodule{
    const struct fbgc_cfunction initializer;
    const struct fbgc_object_property_holder * properties;
};


//C module holds CONSTANT array of c functions, this array is (pre)determined during the compilation time
struct fbgc_cmodule{
    const struct fbgc_cfunction * initializer; //do we need this?
    const struct fbgc_cfunction ** functions;
};

//This is where c functions and modules interact with each other
//This is our object to hold in fields
struct fbgc_cmodule_object{
    struct fbgc_object base;
    const struct fbgc_cmodule * module;
};

extern const struct fbgc_object_property_holder fbgc_cmodule_object_property_holder;

#define sizeof_fbgc_cmodule_object()(sizeof(struct fbgc_cmodule_object))

struct fbgc_cfun_object{
	struct fbgc_object base;
	struct fbgc_object * (* function)(struct fbgc_object **, int);
};

extern const struct fbgc_object_property_holder fbgc_cfun_object_property_holder;

#define sizeof_fbgc_cfun_object()(sizeof(struct fbgc_cfun_object))


#define cast_fbgc_object_as_cfun(x)(((struct fbgc_cfun_object*) x))
#define cast_fbgc_object_as_cmodule(x)(((struct fbgc_cmodule_object*) x))



#define declare_new_fbgc_cfunction(fun_name)\
struct fbgc_object * fun_name(struct fbgc_object ** arg, int argc);\
extern const struct fbgc_cfunction fun_name##_struct

#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object ** arg, int argc)\



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object **, int argc) );
struct fbgc_object * new_fbgc_cmodule_object(const struct fbgc_cmodule * cm);
void print_fbgc_cmodule(const struct fbgc_cmodule * );
void free_fbgc_cfun_object(struct fbgc_object * obj);
struct fbgc_object * get_set_fbgc_cmodule_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * rhs);


struct fbgc_object * new_cfun_object_from_str(struct fbgc_object * field_obj,const char * str);

#define cfun_object_call(cfuno,args,argc)(cast_fbgc_object_as_cfun(cfuno)->function(args,argc))

#ifdef  __cplusplus
}
#endif

#endif