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


struct fbgc_object * operator_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op,struct fbgc_object * result){
	//you have to check before calling this function, a and b must be int type 

	//either int-int or logic-int combination can call this function
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	
	//struct fbgc_object * cdb = result;
	//we are not gonna use a1 but make compiler happy..
	//return operator_method_int(a1,b1,op,result);
	int c = 0;
	double d;
	fbgc_token result_type = (op<=LO_EQ || op>=EXCLAMATION) ? INT: LOGIC;

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
		d = (double)a1*b1/(a1+b1);
		result_type = DOUBLE;
		//*result = new_fbgc_double_object((double)a1*b1/(a1+b1));
		//c = a1/b1;
		//break;
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
		d = (double)a1/b1;
		//result_type = DOUBLE;
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
	case LO_EQ:
	{
		c = (a1<=b1);
		break;
	}
	case GR_EQ:
	{
		c = (a1 >= b1);
		break;
	}
	case EQ_EQ:
	{
		c = (a1 == b1);
		break;
	}
	case NOT_EQ:
	{
		c = (a1 != b1);
		break;
	}
	case LOWER:
	{
		c = (a1<b1);
		break;
	}        
	case GREATER:
	{
		c = (a1>b1);
		break;
	}        
	case PIPE:
	{
		c = (a1 || b1);
		break;
	} 
	case AMPERSAND:
	{
		c = (a1 && b1);
		break;
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


	if(result == NULL){
		switch(result_type){
			case LOGIC:{
				return new_fbgc_logic_object(c);
			}
			case INT:{
				return new_fbgc_int_object(c);
			}
			case DOUBLE:{
				return new_fbgc_double_object(d);
			}
		}
	}else{
		if(result->type == INT) cast_fbgc_object_as_int(result)->content = c;
		else if(result->type ==DOUBLE) cast_fbgc_object_as_double(result)->content = d;
		else if(result->type == LOGIC) cast_fbgc_object_as_logic(result)->content = c;
		return result;
	}
}

struct fbgc_object * operator_fbgc_int_object_address(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op,struct fbgc_object * result){
	//you have to check before calling this function, a and b must be int type 

