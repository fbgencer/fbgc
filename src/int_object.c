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



struct fbgc_object * operator_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    int a1 = convert_fbgc_object_to_int(a);
    int b1 = (b != NULL) ? convert_fbgc_object_to_int(b) : 0;
    int c = 0;

switch(op)
{
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
    case STARSTAR:
    {
        c = pow(a1,b1);
        break;
    }
    case SLASHSLASH:
    {
        // a1//b1 = (a1*b1)/(a1+b1)
        return new_fbgc_double_object(a1*b1/(a1+b1+0.0)); 
    }
    case PLUS:
    {
        c = a1+b1;
        break;
    }
    case MINUS:
    {
        c = a1-b1;
        break;
    }
    case STAR:
    {
        c = a1*b1;
        break;
    }
    case SLASH:
    {
        return new_fbgc_double_object(((double)a1)/b1);
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
    case LO_EQ:
    {
        return new_fbgc_logic_object(a1<=b1);
    }
    case GR_EQ:
    {
        return new_fbgc_logic_object(a1 >= b1);
    }
    case EQ_EQ:
    {
        return new_fbgc_logic_object(a1 == b1);
    }
    case NOT_EQ:
    {
        return new_fbgc_logic_object(a1 != b1);
    }
    case LOWER:
    {
        return new_fbgc_logic_object(a1<b1);
    }        
    case GREATER:
    {
        return new_fbgc_logic_object(a1>b1);
    }        
    case PIPE:
    {
        return new_fbgc_logic_object(a1 || b1);
    } 
    case AMPERSAND:
    {
        return new_fbgc_logic_object(a1 && b1);
    } 
    case EXCLAMATION:
    {
        return new_fbgc_logic_object(!a1);   
    }
    case TILDE:
    {
        c = ~a1;
        break;
    }
    case UPLUS:
    {
        c = +a1;
        break;
    }
    case UMINUS:
    {
        c = -a1;
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
