#include "fbgc.h"

struct fbgc_object * new_fbgc_str_object(const char * str_content){
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

struct fbgc_object * new_fbgc_str_object_empty(int len){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc(size_fbgc_str_object + len + 1); 
    stro->base.type = STRING;
    stro->len = len;
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    *(&stro->content+stro->len) = '\0';
    return (struct fbgc_object*) stro;  
}

struct fbgc_object * binary_op_fbgc_str_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 

    const char * a1 = &cast_fbgc_object_as_str(a)->content;
    const char * b1 = &cast_fbgc_object_as_str(b)->content; 

    switch(op)
    {
       /* case STARSTAR:
        {
            c = pow(a1,b1);
            break;
        }
        case SLASHSLASH:
        {
            c =  1/(1/a1 + 1/b1); 
            break;
        }
        case LO_EQ:
        {
            c = a1<b1;
            break;
        }
        case GR_EQ:
        {
            c = a1 > b1;
            break;
        }
        case EQ_EQ:
        {
            c = a1 == b1;
            break;
        }
        case NOT_EQ:
        {
            c = a1 != b1;
            break;
        }
        case R_SHIFT:
        {
            c = a1>>b1;
            break;
        }
        case L_SHIFT:
        {
            c = a1<<b1;
            break;
        }
        case CARET:
        {
            c = pow(a1,b1);
            break;
        }
        case PERCENT:
        {
            c = a1%b1;
            break;
        }        
        case LOWER:
        {
            c = a1<b1;
            break;
        }        
        case GREATER:
        {
            c = a1>b1;
            break;
        }        
        case PIPE:
        {
            c = a1 || b1;
            break;
        } 
        case AMPERSAND:
        {
            c = a1 && b1;
            break;
        }        
        case SLASH:
        {
            assert(b1 != 0);
            c = a1/b1;

            break;
        }        
        case STAR:
        {
            c = a1*b1;
            break;
        }        
        case MINUS:
        {
            c = a1-b1;
            break;
        }        */
        case PLUS:
        {
           
            struct fbgc_object * o = new_fbgc_str_object_empty(length_fbgc_str_object(a)+length_fbgc_str_object(b));
            char * c = &cast_fbgc_object_as_str(o)->content;
            memcpy(c,a1,length_fbgc_str_object(a));
            memcpy(c+length_fbgc_str_object(a) ,b1,length_fbgc_str_object(b));
            //strcat(c,b1);

            return o;
            //
            //c = a1+b1;
            //break;
        } 
    }
    return new_fbgc_str_object("LOL");
}



void print_fbgc_str_object(struct fbgc_object * obj){
    const char * s = &cast_fbgc_object_as_str(obj)->content;
    cprintf(011,"'%s'",s);   
}

void free_fbgc_str_object(struct fbgc_object * obj){
    //free(cast_fbgc_object_as_str(obj)->content);
    //free(obj);
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$     CSTRING   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//###############################################################################

uint8_t my_strncmp(const char *s1, const char *s2, register size_t n)
{
    register unsigned char u1, u2;
    while (n-- > 0){

        u1 = (unsigned char) *s1++;
        u2 = (unsigned char) *s2++;
        if (u1 != u2 || u1 == '\0')
            return 1;
    }
    return 0;
}

uint8_t my_strcmp(const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}


//##########################################[C-STRINGS]###################################

struct
fbgc_object * new_fbgc_cstr_object(const char * str_content){

    size_t str_len = strlen(str_content);

    struct fbgc_cstr_object *stro =  (struct fbgc_cstr_object*) fbgc_malloc(size_fbgc_cstr_object + str_len + 1); 
    stro->base.type = CSTRING;
    stro->base.next = NULL;
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
