#include "fbgc.h"


struct fbgc_object * new_fbgc_complex_object(double r, double z){
	struct fbgc_complex_object * co =  (struct fbgc_complex_object*) fbgc_malloc(sizeof_fbgc_complex_object());
    co->base.type = COMPLEX;
    co->z.real = r;
    co->z.imag = z;
    return (struct fbgc_object*) co;
}

struct fbgc_object * new_fbgc_complex_object_from_str(const char * z_str){
    //see double object creation, derived from double
  	return new_fbgc_complex_object(0, strtod (z_str,NULL));
}
struct fbgc_object * new_fbgc_complex_object_from_substr(const char * z_str_begin,const char * z_str_end){
	//see double object creation, derived from double
    return new_fbgc_complex_object(0, strtod(z_str_begin, NULL));
}

struct fbgc_object * real_fbgc_complex_object(struct fbgc_object * self){
    return new_fbgc_double_object(cast_fbgc_object_as_complex(self)->z.real);
}

struct fbgc_object * imag_fbgc_complex_object(struct fbgc_object * self){
    return new_fbgc_double_object(cast_fbgc_object_as_complex(self)->z.imag);
}

struct fbgc_object * subscript_fbgc_complex_object(struct fbgc_object * self,uint8_t index){
    if(index > 1 ) return NULL;
    double d = (index == 0) ? 
        cast_fbgc_object_as_complex(self)->z.real :
        cast_fbgc_object_as_complex(self)->z.imag; 
    return new_fbgc_double_object(d);
}

struct fbgc_object * conjugate_fbgc_complex_object(struct fbgc_object ** arg,int argc){
    assert(argc == 0);
        return new_fbgc_complex_object(cast_fbgc_object_as_complex(*arg)->z.real,-cast_fbgc_object_as_complex(*arg)->z.imag); 
};

double absolute_fbgc_complex_object(struct fbgc_object * a){
	return hypot(cast_fbgc_object_as_complex(a)->z.real,cast_fbgc_object_as_complex(a)->z.imag);
}

double arg_fbgc_complex_object(struct fbgc_object * a){
	return atan2(cast_fbgc_object_as_complex(a)->z.imag,cast_fbgc_object_as_complex(a)->z.real);
}

struct raw_complex convert_to_radian_fbgc_complex_object(struct fbgc_object * a){
	struct raw_complex c;
	c.real = absolute_fbgc_complex_object(a);
	c.imag = arg_fbgc_complex_object(a);
	return c;
}

struct fbgc_object * operator_fbgc_complex_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    struct raw_complex a1 = convert_fbgc_object_to_complex(a);
    struct raw_complex b1 = ( b!= NULL ) ? convert_fbgc_object_to_complex(b) : a1;
    struct fbgc_object * cmplx = NULL;
	//we are not gonna use a1 but make compiler happy..
    a1 = operator_method_raw_complex(a1,b1,op,&cmplx);
    return cmplx;
}

struct raw_complex operator_method_raw_complex(struct raw_complex a1,struct raw_complex b1,fbgc_token op, struct fbgc_object ** result){
	struct raw_complex c = {0,0};
	fbgc_token result_type = (op>=LO_EQ && op<=EXCLAMATION) ? LOGIC : COMPLEX;

	switch(op)
	{
	    case R_SHIFT:
	    {
	       	assert(0);
	    }
	    case L_SHIFT:
	    {
	        assert(0);
	    }
	    case STARSTAR:
	    {
	    	goto COMPLEX_POWER;
	    }
	    case SLASHSLASH:
	    {
	        assert(0);
	    }
	    case PLUS:
	    {
	        c.real = a1.real + b1.real;
	        c.imag = a1.imag + b1.imag;
	        break;
	    }
	    case MINUS:
	    {
	        c.real = a1.real - b1.real;
	        c.imag = a1.imag - b1.imag;
	        break;
	    }
	    case STAR:
	    {
	        c.real = a1.real * b1.real - a1.imag * b1.imag;
	        c.imag = a1.real * b1.imag + a1.imag * b1.real;
	        break;
	    }                
	    case SLASH:
	    {
		    double denom = b1.real * b1.real + b1.imag * b1.imag;
		    
		    if (denom == 0.)
		        assert(0);

		    c.real = (a1.real * b1.real + a1.imag *b1.imag)/denom;
		    c.imag = (a1.imag * b1.real - a1.real *b1.imag)/denom;

		    break;
	    }
	    case CARET:
	    {
	    	COMPLEX_POWER: ;

	    	double abs = pow( hypot(a1.real,a1.imag) ,b1.real);
	    	double arg = atan2(a1.imag,a1.real) * b1.real;
	    	c.real = abs * cos(arg);
	    	c.imag = abs * sin(arg);
	    	break;
	    }
	    case PERCENT:
	    {
	        assert(0);
	    }    
	    case LO_EQ:
	    {
	        assert(0);
	    }
	    case GR_EQ:
	    {
	        assert(0);
	    }
	    case EQ_EQ:
	    case NOT_EQ:
	    {
	        uint8_t cmp = (a1.real == b1.real && a1.imag == b1.imag );
	        c.real = (op == NOT_EQ) ? !cmp : cmp ;
	        break;
	    }
	    case LOWER:
	    {
	        
	    }
	    case GREATER:
	    {
	        
	    }
	    case PIPE:
	    {
	        
	    }
	    case AMPERSAND:
	    {
	        
	    }
	    case EXCLAMATION:
	    {
	        assert(0);  
	    }
	    case TILDE:
	    {
	        //conjugate
	        c.real = a1.real;
	        c.imag = -a1.imag;
	        break;
	    }
	    case UPLUS:
	    {
	        c = a1;
	        break;
	    }
	    case UMINUS:
	    {
	        c.real = -a1.real;
	        c.imag = -a1.imag;
	        break;
	    }
	}

	if(result != NULL){
		switch(result_type){
			case LOGIC:
			{
				*result = new_fbgc_logic_object(c.real);
				break;
			}
			case COMPLEX:
			{
				*result = new_fbgc_complex_object(c.real,c.imag);
				break;
			}
		}
	}

	return c;

}


struct fbgc_object * get_set_fbgc_complex_object_member(struct fbgc_object * o, const char * str, struct fbgc_object * nm){

    double * p = NULL;

    if( !strcmp(str,"imag")){
        p = &cast_fbgc_object_as_complex(o)->z.imag;
    }
    else if( !strcmp(str,"real") ){
        p = &cast_fbgc_object_as_complex(o)->z.real;
    }
    else if( !strcmp(str,"conj") ){
        return new_fbgc_cfun_object(&conjugate_fbgc_complex_object);
    }   
    else return NULL;

    if(nm == NULL)
        return new_fbgc_double_object( *p ); 
    else {
        *p = convert_fbgc_object_to_double(nm);
    }
} 


void print_fbgc_complex_object(struct fbgc_object * obj){
   cprintf(011,"%g%+gj",cast_fbgc_object_as_complex(obj)->z.real,cast_fbgc_object_as_complex(obj)->z.imag);  
}

void free_fbgc_complex_object(struct fbgc_object * obj){
    //   free(obj);
}