	//either int-int or logic-int combination can call this function
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	

switch(op)
{
	case R_SHIFT:
	{
		return return_fbgc_object_operator_helper_int(a1>>b1,result);
	}
	case L_SHIFT:
	{
		return return_fbgc_object_operator_helper_int(a1<<b1,result);
	}
	case STARSTAR:
	{
		return return_fbgc_object_operator_helper_int(pow(a1,b1),result);
	}
	case SLASHSLASH:
	{
		return return_fbgc_object_operator_helper_double((double)a1*b1/(a1+b1),result);
	}
	case PLUS:
	{
		return return_fbgc_object_operator_helper_int(a1+b1,result);
	}
	case MINUS:
	{
		return return_fbgc_object_operator_helper_int(a1-b1,result);
	}
	case STAR:
	{
		return return_fbgc_object_operator_helper_int(a1*b1,result);
	}
	case SLASH:
	{
		return return_fbgc_object_operator_helper_double((double)a1/b1,result);
	} 
	case CARET:
	{
		return return_fbgc_object_operator_helper_int(pow(a1,b1),result);
	}
	case PERCENT:
	{
		return return_fbgc_object_operator_helper_int(a1%b1,result);
	}                         
	case LO_EQ:
	{
		return return_fbgc_object_operator_helper_logic(a1<=b1,result);
	}
	case GR_EQ:
	{
		return return_fbgc_object_operator_helper_logic(a1>=b1,result);
	}
	case EQ_EQ:
	{
		return return_fbgc_object_operator_helper_logic(a1==b1,result);
	}
	case NOT_EQ:
	{
		return return_fbgc_object_operator_helper_logic(a1!=b1,result);
	}
	case LOWER:
	{
		return return_fbgc_object_operator_helper_logic(a1<b1,result);
	}        
	case GREATER:
	{
		return return_fbgc_object_operator_helper_logic(a1>b1,result);
	}        
	case PIPE:
	{
		return return_fbgc_object_operator_helper_logic(a1||b1,result);
	} 
	case AMPERSAND:
	{
		return return_fbgc_object_operator_helper_logic(a1&&b1,result);
	} 
	case EXCLAMATION:
	{
		return return_fbgc_object_operator_helper_logic(!a1,result);
	}
	case TILDE:
	{
		return return_fbgc_object_operator_helper_logic(~a1,result);
	}
	case UPLUS:
	{
		return return_fbgc_object_operator_helper_int(a1,result);
	}
	case UMINUS:
	{
		return return_fbgc_object_operator_helper_int(-a1,result);
	}
}	

}

struct fbgc_object * operator_method_int(int a1, int b1,fbgc_token op, struct fbgc_object * result){
	int c = 0;
	double d;
	fbgc_token result_type = (op<=LO_EQ || op>=EXCLAMATION) ? INT: LOGIC;

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
		d = (double)a1*b1/(a1+b1);
		result_type = DOUBLE;
		//*result = new_fbgc_double_object((double)a1*b1/(a1+b1));
		//c = a1/b1;
		//break;
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
		d = (double)a1/b1;
		result_type = DOUBLE;
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
	case LO_EQ:
	{
		c = (a1<=b1);
		break;
	}
	case GR_EQ:
	{
		c = (a1 >= b1);
		break;
	}
	case EQ_EQ:
	{
		c = (a1 == b1);
		break;
	}
	case NOT_EQ:
	{
		c = (a1 != b1);
		break;
	}
	case LOWER:
	{
		c = (a1<b1);
		break;
	}        
	case GREATER:
	{
		c = (a1>b1);
		break;
	}        
	case PIPE:
	{
		c = (a1 || b1);
		break;
	} 
	case AMPERSAND:
	{
		c = (a1 && b1);
		break;
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


	if(result == NULL){
		
		switch(result_type){
			case LOGIC:{
				return new_fbgc_logic_object(c);
				break;
			}
			case INT:{
				return  new_fbgc_int_object(c);
				break;
			}
			case DOUBLE:{
				return new_fbgc_double_object(d);
				break;	
			}
		}
	}else{
		if(result->type == INT) cast_fbgc_object_as_int(result)->content = c;
		else if(result->type ==DOUBLE) cast_fbgc_object_as_double(result)->content = d;
		else if(result->type == LOGIC) cast_fbgc_object_as_logic(result)->content = c;
		return result;
	}
	

}


struct fbgc_object * return_fbgc_object_operator_helper_int(int c,struct fbgc_object * result){
	
	if(result == NULL){
		return new_fbgc_int_object(c);		
	}
	else{
		if(result->type == INT) cast_fbgc_object_as_int(result)->content = c;
		else if(result->type == DOUBLE) cast_fbgc_object_as_double(result)->content = c;
		else if(result->type == LOGIC) cast_fbgc_object_as_logic(result)->content = c;
		return result;
	}
}
struct fbgc_object * return_fbgc_object_operator_helper_double(double c,struct fbgc_object * result){
	
	if(result == NULL){
		return new_fbgc_double_object(c);		
	}
	else{
		if(result->type == INT) cast_fbgc_object_as_int(result)->content = c;
		else if(result->type ==DOUBLE) cast_fbgc_object_as_double(result)->content = c;
		else if(result->type == LOGIC) cast_fbgc_object_as_logic(result)->content = c;
		return result;
	}
}
struct fbgc_object * return_fbgc_object_operator_helper_logic(char c,struct fbgc_object * result){
	
	if(result == NULL){
		return new_fbgc_logic_object(c);		
	}
	else{

		if(result->type == INT) cast_fbgc_object_as_int(result)->content = c;
		else if(result->type ==DOUBLE) cast_fbgc_object_as_double(result)->content = c;
		else if(result->type == LOGIC) cast_fbgc_object_as_logic(result)->content = c;
		return result;
	}
}



struct fbgc_object * operator_R_SHIFT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = a1 >> b1;
	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_L_SHIFT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1<<b1); 
	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_STARSTAR_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (pow(a1,b1));  
	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_SLASHSLASH_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	double c = a1*b1/(a1+b1+0.0);
	return return_fbgc_object_operator_helper_double(c,result); 
}
struct fbgc_object * operator_PLUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1+b1);

	return return_fbgc_object_operator_helper_int(c,result);   
}
struct fbgc_object * operator_MINUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1-b1);       

	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_STAR_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1*b1);       

	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_SLASH_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	double c = (double)a1/b1;      

	return return_fbgc_object_operator_helper_double(c,result);
}
struct fbgc_object * operator_CARET_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (pow(a1,b1));      

	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_PERCENT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	int c = (a1%b1);      

	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_LO_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1<=b1);      

	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_GR_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1>=b1);  
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_EQ_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1==b1);  
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_NOT_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1!=b1);
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_LOWER_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1<b1);
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_GREATER_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1>b1);  
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_PIPE_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1||b1);  
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_AMPERSAND_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);	
	int b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	char c = (a1&&b1);  
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_EXCLAMATION_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	char c = !a1;
	return return_fbgc_object_operator_helper_logic(c,result);
}
struct fbgc_object * operator_TILDE_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	int c = (~a1); 

	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_UPLUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	int c = a1;  

	return return_fbgc_object_operator_helper_int(c,result);
}
struct fbgc_object * operator_UMINUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,struct fbgc_object * result){
	int a1 = (a->type != LOGIC) ? cast_fbgc_object_as_int(a)->content:(int)(cast_fbgc_object_as_logic(a)->content);
	int c = -a1;  
	return return_fbgc_object_operator_helper_int(c,result);
}
//

