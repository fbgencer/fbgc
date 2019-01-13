#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH

struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};

struct fbgc_object * new_fbgc_object(fbgc_token);
void print_fbgc_object(struct fbgc_object *);
void free_fbgc_object(struct fbgc_object *);



#endif