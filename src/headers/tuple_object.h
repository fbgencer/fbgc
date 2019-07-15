#ifndef FBGC_TUPLE_OBJECTH
#define FBGC_TUPLE_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_tuple_object{
    struct fbgc_object base;
    size_t size;
};


#define cast_fbgc_object_as_tuple(x)(((struct fbgc_tuple_object*) x))


#define tuple_object_content(x)((struct fbgc_object **)((char*)&x->size+sizeof(x->size)));

struct fbgc_object * new_fbgc_tuple_object(int size);
void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index);
struct fbgc_object * get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index);

void print_fbgc_tuple_object(struct fbgc_object *);
void free_fbgc_tuple_object(struct fbgc_object * );


struct fbgc_object * add_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b);


#ifdef  __cplusplus
}
#endif

#endif
