#ifndef FBGC_ARRAY_OBJECTH
#define FBGC_ARRAY_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif


// Do not change order of the objects in the struct
// last object is always size_t size!


struct fbgc_array_object{
    struct fbgc_object base;
    size_t block_size; // individual block size
    size_t capacity; // capacity that we can hold
    size_t size; // actual size of the array
};


#define cast_fbgc_object_as_array(x)(((struct fbgc_array_object*) x))

#define size_fbgc_array_object(x)(cast_fbgc_object_as_array(x)->size)
#define capacity_fbgc_array_object(x)(cast_fbgc_object_as_array(x)->capacity)
#define block_size_fbgc_array_object(x)(cast_fbgc_object_as_array(x)->block_size)

#define sizeof_fbgc_array_object(x) (sizeof(struct fbgc_array_object)+(capacity_fbgc_array_object(obj) * block_size_fbgc_array_object(obj) ) )


#define array_object_content(x)((unsigned char **)( (unsigned char*) &x->type + sizeof(struct fbgc_array_object)))


#define array_object_at(ar,in)((char*) array_object_content(ar) + in * block_size_fbgc_array_object(ar))




struct fbgc_object * new_fbgc_array_object(size_t cap, size_t b_size);
void set_in_fbgc_array_object(struct fbgc_object * self,void * obj,int index);
void * get_address_in_fbgc_array_object(struct fbgc_object * self,int index);
void * get_content_in_fbgc_array_object(struct fbgc_object * self,int index);
void *  get_top_in_fbgc_array_object(struct fbgc_object * self);
struct fbgc_object * push_back_fbgc_array_object(struct fbgc_object * self,void * obj);
int index_fbgc_array_object(struct fbgc_object * self, void * obj);

 

void print_fbgc_array_object(struct fbgc_object *);
void free_fbgc_array_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif
