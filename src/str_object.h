#ifndef FBGC_STR_OBJECTH
#define FBGC_STR_OBJECTH

struct fbgc_str_object{
    struct fbgc_object base;
    char * content;
};
#define cast_fbgc_object_as_str(x)(((struct fbgc_str_object*) x))

struct fbgc_object * new_fbgc_str_object(const char *str);
struct fbgc_object * new_fbgc_str_object_from_substr(const char *str1,const char *str2);

void print_fbgc_str_object(struct fbgc_object *);
void free_fbgc_str_object(struct fbgc_object * );




#endif
