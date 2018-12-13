#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH


struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};

void free_fbgc_object(struct fbgc_object *);
#define new_fbgc_object(x)((struct fbgc_object *) malloc(sizeof(fbgc_object)*x))




#endif