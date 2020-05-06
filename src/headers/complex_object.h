#ifndef FBGC_COMPLEX_OBJECTH
#define FBGC_COMPLEX_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct raw_complex{
	double real;
	double imag;
};

struct fbgc_complex_object{
    struct fbgc_object base;
    struct raw_complex z;
};
extern const struct fbgc_object_property_holder fbgc_complex_object_property_holder;

#define cast_fbgc_object_as_complex(x)(((struct fbgc_complex_object*) x))
#define sizeof_fbgc_complex_object()(sizeof(struct fbgc_complex_object))
#define real_fbgc_complex_object(x)(cast_fbgc_object_as_complex(x)->z.real)
#define imag_fbgc_complex_object(x)(cast_fbgc_object_as_complex(x)->z.imag)

struct fbgc_object * new_fbgc_complex_object(double r, double z);
struct fbgc_object * new_fbgc_complex_object_from_str(const char *);
struct fbgc_object * new_fbgc_complex_object_from_substr(const char *,const char * );
struct fbgc_object * real_fbgc_complex_object_to_double(struct fbgc_object * self);
struct fbgc_object * imag_fbgc_complex_object_to_double(struct fbgc_object * self);
struct fbgc_object * conjugate_fbgc_complex_object(struct fbgc_object ** self,int i);
struct fbgc_object * subscript_fbgc_complex_object(struct fbgc_object * self,uint8_t index);

struct fbgc_object * operator_fbgc_complex_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);
struct raw_complex operator_method_raw_complex(struct raw_complex a1,struct raw_complex b1,fbgc_token op, struct fbgc_object ** result);

struct fbgc_object * get_set_fbgc_complex_object_member(struct fbgc_object * o,const char * name, struct fbgc_object * new_m);


uint8_t print_fbgc_complex_object(struct fbgc_object *);

#ifdef  __cplusplus
}
#endif

#endif