#include "fbgc.h"

struct
fbgc_object * new_fbgc_str_object(const char * str_content){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) malloc(sizeof(struct fbgc_str_object));
    stro->base.type = STRING;
    stro->base.next = NULL;
    stro->content = (char *)(malloc(sizeof( char )*(strlen(str_content)+1))); 
    memcpy(stro->content,str_content,strlen(str_content)+1);
    return (struct fbgc_object*) stro;
}


struct fbgc_object * new_fbgc_str_object_from_substr(const char * str1,const char * str2){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) malloc(sizeof(struct fbgc_str_object));
    stro->base.type = STRING;
    stro->base.next = NULL;
    stro->content = (char *)(malloc(sizeof( char )*(str2-str1+1))); 
    memcpy(stro->content,str1,(str2-str1+1));
    return (struct fbgc_object*) stro;    
}


void print_fbgc_str_object(struct fbgc_object * obj){
    struct fbgc_str_object * stro = (struct fbgc_str_object*) obj;
    printf("\033[1;31m[String]\033[0m :{%s}\n",stro->content);   
}

void free_fbgc_str_object(struct fbgc_object * obj){
    struct fbgc_str_object *stro = cast_fbgc_object_as_str(obj);
    free(stro->content);
    free(stro);
}