struct fbgc_object * operator_any_fbgc_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token type,struct fbgc_object * result){

	fbgc_token obj_type = MAX(a->type,b->type);

	switch(obj_type){
		case INT:{
			return fbgc_INT_operators[type](a,b,result);
		}
		case DOUBLE: return NULL;    
		
		case COMPLEX: return NULL;
		case STRING: return NULL;
		case MATRIX: return NULL;
		case TUPLE: return NULL;
	}

}
/*
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
}*/

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

struct fbgc_object * (*fbgc_INT_operators[22]) (struct fbgc_object *, struct fbgc_object *,struct fbgc_object *) =
{   
	operator_R_SHIFT_fbgc_int_object,
	operator_L_SHIFT_fbgc_int_object,
	operator_STARSTAR_fbgc_int_object,
	operator_SLASHSLASH_fbgc_int_object,
	operator_PLUS_fbgc_int_object,
	operator_MINUS_fbgc_int_object,
	operator_STAR_fbgc_int_object,
	operator_SLASH_fbgc_int_object,
	operator_CARET_fbgc_int_object,
	operator_PERCENT_fbgc_int_object,
	operator_LO_EQ_fbgc_int_object,
	operator_GR_EQ_fbgc_int_object,
	operator_EQ_EQ_fbgc_int_object,
	operator_NOT_EQ_fbgc_int_object,
	operator_LOWER_fbgc_int_object,
	operator_GREATER_fbgc_int_object,
	operator_PIPE_fbgc_int_object,
	operator_AMPERSAND_fbgc_int_object,
	operator_EXCLAMATION_fbgc_int_object,
	operator_TILDE_fbgc_int_object,
	operator_UPLUS_fbgc_int_object,
	operator_UMINUS_fbgc_int_object,
};



// struct fbgc_object * (*fbgc_INT_operators[22]) (struct fbgc_object *, struct fbgc_object *, fbgc_token) =
// {   
// 	operator_R_SHIFT_fbgc_object,
// 	operator_L_SHIFT_fbgc_object,
// 	operator_STARSTAR_fbgc_object,
// 	operator_SLASHSLASH_fbgc_object,
// 	operator_PLUS_fbgc_object,
// 	operator_MINUS_fbgc_object,
// 	operator_STAR_fbgc_object,
// 	operator_SLASH_fbgc_object,
// 	operator_CARET_fbgc_object,
// 	operator_PERCENT_fbgc_object,
// 	operator_LO_EQ_fbgc_object,
// 	operator_GR_EQ_fbgc_object,
// 	operator_EQ_EQ_fbgc_object,
// 	operator_NOT_EQ_fbgc_object,
// 	operator_LOWER_fbgc_object,
// 	operator_GREATER_fbgc_object,
// 	operator_PIPE_fbgc_object,
// 	operator_AMPERSAND_fbgc_object,
// 	operator_EXCLAMATION_fbgc_object,
// 	operator_TILDE_fbgc_object,
// 	operator_UPLUS_fbgc_object,
// 	operator_UMINUS_fbgc_object,
// };



void print_fbgc_int_object(struct fbgc_object * obj){
	cprintf(011,"%d",cast_fbgc_object_as_int(obj)->content);  
}

void free_fbgc_int_object(struct fbgc_object * obj){
	//   free(obj);
}
