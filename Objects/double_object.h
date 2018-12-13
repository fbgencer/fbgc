#ifndef FBGC_DOUBLE_OBJECTH
#define FBGC_DOUBLE_OBJECTH

#include "fbgc_object.h"

struct fbgc_double_object{
    struct fbgc_object base;
    double *data;
    uint8_t size;  
};

struct
fbgc_object * add_fbgc_double_object(struct fbgc_object *, double inc_data);

void print_fbgc_double_object(struct fbgc_object *);
void free_fbgc_double_object(struct fbgc_object * );





#endif