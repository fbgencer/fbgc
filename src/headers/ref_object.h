#ifndef FBGC_REF_OBJECTH
#define FBGC_REF_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_ref_object{
    struct fbgc_object base;
   	int * loc;
};

#define cast_fbgc_object_as_ref(x) (((struct fbgc_ref_object*) x))

struct fbgc_object * new_fbgc_ref_object(const int i);
struct fbgc_object * initialize_fbgc_ref_object(struct fbgc_object * ref, struct fbgc_object * obj);
struct fbgc_object * assign_var_to_fbgc_ref_object(struct fbgc_object * ref, struct fbgc_object * obj);
struct fbgc_object * get_var_from_fbgc_ref_object(struct fbgc_object *ref);

void print_fbgc_ref_object(struct fbgc_object * ref);

void free_fbgc_ref_object(struct fbgc_object * refo);

#ifdef  __cplusplus
}
#endif

#endif