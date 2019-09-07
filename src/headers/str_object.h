#ifndef FBGC_STR_OBJECTH
#define FBGC_STR_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_str_object{
    struct fbgc_object base;
    size_t len;
    char content;
};


#define cast_fbgc_object_as_str(x)(((struct fbgc_str_object*) x))
#define size_fbgc_str_object (sizeof(struct fbgc_str_object))

struct fbgc_object * new_fbgc_str_object(const char *str);
struct fbgc_object * new_fbgc_str_object_from_substr(const char *str1,const char *str2);

void print_fbgc_str_object(struct fbgc_object *);
void free_fbgc_str_object(struct fbgc_object * );

uint8_t my_strncmp(const char *s1, const char *s2, register size_t n);
uint8_t my_strcmp(const char *p1, const char *p2);

struct fbgc_cstr_object{
    struct fbgc_object base;
    char content; 
};

#define cast_fbgc_object_as_cstr(x)(((struct fbgc_cstr_object*) x))
#define size_fbgc_cstr_object (sizeof(struct fbgc_object))

struct fbgc_object * new_fbgc_cstr_object(const char *str);
struct fbgc_object * new_fbgc_cstr_object_from_substr(const char *str1,const char *str2);

#define length_fbgc_cstr_object(x)(strlen(&cast_fbgc_object_as_cstr(x)->content))
#define content_fbgc_cstr_object(x)(&cast_fbgc_object_as_cstr(x)->content)

void print_fbgc_cstr_object(struct fbgc_object *);
void free_fbgc_cstr_object(struct fbgc_object * );





#ifdef  __cplusplus
}
#endif

#endif
