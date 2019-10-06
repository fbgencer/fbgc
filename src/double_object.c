#include "fbgc.h"

struct fbgc_object * new_fbgc_double_object(double db_content){
	struct fbgc_double_object *dbo =  (struct fbgc_double_object*) fbgc_malloc(sizeof_fbgc_double_object());
    dbo->base.type = DOUBLE;
    //dbo->base.next = NULL;
    dbo->content = db_content; 
    return (struct fbgc_object*) dbo;
}

struct fbgc_object * new_fbgc_double_object_from_str(const char * db_str){
  	return new_fbgc_double_object(strtod (db_str,NULL));
}

struct fbgc_object * new_fbgc_double_object_from_substr(const char * db_str_begin,const char * db_str_end){
	//now we don't wanna check inf or overlfow issue but later we are going to check them
    return new_fbgc_double_object(strtod (db_str_begin, NULL));
}


struct fbgc_object * binary_op_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    double c = 0;

    switch(op)
    {
        case R_SHIFT:
        {
            return NULL;
            //c = a1>>b1;
            //break;
        }
        case L_SHIFT:
        {
            return NULL;
            //c = a1<<b1;
            //break;
        }        
        case STARSTAR:
        {
            c = pow(a1,b1);
            break;
        }
        case SLASHSLASH:
        {
            c =  1.0/(1.0/a1 + 1.0/b1); 
            break;
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
            c = a1/b1;
            break;
        }
        case CARET:
        {
            c = pow(a1,b1);
            break;
        }
        case PERCENT:
        {
            //c = a1%b1;
            return NULL;
            //break;
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
            return new_fbgc_logic_object(a1 < b1); 
        }        
        case GREATER:
        {
            return new_fbgc_logic_object(a1 > b1);
        }        
        case PIPE:
        {
            return new_fbgc_logic_object(a1 || b1);
        } 
        case AMPERSAND:
        {
            return new_fbgc_logic_object(a1 && b1);
        }        
    }
    return new_fbgc_double_object(c);
}


void print_fbgc_double_object(struct fbgc_object * obj){
    cprintf(011,"%f",cast_fbgc_object_as_double(obj)->content);   
}

void free_fbgc_double_object(struct fbgc_object * obj){
   // free(obj);
}
