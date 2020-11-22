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

struct fbgc_object * new_fbgc_str_object(const char * inc_str){
    size_t len = strlen(inc_str);
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc_object(sizeof(struct fbgc_str_object) + len + 1); 
    stro->base.type = STRING;
    stro->len = len;
    memcpy(stro->content,inc_str,len+1);
    return (struct fbgc_object*) stro;  
}

struct fbgc_object * new_fbgc_str_object_from_substr(const char * str1,const char * str2){
    
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc_object(sizeof(struct fbgc_str_object) + str2-str1+1); 
    stro->base.type = STRING;
    stro->len = str2-str1;
    memcpy(stro->content,str1,stro->len+1);
    stro->content[stro->len] = '\0';

    return (struct fbgc_object*) stro;  
}

struct fbgc_object * new_fbgc_str_object_empty(size_t len){
    struct fbgc_str_object *stro =  (struct fbgc_str_object*) fbgc_malloc_object(sizeof(struct fbgc_str_object) + len+1 ); 
    stro->base.type = STRING;
    stro->len = len;
    stro->content[len] = '\0';
    return (struct fbgc_object*) stro;  
}



struct fbgc_object * new_fbgc_str_object_from_object(struct fbgc_object * obj){
    int len = 0;
    struct fbgc_object * str = NULL;

    switch(obj->type){
        case LOGIC:{
            str = (cast_fbgc_object_as_logic(obj)->content) ? new_fbgc_str_object("true") : new_fbgc_str_object("false");
            break;
        }
        case INT:{
            len = snprintf( NULL, 0, "%d", cast_fbgc_object_as_int(obj)->content);
            str = new_fbgc_str_object_empty(len);
            snprintf(content_fbgc_str_object(str),len+1,"%d",cast_fbgc_object_as_int(obj)->content);
            break;
        }
        case DOUBLE:{
            const char * format = "%.10g";
            len = snprintf( NULL,0,format, cast_fbgc_object_as_double(obj)->content);
            str = new_fbgc_str_object_empty(len);
            snprintf(content_fbgc_str_object(str),len+1,format,cast_fbgc_object_as_double(obj)->content);
            break;
        }
        case COMPLEX:{
            const char * format = "%.10g%+.10gj";
            len = snprintf( NULL,0,format, real_fbgc_complex_object(obj),imag_fbgc_complex_object(obj));
            str = new_fbgc_str_object_empty(len);
            snprintf(content_fbgc_str_object(str),len+1,format,real_fbgc_complex_object(obj),imag_fbgc_complex_object(obj));
            break;
        }
        case STRING:{
            str = obj;
            break;    
        }
        default:{
            assert(0);
        }
    }

    return str;
}


struct fbgc_object * operator_fbgc_str_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){

    if(b == NULL) return NULL;

switch(op)
{
    case RSHIFT:
    {   
        //a>>b
        //only rhs can be string, otherwise it's meaningless
        if(b->type == STRING){
            if(a->type == STRING) goto STRING_CONCAT;
            //Below codes are just an experimental codes, do not ever use them in the main version
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
    case LSHIFT:
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
    case STARSTAR:
    case SLASHSLASH:
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
    case MINUS:
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
    case SLASH:
    {
        //split function for strings
        //a and b must be string
        if(a->type == b->type)
        {
            //we will return tuple of strings
            struct fbgc_object * tp = new_fbgc_tuple_object(0);
            const char * s1 = content_fbgc_str_object(a);
            const char * s2 = s1;

            for(;;)
            {   
                s2 = strstr(s2,content_fbgc_str_object(b));
                if(s2 == NULL){
                    s2 = content_fbgc_str_object(a) + length_fbgc_str_object(a);
                }
                push_back_fbgc_tuple_object(tp, new_fbgc_str_object_from_substr(s1,s2) );
                if(*s2 == '\0') break;
                s1 = (s2 += length_fbgc_str_object(b));
            }
            
            return tp;


            //tp = push_back_fbgc_tuple_object(tp,);
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
            size_t len = 0;


            if(b->type == INT){
                len += sprintf(buf,content_fbgc_str_object(a),cast_fbgc_object_as_int(b)->content);
            }
            else if(b->type == DOUBLE){
                len += sprintf(buf,content_fbgc_str_object(a),cast_fbgc_object_as_double(b)->content);
            }
            else if(b->type == STRING){
                len += sprintf(buf,content_fbgc_str_object(a),content_fbgc_str_object(b));   
            }
            struct fbgc_object * o = new_fbgc_str_object_empty(len);
            memcpy(content_fbgc_str_object(o),buf,len);

            return o;
        }


        return NULL;
    }                                
    case LOEQ:
    {
        return NULL;
    }
    case GREQ:
    {
        return NULL;
    }
    case EQEQ:
    case NOTEQ:
    {   
        //no need to check whether a or b is string, because one of them must be.
        if(a->type == b->type){
            uint8_t cmp = (length_fbgc_str_object(a) == length_fbgc_str_object(b) && 
                memcmp(content_fbgc_str_object(a),content_fbgc_str_object(b),length_fbgc_str_object(a)) == 0);

            return new_fbgc_logic_object( op == NOTEQ ? !cmp : cmp );
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
}

return NULL;
}

int handle_str_index(struct fbgc_object * self,int index){

    index = (index < 0) * length_fbgc_str_object(self) +  index;
    if( index < 0 || index >= length_fbgc_str_object(self) )
        return -1;
    return index;
}

//for the given res object, we will change its content by looking so object
struct fbgc_object * get_char_from_fbgc_str_object(struct fbgc_object * so,int i1,struct fbgc_object * res){
    //change the content of given res object
    if( i1 < length_fbgc_str_object(so) ){
        char * c = content_fbgc_str_object(res);
        *c = *(content_fbgc_str_object(so)+i1);
        return res;
    }
    return NULL;
}

struct fbgc_object * get_object_in_fbgc_str_object(struct fbgc_object * so,int i1, int len){
    //return new str object, it could be sequence inside the object or just one char
    i1 = handle_str_index(so,i1);
    if(i1 < 0) return NULL;
    char * c = content_fbgc_str_object(so);
    return new_fbgc_str_object_from_substr(c+i1,c+i1+len);
}
struct fbgc_object * set_object_in_fbgc_str_object(struct fbgc_object * so,int i1, int len,struct fbgc_object * obj){

    if(get_fbgc_object_type(obj) != STRING) return NULL;

    i1 = handle_str_index(so,i1);
    if(i1 < 0) return 0;

    if(len == 1){
        char * c = content_fbgc_str_object(so);
        *(c+i1) = *(content_fbgc_str_object(obj)); 
    }
    else{
        FBGC_LOGE("not implemented.");
        assert(0);
    }

    return so;
}

uint8_t print_fbgc_str_object(struct fbgc_object * obj){
    return printf("%s",content_fbgc_str_object(obj));   
}

void free_fbgc_str_object(struct fbgc_object * obj){
    //free(cast_fbgc_object_as_str(obj)->content);
    //free(obj);
}

static 
struct fbgc_object * find_fbgc_str_object(struct fbgc_cfun_arg * arg){
    struct fbgc_str_object * self;
    struct fbgc_str_object * so;
    if(parse_tuple_content(arg,"ss",&self,&so) == -1)
        return NULL;

    char * pch = strstr (self->content,so->content);

    if(pch != NULL)
        return new_fbgc_int_object(pch-self->content);
    return NULL;
};


static 
struct fbgc_object * split_fbgc_str_object(struct fbgc_cfun_arg * arg){
    
    if(parse_tuple_content(arg,"!s|ss") == -1)
        return NULL;
    struct fbgc_str_object * self = (struct fbgc_str_object *)arg->arg[0];
    
    struct fbgc_object * str_tuple = new_fbgc_tuple_object(0);

    char * start = self->content;
    char * end = start;

    if(arg->argc == 1){
        for(;;){       
            //Go until we see no space
            while(*end && !isspace(*end))
                ++end;
            //now we have a string from start to end
            push_back_fbgc_tuple_object(str_tuple,new_fbgc_str_object_from_substr(start,end));
            
            //go until finishing the space
            while(*end && isspace(*end))
                ++end;
            //if we are at the end of string break it
            if(*end == 0) break; 

            //new start is end of spaces
            start = end;
        }        
    }
    else{
        struct fbgc_object * splt_str = arg->arg[1];
        while(*end){
            end = strstr(start,content_fbgc_str_object(splt_str));
            if(end == NULL)
                end = content_fbgc_str_object(self)+length_fbgc_str_object(self);
            
            push_back_fbgc_tuple_object(str_tuple,new_fbgc_str_object_from_substr(start,end));
            start = end + length_fbgc_str_object(splt_str);
        }

    }
    return str_tuple;  
};

static const char * lstrip_internal(const char * start, const char * delimiters){
  char c;
  if(delimiters == NULL) delimiters = " \n\r\t";
  const char * d = delimiters;
  while(c = *start){
    if(c == *d){
        ++start;
        d = delimiters;
    }
    else{
        ++d;
        if(!(*d)) break;
    }
  }
    return start;
}

static const char * rstrip_internal(const char * start, const char * end, const char * delimiters){
  char c;
  if(delimiters == NULL) delimiters = " \n\r\t";
  const char * d = delimiters;
  while(end >= start && (c = *end)){
    if(c == *d){
        --end;
        d = delimiters;
    }
    else{
        ++d;
        if(!(*d)) break;
    }
  }
    return end;
}

static
struct fbgc_object * strip_fbgc_str_object(struct fbgc_cfun_arg * arg){
    
    if(parse_tuple_content(arg,"!s|ss") == -1)
        return NULL;

    struct fbgc_str_object * self = (struct fbgc_str_object *)arg->arg[0];
    const char * delim = NULL;
    if(arg->argc > 1) delim = content_fbgc_str_object(arg->arg[1]);

    const char * start = self->content;
    const char * end = self->content + length_fbgc_str_object(self)-1;
    
    start = lstrip_internal(start,delim);
    end = rstrip_internal(start,end,delim);
    struct fbgc_object * xx = new_fbgc_str_object_from_substr(start,++end);
    return xx;
}
static
struct fbgc_object * lstrip_fbgc_str_object(struct fbgc_cfun_arg * arg){
    
    if(parse_tuple_content(arg,"!s|ss") == -1)
        return NULL;

    struct fbgc_str_object * self = (struct fbgc_str_object *)arg->arg[0];
    const char * delim = NULL;
    if(arg->argc > 1) delim = content_fbgc_str_object(arg->arg[1]);

    const char * start = self->content;
    const char * end = self->content + length_fbgc_str_object(self);
    
    start = lstrip_internal(start,delim);
    return new_fbgc_str_object_from_substr(start,end);
}
static
struct fbgc_object * rstrip_fbgc_str_object(struct fbgc_cfun_arg * arg){
    
    if(parse_tuple_content(arg,"!s|ss") == -1)
        return NULL;

    struct fbgc_str_object * self = (struct fbgc_str_object *)arg->arg[0];
    const char * delim = NULL;
    if(arg->argc > 1) delim = content_fbgc_str_object(arg->arg[1]);    
    
    const char * start = self->content;
    const char * end = self->content + length_fbgc_str_object(self)-1;
    
    end = rstrip_internal(start,end,delim);
    return new_fbgc_str_object_from_substr(start,++end);
}

static struct fbgc_object_method _str_methods = {
    .len = 5,
    .method = {
        {.name = "find", .function = &find_fbgc_str_object},
        {.name = "split", .function = &split_fbgc_str_object},
        {.name = "strip", .function = &strip_fbgc_str_object},
        {.name = "lstrip", .function = &lstrip_fbgc_str_object},
        {.name = "rstrip", .function = &rstrip_fbgc_str_object},
    }
};


struct fbgc_object * subscript_operator_fbgc_str_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){
    if(index_obj->type == INT){
        iterable = get_object_in_fbgc_str_object(iterable,cast_fbgc_object_as_int(index_obj)->content,1);   
    }
    else if(index_obj->type == RANGE){
        if(get_fbgc_range_object_iter_type(index_obj) != INT) return NULL;
        int i1 = cast_fbgc_object_as_int(cast_fbgc_object_as_range(index_obj)->start)->content;
        int len = cast_fbgc_object_as_int(cast_fbgc_object_as_range(index_obj)->end)->content - i1;
        iterable = get_object_in_fbgc_str_object(iterable,i1,len);
    }
    else{
        FBGC_LOGE("Index value must be integer, %s given",objtp2str(index_obj));
        return NULL;
    }

    return iterable;
}

struct fbgc_object * subscript_assign_operator_fbgc_str_object(struct fbgc_object * iterable,struct fbgc_object * index_obj, struct fbgc_object * rhs){

    if(index_obj->type != INT){
        FBGC_LOGE("Index value must be integer");
        return NULL;
    }
    return set_object_in_fbgc_str_object(iterable,cast_fbgc_object_as_int(index_obj)->content,1,rhs);   
}


struct fbgc_object * abs_operator_fbgc_str_object(struct fbgc_object * self){
    return new_fbgc_int_object(cast_fbgc_object_as_str(self)->len);
}


static inline size_t size_of_fbgc_str_object(struct fbgc_object * self){
    return (sizeof(struct fbgc_str_object) + cast_fbgc_object_as_str(self)->len+1);
}

const struct fbgc_object_property_holder fbgc_str_object_property_holder = {
    .bits = 
    _BIT_PRINT |
    _BIT_BINARY_OPERATOR |
    _BIT_SUBSCRIPT_OPERATOR |
    _BIT_SUBSCRIPT_ASSIGN_OPERATOR |
    _BIT_ABS_OPERATOR |
    _BIT_SIZE_OF |
    _BIT_METHODS
    ,
    .properties ={
        {.print = &print_fbgc_str_object},
        {.methods = &_str_methods},
        {.binary_operator = &operator_fbgc_str_object}, 
        {.subscript_operator = &subscript_operator_fbgc_str_object},
        {.subscript_assign_operator = &subscript_assign_operator_fbgc_str_object},
        {.abs_operator = &abs_operator_fbgc_str_object},   
        {.size_of = &size_of_fbgc_str_object},
    }
};



//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$     CSTRING   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//###############################################################################

uint8_t my_strncmp(const char *s1, const char *s2, size_t n)
{
    unsigned char u1, u2;
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