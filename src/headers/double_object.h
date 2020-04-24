#ifndef FBGC_DOUBLE_OBJECTH
#define FBGC_DOUBLE_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_double_object{
    struct fbgc_object base;
    double content;
};

extern const struct fbgc_object_property_holder fbgc_double_object_property_holder;

#define cast_fbgc_object_as_double(x)(((struct fbgc_double_object*) x))
#define sizeof_fbgc_double_object() (sizeof(struct fbgc_double_object)) 


struct fbgc_object * new_fbgc_double_object(double);
struct fbgc_object * new_fbgc_double_object_from_str(const char *);
struct fbgc_object * new_fbgc_double_object_from_substr(const char *,const char *);

struct fbgc_object * operator_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);
double operator_method_double(double a1,double b1,fbgc_token op,struct fbgc_object **);

struct fbgc_object * fbgc_double_object_to_str(struct fbgc_object * obj);


uint8_t print_fbgc_double_object(struct fbgc_object *);
void free_fbgc_double_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif
