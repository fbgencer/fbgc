#include "fbgc.h"

struct
fbgc_object * new_fbgc_double_object(double db_content){
	struct fbgc_double_object *dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = db_content; 
    return (struct fbgc_object*) dbo;
}

struct
fbgc_object * new_fbgc_double_object_from_str(const char * db_str){
  	return new_fbgc_double_object(strtod (db_str,NULL));
}

struct
fbgc_object * new_fbgc_double_object_from_substr(const char * db_str_begin,const char * db_str_end){
	//now we don't wanna check inf or overlfow issue but later we are going to check them
  	return new_fbgc_double_object(strtod (db_str_begin, &db_str_end));
}

struct
fbgc_object * add_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    return new_fbgc_double_object(a1+b1);
}

struct
fbgc_object * subtract_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    return new_fbgc_double_object(a1-b1);
}

struct
fbgc_object * multiply_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    return new_fbgc_double_object(a1 * b1);
}

struct
fbgc_object * divide_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    //if b1 is 0 check before!!!!

    return new_fbgc_double_object(a1 / b1);
}


void print_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object * dbo = (struct fbgc_double_object*) obj;
    printf("\033[1;31m[Double]\033[0m :{%f}\n",dbo->content);   
}

void free_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object *dbo = (struct fbgc_double_object*) obj;
    #ifdef DEBUG
    	//fprintf(fbgc_log_file,"Double Deleting..");
    #endif
    free(dbo);
}
