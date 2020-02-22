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

/*

struct fbgc_object * operator_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
	//you have to check before calling this function, a and b must be int type 
	double a1 = convert_fbgc_object_to_double(a);
	double b1 = (b!= NULL) ? convert_fbgc_object_to_double(b) : 0;
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
		case EXCLAMATION:
		{
			return new_fbgc_logic_object(!a1);   
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
	return new_fbgc_double_object(c);
}

*/

struct fbgc_object * operator_fbgc_double_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
	//you have to check before calling this function, a and b must be int type 
	double a1 = (a->type != INT) ? cast_fbgc_object_as_double(a)->content : (int)(cast_fbgc_object_as_int(a)->content);	
	double b1 = (b->type != INT) ? cast_fbgc_object_as_double(b)->content : (int)(cast_fbgc_object_as_int(b)->content);	
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
	fbgc_token result_type = (op>=LO_EQ && op<=EXCLAMATION) ? LOGIC : DOUBLE;

	switch(op)
	{
		case R_SHIFT:
		{
			*result = NULL;
			break;
		}
		case L_SHIFT:
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
			a1 != b1;
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


void print_fbgc_double_object(struct fbgc_object * obj){
	cprintf(011,"%f",cast_fbgc_object_as_double(obj)->content);   
}

void free_fbgc_double_object(struct fbgc_object * obj){
   // free(obj);
}
