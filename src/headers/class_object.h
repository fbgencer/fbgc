#ifndef CLASS_OBJECT_H
#define CLASS_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_class_object{ 
	struct fbgc_object base;
	struct fbgc_ll_base * code; //class linked list code
	struct fbgc_object * locals; // Local array
};


struct fbgc_instance_object{
	struct fbgc_object base;
	struct fbgc_object * template_class;
	struct fbgc_object * variables;
};

#define cast_fbgc_object_as_class(x)(((struct fbgc_class_object*) x))
#define sizeof_fbgc_class_object()(sizeof(struct fbgc_class_object)) 

#define cast_fbgc_object_as_instance(x)(((struct fbgc_instance_object*) x))

void print_fbgc_class_object(struct fbgc_object *);
void fprintf_fbgc_class_object(struct fbgc_object *);

struct fbgc_object * new_fbgc_class_object();
struct fbgc_object * new_fbgc_instance_object(struct fbgc_object * template);
struct fbgc_object * get_set_fbgc_instance_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm);

#ifdef  __cplusplus
}
#endif

#endif