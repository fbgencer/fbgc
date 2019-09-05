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


// This is actual function struct
// Pointer holds C function itself, each C function must take 1 arg as fbgc_object and return 1 fbgc_object
struct fbgc_cfunction{
    const char *name;
    struct fbgc_object * (* function)(struct fbgc_object *);
};

//C module holds CONSTANT array of c functions, this array is (pre)determined during the compilation time
struct fbgc_cmodule{
    const struct fbgc_cfunction * initializer;
    const struct fbgc_cfunction ** functions;
};

//This is where c functions and modules interact with each other
//This is our object to hold in fields
struct fbgc_cmodule_object{
    struct fbgc_object base;
    const struct fbgc_cmodule * module;
    struct fbgc_object * variables; // array object 
};

struct fbgc_cfun_object{
	struct fbgc_object base;
	struct fbgc_object * (* function)(struct fbgc_object *);
};

#define cast_fbgc_object_as_cfun(x)(((struct fbgc_cfun_object*) x))



#define declare_new_fbgc_cfunction(fun_name)\
struct fbgc_object * fun_name(struct fbgc_object * sm);\
extern const struct fbgc_cfunction fun_name##_struct

#define new_fbgc_cfunction(fun_name,str_fun_name)\
const struct fbgc_cfunction fun_name##_struct  = {str_fun_name,fun_name};\
extern struct fbgc_object * fun_name(struct fbgc_object * arg)\



struct fbgc_object * new_fbgc_cfun_object( struct fbgc_object * (* function_obj)(struct fbgc_object *) );
void print_fbgc_cmodule(const struct fbgc_cmodule * );
void free_fbgc_cfun_object(struct fbgc_object * obj);

#define cfun_object_call(cfuno,obj)(cast_fbgc_object_as_cfun(cfuno)->function(obj))

#ifdef  __cplusplus
}
#endif

#endif