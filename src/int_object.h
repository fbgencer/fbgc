#ifndef FBGC_INT_OBJECTH
#define FBGC_INT_OBJECTH


struct fbgc_int_object{
    struct fbgc_object base;
    int content;
};

struct fbgc_object * new_fbgc_int_object(int);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(char *,char *);

void print_fbgc_int_object(struct fbgc_object *);
void free_fbgc_int_object(struct fbgc_object * );




#endif