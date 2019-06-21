#ifndef FBGC_INT_OBJECTH
#define FBGC_INT_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_int_object{
    struct fbgc_object base;
    int content;
};

#define cast_fbgc_object_as_int(x)(((struct fbgc_int_object*) x))
#define size_fbgc_int_object (sizeof(struct fbgc_int_object))


struct fbgc_object * new_fbgc_int_object(int);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(const char *,const char *,fbgc_token );

struct
fbgc_object * add_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);
struct
fbgc_object * subtract_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);
struct
fbgc_object * multiply_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);
struct
fbgc_object * divide_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);


void print_fbgc_int_object(struct fbgc_object *);
void free_fbgc_int_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif