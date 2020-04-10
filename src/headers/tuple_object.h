#ifndef FBGC_TUPLE_OBJECTH
#define FBGC_TUPLE_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_tuple_object{
    struct fbgc_object base;
    size_t capacity;
    size_t size;
    struct fbgc_object * content[0];
};


#define cast_fbgc_object_as_tuple(x)(((struct fbgc_tuple_object*) x))
#define size_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->size)
#define capacity_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->capacity)

#define sizeof_fbgc_tuple_object(x)(sizeof(struct fbgc_tuple_object) + sizeof(struct fbgc_object*)*capacity_fbgc_tuple_object(x))


#define content_fbgc_tuple_object(x)(cast_fbgc_object_as_tuple(x)->content)

struct fbgc_object * new_fbgc_tuple_object(size_t size);
struct fbgc_object * new_fbgc_tuple_object_from_tuple_content(struct fbgc_object ** src, size_t len);

void set_object_in_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj,int index);
struct fbgc_object * get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index);
struct fbgc_object ** get_object_address_in_fbgc_tuple_object(struct fbgc_object * self,int index);
struct fbgc_object *  get_back_in_fbgc_tuple_object(struct fbgc_object * self);
struct fbgc_object *  get_front_in_fbgc_tuple_object(struct fbgc_object * self);
struct fbgc_object * push_back_fbgc_tuple_object(struct fbgc_object * self,struct fbgc_object * obj);
int index_fbgc_tuple_object(struct fbgc_object * self, struct fbgc_object * obj);



struct fbgc_object *  __get_object_in_fbgc_tuple_object(struct fbgc_object * self,int index,struct fbgc_object * res);

struct fbgc_object * operator_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

void print_fbgc_tuple_object(struct fbgc_object *);
void free_fbgc_tuple_object(struct fbgc_object * );


struct fbgc_object * add_fbgc_tuple_object(struct fbgc_object * a,struct fbgc_object * b);


/*! \cond DOCUMENT_EVERYTHING
	Lets not document this part, just log definitions nothing important 
*/
#ifdef LOG_TUPLE_OBJECT
#define TUPLE_OBJECT_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define TUPLE_OBJECT_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _TUPLE_OBJECT_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _TUPLE_OBJECT_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define TUPLE_OBJECT_LOGV(...)
#define TUPLE_OBJECT_LOGD(...)
#define _TUPLE_OBJECT_LOGV(...)
#define _TUPLE_OBJECT_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif
