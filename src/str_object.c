#include "fbgc.h"

struct fbgc_object * new_fbgc_str_object(const char * str_content){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc(sizeof(struct fbgc_str_object) + strlen(str_content) ); 
    stro->base.type = STRING;
    //stro->base.next = NULL;
    stro->len = strlen(str_content);
    //stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str_content,stro->len);
    *(&stro->content+stro->len) = '\0';
    return (struct fbgc_object*) stro;  
}

struct fbgc_object * new_fbgc_str_object_from_substr(const char * str1,const char * str2){
    
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc(sizeof(struct fbgc_str_object) + str2-str1); 
    stro->base.type = STRING;
    //stro->base.next = NULL;
    stro->len = str2-str1;
    //stro->content = ( (char *) stro ) + size_fbgc_str_object;
    //cprintf(111,"stro %p CC: %p content %p\n",stro,cc,&stro->content);
    //memcpy(cc,str1,stro->len);
    //*(cc+stro->len) = '\0';    
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    return (struct fbgc_object*) stro;  
}

struct fbgc_object * new_fbgc_str_object_empty(int len){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc(sizeof(struct fbgc_str_object) + len ); 
    stro->base.type = STRING;
    stro->len = len;
    //stro->content = ( (char *) stro ) + size_fbgc_str_object;
    *(&stro->content+stro->len) = '\0';
    return (struct fbgc_object*) stro;  
}

struct fbgc_object * binary_op_fbgc_str_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){


switch(op)
{
    case STARSTAR:
    {
        return NULL;
    }
    case SLASHSLASH:
    {
        return NULL;
    }
    case LO_EQ:
    {
        return NULL;
    }
    case GR_EQ:
    {
        return NULL;
    }
    case EQ_EQ:
    case NOT_EQ:
    {   
        //no need to check whether a or b is string, because one of them must be.
        if(a->type == b->type){
            uint8_t cmp = (length_fbgc_str_object(a) == length_fbgc_str_object(b) && 
                memcmp(content_fbgc_str_object(a),content_fbgc_str_object(b),length_fbgc_str_object(a)) == 0);

            return new_fbgc_logic_object( op == NOT_EQ ? !cmp : cmp );
        }   
        return NULL;        
    }
    case R_SHIFT:
    {   
        //a>>b
        //only rhs can be string, otherwise it's meaningless
        if(b->type == STRING){
            if(a->type == STRING) goto STRING_CONCAT;
            //Below codes are just an experimental codes, do not ever use them in alpha version
            //Change the below codes and write appopriate converter functions
            /*else if(a->type == INT){
                char tmp[100]={'\0'};
                sprintf(tmp,"%d",cast_fbgc_object_as_int(a)->content);
                a = new_fbgc_str_object_empty(strlen(tmp));
                memcpy(content_fbgc_str_object(a),tmp,strlen(tmp));
                goto STRING_CONCAT;
            }
            else if(a->type == DOUBLE){
                char tmp[100]={'\0'};
                sprintf(tmp,"%f",cast_fbgc_object_as_double(a)->content);
                a = new_fbgc_str_object_empty(strlen(tmp));
                memcpy(content_fbgc_str_object(a),tmp,strlen(tmp));
                goto STRING_CONCAT;                    
            }*/
        }
        return NULL;
    }
    case L_SHIFT:
    {
        //a<<b
        //only lhs ,a ,  can be string b can be different types
        if(a->type == STRING){
            if(b->type == STRING){
                goto STRING_CONCAT;
            }
        }
        return NULL;
    }
    case CARET:
    {
        return NULL;
    }
    case PERCENT:
    {
        
        if(a->type == STRING){
            char buf[100];
            int len = 0;

            if(b->type == INT){
                len = sprintf(buf,content_fbgc_str_object(a),cast_fbgc_object_as_int(b)->content);
            }
            else if(b->type == DOUBLE){
                len = sprintf(buf,content_fbgc_str_object(a),cast_fbgc_object_as_double(b)->content);
            }

            struct fbgc_object * o = new_fbgc_str_object_empty(len);
            memcpy(content_fbgc_str_object(o),buf,len);

            return o;
        }


        return NULL;
    }        
    case LOWER:
    {
        return NULL;
    }        
    case GREATER:
    {
        return NULL;
    }        
    case PIPE:
    {
        return NULL;
    } 
    case AMPERSAND:
    {
        return NULL;
    }        
    case SLASH:
    {
        return NULL;
    }       
    case STAR:
    {   

        //b is always int, a is str;
        if(a->type == INT){
            struct fbgc_object * t = a;
            a = b;
            b = t;
        }
        else if(b->type == INT){
            ;
        }
        else{
            cprintf(111,"Star is overloaded for str and int!\n"); 
           return NULL;
        }

        int count = cast_fbgc_object_as_int(b)->content;
        int len = length_fbgc_str_object(a);

        if(count<0){
            cprintf(111,"Cannot multiply by negative!\n");
            return NULL;
        }

        struct fbgc_object * o = new_fbgc_str_object_empty( len*count);
       //for(int i =0 ; i<count; i++)
        //why while did not work??*
        while(--count>=0)
            memcpy(content_fbgc_str_object(o)+(count*len),content_fbgc_str_object(a),len);
       
        return o;
    }        
    case MINUS:
    {   
        return NULL;
    }        
    case PLUS:
    {
        STRING_CONCAT: ;
        
        struct fbgc_object * o = new_fbgc_str_object_empty(length_fbgc_str_object(a)+length_fbgc_str_object(b));
        memcpy(content_fbgc_str_object(o),content_fbgc_str_object(a),  length_fbgc_str_object(a));
        memcpy(content_fbgc_str_object(o)+length_fbgc_str_object(a)  , content_fbgc_str_object(b),length_fbgc_str_object(b));
        return o;
    } 
}

return NULL;
}



struct fbgc_object * subscript_fbgc_str_object(struct fbgc_object * obj,int i1, int i2){
    //return new str object, it could be sequence inside the object or just one char

    if(i1<length_fbgc_str_object(obj) && i2<=length_fbgc_str_object(obj) && i2>i1 ){
        char * c = content_fbgc_str_object(obj);
        return new_fbgc_str_object_from_substr(c+i1,c+i2);
    }
    return NULL;
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

/*
uint8_t my_strcmp(const char *a, const char *b)
{
    while(*a && *b) {
        if(*a++ != *b++) return 1;
    }
    return (*a || *b);
    //return 1;
    //return 0;
}

*/

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

    struct fbgc_cstr_object *stro =  (struct fbgc_cstr_object*) fbgc_malloc(sizeof(struct fbgc_cstr_object) + str_len ); 
    stro->base.type = CSTRING;
    //stro->base.next = NULL;
    memcpy(&stro->content,str_content,str_len);
    *(&stro->content+str_len) = '\0';
    return (struct fbgc_object*) stro;  

}


struct fbgc_object * new_fbgc_cstr_object_from_substr(const char * str1,const char * str2){
    
    size_t str_len = str2-str1;
    struct fbgc_cstr_object *stro =  (struct fbgc_cstr_object*) fbgc_malloc(sizeof(struct fbgc_cstr_object)+ str_len ); 
    stro->base.type = CSTRING;
    //stro->base.next = NULL;
    //stro->content = ( (char *) stro ) + size_fbgc_cstr_object;
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
