#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH

struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};

#define get_fbgc_object_type(obj)( obj != NULL ? obj->type : UNKNOWN)


struct fbgc_object * new_fbgc_object(fbgc_token);
struct fbgc_object * new_fbgc_object_from_str(const char *str1,fbgc_token token);
struct fbgc_object * new_fbgc_object_from_substr(struct fbgc_object * field,const char *str1,const char*str2, fbgc_token token);
void print_fbgc_object(struct fbgc_object *);
void free_fbgc_object(struct fbgc_object *);



#endif