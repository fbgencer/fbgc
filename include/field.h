#ifndef FIELD_H
#define FIELD_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_object * new_fbgc_field(uint8_t module_no, ...);
struct fbgc_ll_base * add_variable_in_field(struct fbgc_object * fobj,const char * var_name, struct fbgc_object * rhs);
uint8_t print_field_locals(struct fbgc_object * fobj);
struct fbgc_object * get_fbgc_field_cmodules(struct fbgc_object * o);

#define cast_fbgc_object_as_field(x)(((struct fbgc_class_object*) x))


#ifdef  __cplusplus
}
#endif

#endif