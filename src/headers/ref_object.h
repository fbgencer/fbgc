#ifndef FBGC_REF_OBJECTH
#define FBGC_REF_OBJECTH

struct fbgc_ref_object{
    struct fbgc_object base;
   	struct fbgc_object * content;
};

#define cast_fbgc_object_as_ref(x) (((struct fbgc_ref_object*) x))

struct fbgc_object * new_fbgc_ref_object();
struct fbgc_object * assign_fbgc_ref_object(struct fbgc_object * ref, struct fbgc_object * obj);

void print_fbgc_ref_object(struct fbgc_object * ref);

void free_fbgc_ref_object(struct fbgc_object * refo);



#endif