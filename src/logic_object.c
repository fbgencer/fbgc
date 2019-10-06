#include "fbgc.h"


struct fbgc_object  * new_fbgc_logic_object(int i){

	struct fbgc_logic_object * lo = fbgc_malloc(sizeof(struct fbgc_logic_object));
	lo->base.type = LOGIC;
	lo->content = (char) i == 1;

	return (struct fbgc_object * )lo;
}




struct fbgc_object * binary_op_fbgc_logic_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    char a1 = cast_fbgc_object_as_logic(a)->content;
    char b1 = cast_fbgc_object_as_logic(b)->content;
    
    char c = 0;

switch(op)
{   

    case R_SHIFT:
    case L_SHIFT:
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
    case LO_EQ:
    {
        c = a1 <= b1;
        break;
    }
    case GR_EQ:
    {
        c = a1 >= b1;
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

}

    return new_fbgc_logic_object(c);
}