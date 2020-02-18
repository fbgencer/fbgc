#include "fbgc.h"


struct
fbgc_object * new_fbgc_int_object(int int_content){
	struct fbgc_int_object *into =  (struct fbgc_int_object*) fbgc_malloc(sizeof_fbgc_int_object());
    into->base.type = INT;
    //into->base.next = NULL;
    into->content = int_content; 
    return (struct fbgc_object*) into;
}

struct
fbgc_object * derive_from_new_int_object(fbgc_token type, int int_content){
    struct fbgc_int_object *into =  (struct fbgc_int_object*) fbgc_malloc(sizeof_fbgc_int_object());
    into->base.type = type;
    into->content = int_content; 
    return (struct fbgc_object*) into;
}


struct
fbgc_object * new_fbgc_int_object_from_str(const char * int_str){

  	return new_fbgc_int_object(strtol(int_str,NULL,10));
}


struct fbgc_object * new_fbgc_int_object_from_substr(const char * int_str_begin,const char * int_str_end,uint8_t base){
	//now we don't wanna check inf or overflow issue but later we are going to check them
    return new_fbgc_int_object(strtol(int_str_begin, NULL,base));
}

struct fbgc_object * operator_fbgc_int_object2(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    int b1 = convert_fbgc_object_to_int(b);
    
    int * c =  &cast_fbgc_object_as_int(a)->content;
    
switch(op)
{
    case R_SHIFT:
    {
        *c >>= b1;
        break;
    }
    case L_SHIFT:
    {
        *c <<= b1;
        break;
    }
    case STARSTAR:
    {
        *c = pow(*c,b1);
        break;
    }
    case SLASHSLASH:
    {
        // a1//b1 = (a1*b1)/(a1+b1)
        return new_fbgc_double_object(*c*b1/(*c+b1+0.0)); 
    }
    case PLUS:
    {
        *c += b1;
        break;
    }
    case MINUS:
    {
        *c -= b1;
        break;
    }
    case STAR:
    {
        *c *= b1;
        break;
    }
    case SLASH:
    {
        return new_fbgc_double_object(((double)*c)/b1);
    } 
    case CARET:
    {
        *c = pow(*c,b1);
        break;
    }
    case PERCENT:
    {
        *c %= b1;
        break;
    }                         
}

    return a;
    //new_fbgc_int_object(c);
}

struct fbgc_object * operator_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = convert_fbgc_object_to_int(b);
    
    uint8_t operation_type = 0;

    struct fbgc_object * cs = (operation_type == 0) ? new_fbgc_int_object(0) : a;

    int * c =  &cast_fbgc_object_as_int(cs)->content;

    // c = a + b
    // a += b

switch(op)
{
    case R_SHIFT:
    {
        *c = a1>>b1;
        break;
    }
    case L_SHIFT:
    {
        *c = a1<<b1;
        break;
    }
    case STARSTAR:
    {
        *c = pow(a1,b1);
        break;
    }
    case SLASHSLASH:
    {
        // a1//b1 = (a1*b1)/(a1+b1)
        return new_fbgc_double_object(a1*b1/(a1+b1+0.0)); 
    }
    case PLUS:
    {
        *c = a1+b1;
        break;
    }
    case MINUS:
    {
        *c = a1-b1;
        break;
    }
    case STAR:
    {
        *c = a1*b1;
        break;
    }
    case SLASH:
    {
        return new_fbgc_double_object(((double)a1)/b1);
    } 
    case CARET:
    {
        *c = pow(a1,b1);
        break;
    }
    case PERCENT:
    {
        *c = a1%b1;
        break;
    }                         
    case LO_EQ:
    {
        *c = (a1<=b1);
        break;
    }
    case GR_EQ:
    {
        *c = (a1 >= b1);
        break;
    }
    case EQ_EQ:
    {
        *c = (a1 == b1);
        break;
    }
    case NOT_EQ:
    {
        *c = (a1 != b1);
        break;
    }
    case LOWER:
    {
        *c = (a1<b1);
        break;
    }        
    case GREATER:
    {
        *c = (a1>b1);
        break;
    }        
    case PIPE:
    {
        *c = (a1 || b1);
        break;
    } 
    case AMPERSAND:
    {
        *c = (a1 && b1);
        break;
    } 
    case EXCLAMATION:
    {
        *c = (!a1);
        break;   
    }
    case TILDE:
    {
        *c = ~a1;
        break;
    }
    case UPLUS:
    {
        *c = +a1;
        break;
    }
    case UMINUS:
    {
        *c = -a1;
        break;
    }
}

    return cs;
    //new_fbgc_int_object(c);
}


void print_fbgc_int_object(struct fbgc_object * obj){
    cprintf(011,"%d",cast_fbgc_object_as_int(obj)->content);  
}

void free_fbgc_int_object(struct fbgc_object * obj){
    //   free(obj);
}
