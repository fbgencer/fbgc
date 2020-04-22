#include "fbgc.h"



struct fbgc_object * new_fbgc_int_object(int int_content){
	struct fbgc_int_object *into =  (struct fbgc_int_object*) fbgc_malloc(sizeof_fbgc_int_object());
	into->base.type = INT;
	//into->base.next = NULL;
	into->content = int_content; 
	return (struct fbgc_object*) into;
}



struct fbgc_object * new_fbgc_int_object_from_str(const char * int_str){

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
	case RSHIFT:
	{
		*c >>= b1;
		break;
	}
	case LSHIFT:
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
	int b1;
	if(b != NULL) b1 = (b->type != LOGIC) ? cast_fbgc_object_as_int(b)->content:(int)(cast_fbgc_object_as_logic(b)->content);
	struct fbgc_object * result = NULL;
switch(op)
{
	case RSHIFT:
	{
		return return_fbgc_object_operator_helper_int(a1>>b1,result);
	}
	case LSHIFT:
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
	case LOEQ:
	{
		return return_fbgc_object_operator_helper_logic(a1<=b1,result);
	}
	case GREQ:
	{
		return return_fbgc_object_operator_helper_logic(a1>=b1,result);
	}
	case EQEQ:
	{
		return return_fbgc_object_operator_helper_logic(a1==b1,result);
	}
	case NOTEQ:
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

	return NULL;
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


uint8_t print_fbgc_int_object(struct fbgc_object * obj){
	int x = cast_fbgc_object_as_int(obj)->content;
	x = x >> 5;
	return x;
	//printf("Gullere ask olsun\n");
	//return cprintf(011,"%d",cast_fbgc_object_as_int(obj)->content);  
}


struct fbgc_object * fbgc_int_object_to_str(struct fbgc_object * obj){
	int len = snprintf( NULL, 0, "%d", cast_fbgc_object_as_int(obj)->content);
	struct fbgc_object * s = new_fbgc_str_object_empty(len);
	snprintf(content_fbgc_str_object(s),len+1,"%d",cast_fbgc_object_as_int(obj)->content);
	return s;
}

void free_fbgc_int_object(struct fbgc_object * obj){
	printf("Free called\n");
}


struct fbgc_object_property_holder fbgc_int_object_property_holder = {
	.bits = _BIT_PRINT | 
	_BIT_BINARY_OPERATOR |
	_BIT_DESTRUCTOR |
	_BIT_TO_STR

	,
	.properties ={
		{.binary_operator = &operator_fbgc_int_object},
		{.to_str = &fbgc_int_object_to_str},
		{.print = &print_fbgc_int_object},
		{.destructor = &free_fbgc_int_object},				
	}
};

//print_fbgc_int_object,
	//free_fbgc_int_object
