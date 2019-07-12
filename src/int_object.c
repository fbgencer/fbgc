#include "fbgc.h"


struct
fbgc_object * new_fbgc_int_object(int int_content){
	struct fbgc_int_object *into =  (struct fbgc_int_object*) fbgc_malloc(size_fbgc_int_object);
    into->base.type = INT;
    into->base.next = NULL;
    into->content = int_content; 
    return (struct fbgc_object*) into;
}

struct
fbgc_object * new_fbgc_int_object_from_str(const char * int_str){

  	return new_fbgc_int_object(strtol(int_str,NULL,10));
}

struct
fbgc_object * new_fbgc_int_object_from_substr(const char * int_str_begin,const char * int_str_end,fbgc_token base){
	//now we don't wanna check inf or overlfow issue but later we are going to check them

    if(base == INT) base = 10;
    else if(base == INT16) {base = 16; int_str_begin+=2;}  //eat 0x
    else if(base == INT2) {base = 2; int_str_begin+=2;} //eat 0b
    return new_fbgc_int_object(strtol(int_str_begin, NULL,base));
    //&int_str_end
}

struct
fbgc_object * add_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = convert_fbgc_object_to_int(b);
    
    return new_fbgc_int_object(a1+b1);
}

struct
fbgc_object * subtract_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = convert_fbgc_object_to_int(b);
    
    return new_fbgc_int_object(a1-b1);
}

struct
fbgc_object * multiply_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = convert_fbgc_object_to_int(b);
    
    return new_fbgc_int_object(a1 * b1);
}

struct
fbgc_object * divide_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = convert_fbgc_object_to_int(b);
    
    //if b1 is 0 check before!!!!

    return new_fbgc_int_object(a1 / b1);
}

void print_fbgc_int_object(struct fbgc_object * obj){
    cprintf(011,"%d",cast_fbgc_object_as_int(obj)->content);  
}

void free_fbgc_int_object(struct fbgc_object * obj){
    //   free(obj);
}
