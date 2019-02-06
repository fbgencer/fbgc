#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};

#define get_fbgc_object_type(obj)( obj != NULL ? obj->type : UNKNOWN)


struct fbgc_object * new_fbgc_object(fbgc_token);
struct fbgc_object * new_fbgc_object_from_substr(struct fbgc_object * field,const char *str1,const  char*str2, fbgc_token token);


int convert_fbgc_object_to_int(struct fbgc_object * );
double convert_fbgc_object_to_double(struct fbgc_object * );
char * convert_fbgc_object_to_string(struct fbgc_object * );

void print_fbgc_object(struct fbgc_object *);
void free_fbgc_object(struct fbgc_object *);

#ifdef  __cplusplus
}
#endif

#endif