#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH

struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};

struct fbgc_object * new_fbgc_object(fbgc_token);
struct fbgc_object * new_fbgc_object_from_str(char *str1,fbgc_token token);
struct fbgc_object * new_fbgc_object_from_substr(char *str1,char*str2, fbgc_token token);
void print_fbgc_object(struct fbgc_object *);
void free_fbgc_object(struct fbgc_object *);



#endif