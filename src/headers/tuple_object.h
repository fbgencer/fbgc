#ifndef FBGC_TUPLE_OBJECTH
#define FBGC_TUPLE_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_tuple_object{
    struct fbgc_object base;
    size_t capacity;
    size_t size;
};


#define cast_fbgc_object_as_tuple(x)(((struct fbgc_tuple_object*) x))

#define size_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->size)
#define capacity_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->capacity)



#define tuple_object_content(x)((struct fbgc_object **)((char*) &cast_fbgc_object_as_tuple(x)->size+sizeof(cast_fbgc_object_as_tuple(x)->size)));

struct fbgc_object * new_fbgc_tuple_object(size_t size);
struct fbgc_object * new_fbgc_tuple_object_from_tuple_content(struct fbgc_object ** src, int num);

void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index);
struct fbgc_object * get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index);
struct fbgc_object *  get_top_in_fbgc_tuple_object(struct fbgc_object * self);
struct fbgc_object * push_back_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj);
int index_fbgc_tuple_object(struct fbgc_object * self, struct fbgc_object * obj);

void print_fbgc_tuple_object(struct fbgc_object *);
void free_fbgc_tuple_object(struct fbgc_object * );


struct fbgc_object * add_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b);


#ifdef  __cplusplus
}
#endif

#endif
