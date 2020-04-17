#include "fbgc.h"

struct fbgc_object *  new_fbgc_range_object(struct fbgc_object * s,struct fbgc_object * e){ 
    struct fbgc_range_object * r;
    if(s->type != RANGE){
        r =  (struct fbgc_range_object*) fbgc_malloc(sizeof_fbgc_range_object());
        r->base.type = RANGE;
        r->start = s;
        r->end = e;  

        fbgc_token tok = MAX(get_fbgc_object_type(r->start),get_fbgc_object_type(r->end));    
     
        if(tok == INT){
            //if tok is int, both of them is int (we are sure check tokens.h)
            int stp = cast_fbgc_object_as_int(s)->content > cast_fbgc_object_as_int(e)->content ? -1 : 1; 
            r->step = new_fbgc_int_object(stp);
            
        }else if(tok == DOUBLE){
            double stp = cast_fbgc_object_as_double(s)->content > cast_fbgc_object_as_double(e)->content ? -1.0 : 1.0;
            r->step = new_fbgc_double_object(stp);
        }
    }
    else{
        r = cast_fbgc_object_as_range(s);
        fbgc_token tok = MAX(get_fbgc_object_type(r->start), get_fbgc_object_type(r->end));
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
