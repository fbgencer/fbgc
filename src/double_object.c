#include "fbgc.h"

struct fbgc_object * new_fbgc_double_object(double db_content){
	struct fbgc_double_object *dbo =  (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
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

/*
struct fbgc_object * plus_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    return new_fbgc_double_object(a1+b1);
}

struct fbgc_object * minus_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    return new_fbgc_double_object(a1-b1);
}

struct fbgc_object * star_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    return new_fbgc_double_object(a1 * b1);
}

struct fbgc_object * slash_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b){
    //you have to check before calling this function, a and b must be double type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    
    //if b1 is 0 check before!!!!

    return (b1 == 0.0) ? NULL : new_fbgc_double_object(a1 / b1);
}*/


struct fbgc_object * binary_op_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    //you have to check before calling this function, a and b must be int type 
    double a1 = convert_fbgc_object_to_double(a);
    double b1 = convert_fbgc_object_to_double(b);
    double c = 0;

    switch(op)
    {
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
        case LO_EQ:
        {
            c = a1<=b1;
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
    return new_fbgc_double_object(c);
}


void print_fbgc_double_object(struct fbgc_object * obj){
    cprintf(011,"%f",cast_fbgc_object_as_double(obj)->content);   
}

void free_fbgc_double_object(struct fbgc_object * obj){
   // free(obj);
}
