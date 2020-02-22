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
#define sizeof_fbgc_int_object(x)(sizeof(struct fbgc_int_object))


struct fbgc_object * new_fbgc_int_object(int);
struct fbgc_object * derive_from_new_int_object(fbgc_token type, int int_content);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(const char *,const char *,fbgc_token );

struct fbgc_object * operator_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);


struct fbgc_object * operator_fbgc_int_object2(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

void print_fbgc_int_object(struct fbgc_object *);
void free_fbgc_int_object(struct fbgc_object * );

struct fbgc_object * return_fbgc_object_operator_helper_int(int c,struct fbgc_object * result);
struct fbgc_object * return_fbgc_object_operator_helper_double(double c,struct fbgc_object * result);
struct fbgc_object * return_fbgc_object_operator_helper_logic(char c,struct fbgc_object * result);




#ifdef  __cplusplus
}
#endif

#endif