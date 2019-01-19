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
  	return new_fbgc_int_object(strtol(int_str,NULL,10));
}

struct
fbgc_object * new_fbgc_int_object_from_substr(const char * int_str_begin,const char * int_str_end,fbgc_token base){
	//now we don't wanna check inf or overlfow issue but later we are going to check them
  	if(base == INT) base = 10;
    else if(base == HEX) base = 16;
    else if(base == BIN) base = 2;
    return new_fbgc_int_object(strtol(int_str_begin, &int_str_end,base));
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
