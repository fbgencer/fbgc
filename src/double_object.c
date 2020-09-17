#include "fbgc.h"

struct fbgc_object * new_fbgc_double_object(double db_content){
	struct fbgc_double_object *dbo =  (struct fbgc_double_object*) fbgc_malloc_object(sizeof_fbgc_double_object());
	dbo->base.type = DOUBLE;
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

struct fbgc_object * operator_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){

	//you have to check before calling this function, a and b must be int type 
	double a1 = (a->type != INT) ? cast_fbgc_object_as_double(a)->content : (int)(cast_fbgc_object_as_int(a)->content);	

	double b1 = 0;

	if(b != NULL)
		b1 = (b->type != INT) ? cast_fbgc_object_as_double(b)->content : (int)(cast_fbgc_object_as_int(b)->content);	
	//double a1 = convert_fbgc_object_to_double(a);	
	//double b1 = (b!= NULL) ? convert_fbgc_object_to_double(b) : 0;
	struct fbgc_object * cdb = NULL;

	//we are not gonna use a1 but make compiler happy..
	a1 = operator_method_double(a1,b1,op,&cdb);
	return cdb;
}

double operator_method_double(double a1, double b1, fbgc_token op, struct fbgc_object ** result){
	//different functions may call this function, they can get the answer from the returned value or they can provide result object
	double c = 0;
	fbgc_token result_type = (op>=LOEQ && op<=EXCLAMATION) ? LOGIC : DOUBLE;

	switch(op)
	{
		case RSHIFT:
		{
			*result = NULL;
			break;
		}
		case LSHIFT:
		{
			*result = NULL;
			break;
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
			*result = NULL;
			break;
		}                                      
		case LOEQ:
		{
			c = a1<=b1;
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
			c = a1 < b1;
			break; 
		}
		case GREATER:
		{
			c = a1 > b1;
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
			c = ceil(a1);
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

	if(result != NULL){
		switch(result_type){
			case LOGIC:
			{
				*result = new_fbgc_logic_object(c);
				break;
			}
			case DOUBLE:
			{
				*result = new_fbgc_double_object(c);
				break;
			}
		}
	}
	return c;    
}


uint8_t print_fbgc_double_object(struct fbgc_object * obj){
	return printf("%g",cast_fbgc_object_as_double(obj)->content);   
}

static struct fbgc_object * fbgc_double_object_to_str(struct fbgc_object * obj){
	const char * format = "%.10g";
	int len = snprintf( NULL,0,format, cast_fbgc_object_as_double(obj)->content);
	struct fbgc_object * s = new_fbgc_str_object_empty(len);
	snprintf(content_fbgc_str_object(s),len+1,format,cast_fbgc_object_as_double(obj)->content);
	return s;
}

static struct fbgc_object * fbgc_double_object_to_int(struct fbgc_object * obj){
	return new_fbgc_int_object(cast_fbgc_object_as_double(obj)->content);
}

static struct fbgc_object * abs_operator_fbgc_double_object(struct fbgc_object * self){
	return new_fbgc_double_object(fabs(cast_fbgc_object_as_double(self)->content));
}


static inline size_t size_of_fbgc_double_object(struct fbgc_object * self){
    return sizeof(struct fbgc_double_object);
}


const struct fbgc_object_property_holder fbgc_double_object_property_holder = {
	.bits = 
	_BIT_PRINT |
	_BIT_TO_STR |
	_BIT_TO_INT |
	_BIT_BINARY_OPERATOR |
	_BIT_ABS_OPERATOR |
	_BIT_SIZE_OF
	,
	.properties ={
		{.print = &print_fbgc_double_object},
		{.binary_operator = &operator_fbgc_double_object},
		{.abs_operator = &abs_operator_fbgc_double_object},
		{.size_of = &size_of_fbgc_double_object},
		{.to_int = &fbgc_double_object_to_int},
		{.to_str = &fbgc_double_object_to_str},
		
	}
};
