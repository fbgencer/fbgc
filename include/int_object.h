#ifndef FBGC_INT_OBJECTH
#define FBGC_INT_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif
/*! fbgc int object derived from base class and hold one int variable */

struct fbgc_int_object{
    struct fbgc_object base; /**< fbgc base structure */
    int content; /**< fbgc_int object holds integer value in this variable */
};

extern const struct fbgc_object_property_holder fbgc_int_object_property_holder;


#define cast_fbgc_object_as_int(x)(((struct fbgc_int_object*) x))
#define sizeof_fbgc_int_object(x)(sizeof(struct fbgc_int_object))


struct fbgc_object * new_fbgc_int_object(int x);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(const char *,const char *,fbgc_token );


uint8_t print_fbgc_int_object(struct fbgc_object *);
struct fbgc_object * fbgc_int_object_to_str(struct fbgc_object * obj);


void free_fbgc_int_object(struct fbgc_object * obj);


#ifdef  __cplusplus
}
#endif

#endif