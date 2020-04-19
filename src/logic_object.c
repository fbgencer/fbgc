#include "fbgc.h"


struct fbgc_object  * new_fbgc_logic_object(int i){

	struct fbgc_logic_object * lo = (struct fbgc_logic_object *)fbgc_malloc(sizeof(struct fbgc_logic_object));
	lo->base.type = LOGIC;
	lo->content = (char) i == 1;

	return (struct fbgc_object * )lo;
}




struct fbgc_object * operator_fbgc_logic_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    char a1 = cast_fbgc_object_as_logic(a)->content;
    char b1 = (b!= NULL) ? cast_fbgc_object_as_logic(b)->content : 0;
    
    char c = 0;

switch(op)
{   

    case RSHIFT:
    case LSHIFT:
    case STARSTAR:
    case SLASHSLASH:    
    case PLUS:
    case MINUS:
    case STAR: 
    case SLASH:  
    case CARET:
    case PERCENT:
    {
        return NULL;
    }
    case LOEQ:
    {
        c = a1 <= b1;
        break;
    }
    case GREQ:
    {
        c = a1 >= b1;
        break;
    }
    case EQEQ:
    {
        c = a1 == b1;
        break;
    }
    case NOTEQ:
    {
        c = a1 != b1;
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
    case EXCLAMATION:
    {
        c = !a1;
        break;
    }
    case TILDE:
    {
        c = ~a1;
        break;
    }
    case UPLUS:
    case UMINUS:
    {
        return NULL;
    }
}

    return new_fbgc_logic_object(c);
}



struct fbgc_object * fbgc_logic_object_to_str(struct fbgc_object * obj){
    return (cast_fbgc_object_as_logic(obj)->content) ? new_fbgc_str_object("true") : new_fbgc_str_object("false");
}