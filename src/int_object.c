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

	//either int-int or logic-int combination can call this function
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c;

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
        c = (!a1);   
        break;
    }
    case TILDE:
    {
        c = ~a1;
        break;
    }
    case UPLUS:
    {
        c = a1;
        break;
    }
    case UMINUS:
    {
        c = -a1;
        break;
    }
}
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}


inline struct fbgc_object * operator_R_SHIFT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = a1 >> b1;
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_L_SHIFT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1<<b1); 
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);	
}
inline struct fbgc_object * operator_STARSTAR_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (pow(a1,b1));  
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_SLASHSLASH_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	return new_fbgc_double_object(a1*b1/(a1+b1+0.0)); 
}
inline struct fbgc_object * operator_PLUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1+b1);
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);	   
}
inline struct fbgc_object * operator_MINUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1-b1);       
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_STAR_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1*b1);       
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_SLASH_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	return new_fbgc_double_object((double)a1/b1);      
}
inline struct fbgc_object * operator_CARET_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (pow(a1,b1));      
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_PERCENT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1%b1);      
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_LO_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1<=b1);      
    return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_GR_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1>=b1);  
    return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_EQ_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1==b1);  
	return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_NOT_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1!=b1);
	return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_LOWER_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1<b1);
	return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_GREATER_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1>b1);  
	return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_PIPE_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1||b1);  
	return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_AMPERSAND_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1&&b1);  
	return new_fbgc_logic_object(c);
}
inline struct fbgc_object * operator_EXCLAMATION_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	return new_fbgc_logic_object(!a1);  
}
inline struct fbgc_object * operator_TILDE_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	int c = (~a1); 
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_UPLUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	int c = a1;  
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
inline struct fbgc_object * operator_UMINUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	int c = -a1;  
	uint8_t operation_type = 1;
	if(operation_type){
		cast_fbgc_object_as_int(a)->content = c;
		return a;
	}
    else return new_fbgc_int_object(c);
}
//

/////////////
struct fbgc_object * operator_R_SHIFT_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){

	switch(type){
		case INT:{
			return operator_R_SHIFT_fbgc_int_object(a,b);
		}
		case DOUBLE: return NULL;    
		
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}

}

/////////////
struct fbgc_object * operator_L_SHIFT_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	
	switch(type){
		case INT:{  
			return operator_L_SHIFT_fbgc_int_object(a,b);  
		}
		case DOUBLE: return NULL;
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_STARSTAR_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type){

	switch(type){
		case INT:{  
			return operator_STARSTAR_fbgc_int_object(a,b); 
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(pow(a1,b1));
		}

		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_SLASHSLASH_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_SLASHSLASH_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(a1*b1/(a1+b1));
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_PLUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
  
	switch(type){
		case INT:{
			return operator_PLUS_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(a1+b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

////////////
struct fbgc_object * operator_MINUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){

	switch(type){
		case INT:{
			return operator_MINUS_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(a1-b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

struct fbgc_object * operator_STAR_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type){
	switch(type){
		case INT:{
			return operator_STAR_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(a1*b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

struct fbgc_object * operator_SLASH_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
 
	switch(type){
		case INT:{
 			return operator_SLASH_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(a1/b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_CARET_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	switch(type){
		case INT:{
			return operator_CARET_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_double_object(pow(a1,b1));
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_PERCENT_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_PERCENT_fbgc_int_object(a,b);
		}
		case DOUBLE: return NULL;
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_LO_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	
	switch(type){
		case INT:{
 			return operator_LO_EQ_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1<=b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_GR_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_GR_EQ_fbgc_int_object(a,b);  
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1>=b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}
/////////////
struct fbgc_object * operator_EQ_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	

	switch(type){
		case INT:{
			return operator_EQ_EQ_fbgc_int_object(a,b);   
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1==b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}  
}
/////////////
struct fbgc_object * operator_NOT_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_NOT_EQ_fbgc_int_object(a,b);  
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1!=b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}
/////////////
struct fbgc_object * operator_LOWER_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	switch(type){
		case INT:{
			return operator_LOWER_fbgc_int_object(a,b); 
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1<b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}
/////////////
struct fbgc_object * operator_GREATER_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){

	switch(type){
		case INT:{
			return operator_GREATER_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1>b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}
/////////////
struct fbgc_object * operator_PIPE_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_PIPE_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
		   return new_fbgc_int_object(a1||b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}
/////////////
struct fbgc_object * operator_AMPERSAND_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){
	switch(type){
		case INT:{
			return operator_AMPERSAND_fbgc_int_object(a,b);  
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			double b1 = convert_fbgc_object_to_double(b);
			return new_fbgc_int_object(a1&&b1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_EXCLAMATION_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type){

	switch(type){
		case INT:{
			return operator_EXCLAMATION_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			return new_fbgc_int_object(!a1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

/////////////
struct fbgc_object * operator_TILDE_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_TILDE_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			return new_fbgc_double_object(-a1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}


/////////////
struct fbgc_object * operator_UPLUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type){
	

	switch(type){
		case INT:{
			return operator_UPLUS_fbgc_int_object(a,b);
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			return new_fbgc_double_object(a1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}


/////////////
struct fbgc_object * operator_UMINUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type){
	
	switch(type){
		case INT:{
			return operator_UMINUS_fbgc_int_object(a,b);  
		}
		case DOUBLE:{
			double a1 = convert_fbgc_object_to_double(a);
			return new_fbgc_double_object(-a1);
		}
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}
}

// struct fbgc_object * operator_function_call(struct fbgc_object * a,struct fbgc_object * b,fbgc_token function_type){
// 	fbgc_token type = MAX(a->type,b->type);

// 	switch(type){
// 		case INT:{
// 			return   
// 		}
// 		case DOUBLE:{
// 			double a1 = convert_fbgc_object_to_double(a);
// 			return new_fbgc_double_object(-a1);
// 		}
// 		case COMPLEX: return NULL;
// 		case STRING: return NULL;
// 		case MATRIX: return NULL;
// 		case TUPLE: return NULL;
// 	}
// }


struct fbgc_object * (*fbgc_INT_operators[22]) (struct fbgc_object *, struct fbgc_object *, fbgc_token) =
{   
	operator_R_SHIFT_fbgc_object,
	operator_L_SHIFT_fbgc_object,
	operator_STARSTAR_fbgc_object,
	operator_SLASHSLASH_fbgc_object,
	operator_PLUS_fbgc_object,
	operator_MINUS_fbgc_object,
	operator_STAR_fbgc_object,
	operator_SLASH_fbgc_object,
	operator_CARET_fbgc_object,
	operator_PERCENT_fbgc_object,
	operator_LO_EQ_fbgc_object,
	operator_GR_EQ_fbgc_object,
	operator_EQ_EQ_fbgc_object,
	operator_NOT_EQ_fbgc_object,
	operator_LOWER_fbgc_object,
	operator_GREATER_fbgc_object,
	operator_PIPE_fbgc_object,
	operator_AMPERSAND_fbgc_object,
	operator_EXCLAMATION_fbgc_object,
	operator_TILDE_fbgc_object,
	operator_UPLUS_fbgc_object,
	operator_UMINUS_fbgc_object,
};



void print_fbgc_int_object(struct fbgc_object * obj){
	cprintf(011,"%d",cast_fbgc_object_as_int(obj)->content);  
}

void free_fbgc_int_object(struct fbgc_object * obj){
	//   free(obj);
}
