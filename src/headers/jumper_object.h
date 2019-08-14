#ifndef FBGC_JUMPER_OBJECTH
#define FBGC_JUMPER_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_jumper_object{
    struct fbgc_object base;
   	struct fbgc_object * content;
};

#define cast_fbgc_object_as_jumper(x) (((struct fbgc_jumper_object*) x))

struct fbgc_object * new_fbgc_jumper_object(fbgc_token tok);

void print_fbgc_jumper_object(struct fbgc_object * jumper);

void free_fbgc_jumper_object(struct fbgc_object * jumpero);

#ifdef  __cplusplus
}
#endif

#endif