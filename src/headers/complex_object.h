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

#define cast_fbgc_object_as_complex(x)(((struct fbgc_complex_object*) x))

#define sizeof_fbgc_complex_object()(sizeof(struct fbgc_complex_object))

struct fbgc_object * new_fbgc_complex_object(double r, double z);
struct fbgc_object * new_fbgc_complex_object_from_str(const char *);
struct fbgc_object * new_fbgc_complex_object_from_substr(const char *,const char * );
struct fbgc_object * real_fbgc_complex_object(struct fbgc_object * self);
struct fbgc_object * imag_fbgc_complex_object(struct fbgc_object * self);
struct fbgc_object * subscript_fbgc_complex_object(struct fbgc_object * self,uint8_t index);
struct fbgc_object * binary_op_fbgc_complex_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

void print_fbgc_complex_object(struct fbgc_object *);
void free_fbgc_complex_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif