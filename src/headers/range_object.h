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
struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * robj,int index,struct fbgc_object * result);
struct fbgc_object * get_int_element_in_fbgc_range_object(struct fbgc_object * robj,int index,struct fbgc_object * result);
struct fbgc_object * get_double_element_in_fbgc_range_object(struct fbgc_object * robj,int index,struct fbgc_object * result);

#define get_fbgc_range_object_iter_type(x)\
(MAX(MAX(get_fbgc_object_type(cast_fbgc_object_as_range(x)->start),get_fbgc_object_type(cast_fbgc_object_as_range(x)->step)),get_fbgc_object_type(cast_fbgc_object_as_range(x)->end))) 

void print_fbgc_range_object(struct fbgc_object * obj);

#ifdef  __cplusplus
}
#endif

#endif