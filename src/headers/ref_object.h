#ifndef FBGC_REF_OBJECTH
#define FBGC_REF_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_ref_object{
    struct fbgc_object base;
   	struct fbgc_object * content;
};

#define cast_fbgc_object_as_ref(x) (((struct fbgc_ref_object*) x))

//o must be reference object, check before using it!
#define abandon_ownership(ref)( (cast_fbgc_object_as_ref(ref)->content->next != NULL) ?  cast_fbgc_object_as_ref(ref)->content->next->type &= 0x7F : 0)

struct fbgc_object * new_fbgc_ref_object();
struct fbgc_object * initialize_fbgc_ref_object(struct fbgc_object * ref, struct fbgc_object * obj);
void assign_var_to_fbgc_ref_object(struct fbgc_object * ref, struct fbgc_object * obj);
struct fbgc_object * get_var_from_fbgc_ref_object(struct fbgc_object *ref);

void print_fbgc_ref_object(struct fbgc_object * ref);

void free_fbgc_ref_object(struct fbgc_object * refo);

#ifdef  __cplusplus
}
#endif

#endif