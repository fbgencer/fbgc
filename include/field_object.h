#ifndef FIELD_OBJECT_H
#define FIELD_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

#define fbgc_field_object fbgc_class_object
#define fbgc_field_object_property_holder fbgc_class_object_property_holder

#define cast_fbgc_object_as_field(x)(((struct fbgc_field_object*) x))
#define sizeof_fbgc_field_object(x)(sizeof(struct fbgc_field_object) )

struct fbgc_object * get_fbgc_field_object_cmodules(struct fbgc_object * field_obj);
struct fbgc_object * new_fbgc_field_object(void);
struct fbgc_ll_base * add_variable_in_field_object(struct fbgc_object * field_obj,const char * var_name, struct fbgc_object * rhs);
void free_fbgc_field_object(struct fbgc_object * field_obj);
uint8_t print_field_object_locals(struct fbgc_object * field_obj);

struct fbgc_object * get_set_fbgc_field_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm);

#ifdef  __cplusplus
}
#endif

#endif