#include "fbgc.h"

struct
fbgc_object * new_fbgc_str_object(const char * str_content){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc(size_fbgc_str_object + strlen(str_content)+1); 
    stro->base.type = STRING;
    stro->base.next = NULL;
    stro->len = strlen(str_content);
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str_content,stro->len);
    *(&stro->content+stro->len) = '\0';
    return (struct fbgc_object*) stro;  

}


struct fbgc_object * new_fbgc_str_object_from_substr(const char * str1,const char * str2){
    
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc(size_fbgc_str_object + str2-str1 + 1); 
    stro->base.type = STRING;
    stro->base.next = NULL;
    stro->len = str2-str1;
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    return (struct fbgc_object*) stro;  
}


void print_fbgc_str_object(struct fbgc_object * obj){
    const char * s = &cast_fbgc_object_as_str(obj)->content;
    cprintf(011,"'%s'",s);   
}

void free_fbgc_str_object(struct fbgc_object * obj){
    //free(cast_fbgc_object_as_str(obj)->content);
    //free(obj);
}


//##########################################[C-STRINGS]###################################

struct
fbgc_object * new_fbgc_cstr_object(const char * str_content){

    size_t str_len = strlen(str_content);

    struct fbgc_cstr_object *stro =  (struct fbgc_cstr_object*) fbgc_malloc(size_fbgc_cstr_object + str_len + 1); 
    stro->base.type = CSTRING;
    stro->base.next = NULL;
    stro->content = ( (char *) stro ) + size_fbgc_cstr_object;
    memcpy(&stro->content,str_content,str_len);
    *(&stro->content+str_len) = '\0';
    return (struct fbgc_object*) stro;  

}


struct fbgc_object * new_fbgc_cstr_object_from_substr(const char * str1,const char * str2){
    
    size_t str_len = str2-str1;
    struct fbgc_cstr_object *stro =  (struct fbgc_cstr_object*) fbgc_malloc(size_fbgc_cstr_object + str_len + 1); 
    stro->base.type = CSTRING;
    stro->base.next = NULL;
    stro->content = ( (char *) stro ) + size_fbgc_cstr_object;
    memcpy(&stro->content,str1,str_len);
    *(&stro->content+str_len) = '\0';
    return (struct fbgc_object*) stro;  
}


void print_fbgc_cstr_object(struct fbgc_object * obj){
   // const char * s = &cast_fbgc_object_as_cstr(obj)->content;
    cprintf(011,"\"%s\"", &cast_fbgc_object_as_cstr(obj)->content);   
}

void free_fbgc_cstr_object(struct fbgc_object * obj){
    //free(cast_fbgc_object_as_str(obj)->content);
    //free(obj);
}
