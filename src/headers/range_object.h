#ifndef RANGE_OBJECT_H
#define RANGE_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_range_object{ 
	struct fbgc_object base;
	struct fbgc_object * start;
	struct fbgc_object * end;
	struct fbgc_object * step;
};

#define cast_fbgc_object_as_range(x)(((struct fbgc_range_object*) x))
#define sizeof_fbgc_range_object() (sizeof(struct fbgc_range_object)) 



struct fbgc_object * new_fbgc_range_object(struct fbgc_object * s,struct fbgc_object * e);
struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * robj,int index);
fbgc_token get_fbgc_range_object_iter_type(struct fbgc_object * robj);

void print_fbgc_range_object(struct fbgc_object * obj);

#ifdef  __cplusplus
}
#endif

#endif