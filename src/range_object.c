#include "fbgc.h"

struct fbgc_object *  new_fbgc_range_object(struct fbgc_object * s,struct fbgc_object * e){ 
    struct fbgc_range_object * r =  (struct fbgc_range_object*) fbgc_malloc(sizeof_fbgc_range_object());
    r->base.type = RANGE;
    r->start = s;
    r->end = e;
    r->step = NULL;
    return (struct fbgc_object*) r;
}


struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * robj, int index){

    #define r cast_fbgc_object_as_range(robj)

    fbgc_token tok = MAX(get_fbgc_object_type(r->start),get_fbgc_object_type(r->end));
    if(r->step != NULL) tok = MAX(get_fbgc_object_type(r->step),tok);

    if(tok == INT){
        //if tok is int we are absolutely sure that each element of range object is integer..
        int step = (r->step == NULL) ? 1 : cast_fbgc_object_as_int(r->step)->content; 
        int no = cast_fbgc_object_as_int(r->start)->content + (step*index);

        if(step>0)
            return  (no >= cast_fbgc_object_as_int(r->end)->content ) ? NULL :
            new_fbgc_int_object(no);
        else
            return  (no > cast_fbgc_object_as_int(r->end)->content ) ?  new_fbgc_int_object(no) : NULL ;
    }
    else if(tok == DOUBLE){
        double step = (r->step == NULL) ? 1.0 : convert_fbgc_object_to_double(r->step); 
        double no = convert_fbgc_object_to_double(r->start) + (step*index);

        if(step>0)
            return  (no >= convert_fbgc_object_to_double(r->end) ) ? NULL:
            new_fbgc_double_object(no);
        else
            return  (no > convert_fbgc_object_to_double(r->end)  ) ?  new_fbgc_double_object(no) : NULL ;        
    }
    #undef r
} 

/*
struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * obj, int index){

    #define r cast_fbgc_object_as_range(obj)

    if(r->start->type  == INT){
        int step = (r->step == NULL) ? 1 : cast_fbgc_object_as_int(r->step)->content; 
        int no = cast_fbgc_object_as_int(r->start)->content + (step*index);

        if(step>0)
            return  (no >= cast_fbgc_object_as_int(r->end)->content ) ? NULL : new_fbgc_int_object( no );
        else
             return  (no > cast_fbgc_object_as_int(r->end)->content ) ?  new_fbgc_int_object( no ) : NULL ;
    }
    #undef r
} 
*/

void print_fbgc_range_object(struct fbgc_object * obj){

   #define r cast_fbgc_object_as_range(obj)
    cprintf(111,"<");
    print_fbgc_object(r->start);
    cprintf(111,":");
    if(r->step != NULL) print_fbgc_object(r->step);
    cprintf(111,":");
    print_fbgc_object(r->end);
    cprintf(111,">");
    #undef r
     
}
