#include "fbgc.h"


struct
fbgc_object * new_fbgc_int_object(int int_content){
	struct fbgc_int_object *into =  (struct fbgc_int_object*) malloc(sizeof(struct fbgc_int_object));
    into->base.type = INT;
    into->base.next = NULL;
    into->content = int_content; 
    return (struct fbgc_object*) into;
}

struct
fbgc_object * new_fbgc_int_object_from_str(const char * int_str){
  	return new_fbgc_int_object(strtod (int_str,NULL));
}

struct
fbgc_object * new_fbgc_int_object_from_substr(char * int_str_begin,char * int_str_end){
	//now we don't wanna check inf or overlfow issue but later we are going to check them
  	return new_fbgc_int_object(strtod (int_str_begin, &int_str_end));
}


void print_fbgc_int_object(struct fbgc_object * obj){
    struct fbgc_int_object * into = (struct fbgc_int_object*) obj;
    printf("\033[1;31m[Int]\033[0m :{%d}\n",into->content);   
}

void free_fbgc_int_object(struct fbgc_object * obj){
    struct fbgc_int_object *into = (struct fbgc_int_object*) obj;
    #ifdef DEBUG
    	//fprintf(fbgc_log_file,"int Deleting..");
    #endif
    free(into);
}
