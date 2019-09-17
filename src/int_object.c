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
	//now we don't wanna check inf or overlfow issue but later we are going to check them
    return new_fbgc_int_object(strtol(int_str_begin, NULL,base));
}


struct fbgc_object * binary_op_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = convert_fbgc_object_to_int(b);
    int c = 0;

switch(op)
{
    case STARSTAR:
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
    }        
    case PLUS:
    {
        c = a1+b1;
        break;
    } 
}

    return new_fbgc_int_object(c);
}


void print_fbgc_int_object(struct fbgc_object * obj){
    cprintf(011,"%d",cast_fbgc_object_as_int(obj)->content);  
}

void free_fbgc_int_object(struct fbgc_object * obj){
    //   free(obj);
}
