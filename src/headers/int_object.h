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
struct fbgc_object * derive_from_new_int_object(fbgc_token type, int int_content);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(const char *,const char *,fbgc_token );

/*
struct fbgc_object * plus_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);
struct fbgc_object * minus_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);
struct fbgc_object * star_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);
struct fbgc_object * slash_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b);*/



struct fbgc_object * binary_op_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

void print_fbgc_int_object(struct fbgc_object *);
void free_fbgc_int_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif