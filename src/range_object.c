// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"

struct fbgc_object *  new_fbgc_range_object(struct fbgc_object * s,struct fbgc_object * e){ 
    struct fbgc_range_object * r;
    fbgc_token tok = MAX(get_fbgc_object_type(s), get_fbgc_object_type(e));

    if(s->type != RANGE){
        r =  (struct fbgc_range_object*) fbgc_malloc_object(sizeof_fbgc_range_object());
        r->base.type = RANGE;
        r->start = s;
        r->end = e;  
     
        if(tok == INT){
            //if tok is int, both of them is int (we are sure check tokens.h)
            int stp = cast_fbgc_object_as_int(s)->content > cast_fbgc_object_as_int(e)->content ? -1 : 1; 
            r->step = new_fbgc_int_object(stp);
            
        }else if(tok == DOUBLE){
            double stp = cast_fbgc_object_as_double(s)->content > cast_fbgc_object_as_double(e)->content ? -1.0 : 1.0;
            r->step = new_fbgc_double_object(stp);
        }
        else r->step = NULL;
    }
    else{
        r = cast_fbgc_object_as_range(s);
        
        tok = MAX(get_fbgc_object_type(e),tok);

        //if tok is int we are absolutely sure that each element of range object is integer..
        /*cprintf(100,"tok is double %d\n",tok == DOUBLE);

        if(tok == DOUBLE){
            if(get_fbgc_object_type(r->start) == INT) r->start = new_fbgc_double_object(cast_fbgc_object_as_int(r->start)->content);
            if(get_fbgc_object_type(r->start) == INT)
        }*/

        r->step = r->end;
        r->end = e;

    }   

    return (struct fbgc_object*) r;
}


struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * robj, int index, struct fbgc_object * result){

    fbgc_token range_type = get_fbgc_range_object_iter_type(robj);

    if(range_type == INT) return get_int_element_in_fbgc_range_object(robj,index,result);
    else if(range_type == DOUBLE) return get_double_element_in_fbgc_range_object(robj,index,result);
    
    return NULL;
} 


struct fbgc_object * get_int_element_in_fbgc_range_object(struct fbgc_object * robj, int index, struct fbgc_object * result){

    #define r cast_fbgc_object_as_range(robj)

    int step = cast_fbgc_object_as_int(r->step)->content;
    int no = cast_fbgc_object_as_int(r->start)->content + (step*index);
    cast_fbgc_object_as_int(result)->content = no;

    if(step > 0)
        return  (no < cast_fbgc_object_as_int(r->end)->content ) ? result : NULL;
    else 
        return  (no > cast_fbgc_object_as_int(r->end)->content ) ? result : NULL;

    #undef r
} 


struct fbgc_object * get_double_element_in_fbgc_range_object(struct fbgc_object * robj, int index, struct fbgc_object * result){

    #define r cast_fbgc_object_as_range(robj)
    
    double step = convert_fbgc_object_to_double(r->step);
    double no = convert_fbgc_object_to_double(r->start) + (step*index);
    cast_fbgc_object_as_double(result)->content = no;

    //We need a tolerance value here, 
    if(step>0)
        return  (no < convert_fbgc_object_to_double(r->end) ) ? result : NULL;
    else 
        return  (no > convert_fbgc_object_to_double(r->end) ) ? result : NULL;
    #undef r
} 

// struct fbgc_object * get_element_in_fbgc_range_object(struct fbgc_object * robj, int index){

//     #define r cast_fbgc_object_as_range(robj)
//     //fbgc_token tok = MAX(get_fbgc_object_type(r->start),get_fbgc_object_type(r->end));
//     //if(r->step != NULL) tok = MAX(get_fbgc_object_type(r->step),tok);

//     //if(tok == INT){
//         //int step = (r->step == NULL) ? 1 : cast_fbgc_object_as_int(r->step)->content; 
//         //if(r->step == NULL && cast_fbgc_object_as_int(r->start)->content > cast_fbgc_object_as_int(r->end)->content) step = -1;
        
//         int no = cast_fbgc_object_as_int(r->start)->content + (cast_fbgc_object_as_int(r->step)->content*index);
//         cprintf(100,"index = %d, no = %d\n",index,no);
//         //return  (no >= cast_fbgc_object_as_int(r->end)->content ) ? NULL : new_fbgc_int_object(no);

//         if(cast_fbgc_object_as_int(r->step)->content > 0)
//            return  (no < cast_fbgc_object_as_int(r->end)->content ) ? new_fbgc_int_object(no) : NULL;
//         else
//            return  (no > cast_fbgc_object_as_int(r->end)->content ) ? new_fbgc_int_object(no) : NULL;
//     //}*/

//     /*else if(tok == DOUBLE){
//         double step = (r->step == NULL) ? 1.0 : convert_fbgc_object_to_double(r->step); 
//         double no = convert_fbgc_object_to_double(r->start) + (step*index);

//         if(step>0)
//             return  (no >= convert_fbgc_object_to_double(r->end) ) ? NULL:
//             new_fbgc_double_object(no);
//         else
//             return  (no > convert_fbgc_object_to_double(r->end)  ) ?  new_fbgc_double_object(no) : NULL ;        
//     }*/
//     #undef r
// } 

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

uint8_t print_fbgc_range_object(struct fbgc_object * self){
    printf_fbgc_object(cast_fbgc_object_as_range(self)->start);
    printf(":");
    if(cast_fbgc_object_as_range(self)->step != NULL){
        printf_fbgc_object(cast_fbgc_object_as_range(self)->step);
        printf(":");
    }
    printf_fbgc_object(cast_fbgc_object_as_range(self)->end); 
    return 1;
}


static inline size_t size_of_fbgc_range_object(struct fbgc_object * self){
    return sizeof(struct fbgc_range_object);
}

static inline void gc_mark_fbgc_range_object(struct fbgc_object * self){
    fbgc_gc_mark_fbgc_object(cast_fbgc_object_as_range(self)->start);
    fbgc_gc_mark_fbgc_object(cast_fbgc_object_as_range(self)->step);
    fbgc_gc_mark_fbgc_object(cast_fbgc_object_as_range(self)->end);
}


const struct fbgc_object_property_holder fbgc_range_object_property_holder = {
    .bits = 
    _BIT_PRINT |
    _BIT_SIZE_OF |
    _BIT_GC_MARK
    ,
    .properties ={
        {.print = &print_fbgc_range_object},
        {.gc_mark = &gc_mark_fbgc_range_object},
        {.size_of = &size_of_fbgc_range_object},
        
    }
};
