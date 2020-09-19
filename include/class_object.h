#ifndef CLASS_OBJECT_H
#define CLASS_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_class_object{ 
	struct fbgc_object base;
	struct fbgc_ll_base * code; //class linked list code
	struct fbgc_object * variables; // class variable map
};
extern const struct fbgc_object_property_holder fbgc_class_object_property_holder;

struct fbgc_instance_object{
	struct fbgc_object base;
	struct fbgc_object * template_class; //The class which creates this instance
	struct fbgc_object * variables; //tuple from class variable map-values, does not contain keys we will find with indexes
};

extern const struct fbgc_object_property_holder fbgc_instance_object_property_holder;

#define cast_fbgc_object_as_class(x)(((struct fbgc_class_object*) x))
#define sizeof_fbgc_class_object()(sizeof(struct fbgc_class_object)) 

#define cast_fbgc_object_as_instance(x)(((struct fbgc_instance_object*) x))

void print_fbgc_class_object(struct fbgc_object *);
void fprintf_fbgc_class_object(struct fbgc_object *);

struct fbgc_object * new_fbgc_class_object();
void inherit_from_another_class(struct fbgc_object * self,struct fbgc_object * father);
struct fbgc_object * get_set_fbgc_class_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm);


struct fbgc_object * new_fbgc_instance_object(struct fbgc_object * _template);
struct fbgc_object * get_set_fbgc_instance_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm);
struct fbgc_object ** get_set_fbgc_instance_object_member_address(struct fbgc_object * o, const char * str);
struct fbgc_object * operator_fbgc_instance_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);
struct fbgc_object * get_overloaded_member(struct fbgc_object * o, const char * str);

#ifdef LOG_CLASS_OBJECT
#define CLASS_OBJECT_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define CLASS_OBJECT_LOGD(format,...) LOGD(format,##__VA_ARGS__) 
#else
#define CLASS_OBJECT_LOGV(...)
#define CLASS_OBJECT_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif