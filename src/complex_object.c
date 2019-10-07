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


struct fbgc_object * operator_fbgc_complex_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op){
    struct raw_complex a1 = convert_fbgc_object_to_complex(a);
    struct raw_complex b1 = ( b!= NULL ) ? convert_fbgc_object_to_complex(b) : a1;
    struct raw_complex c;
    

switch(op)
{
    case R_SHIFT:
    {
        return NULL;
    }
    case L_SHIFT:
    {
        return NULL;
    }
    case STARSTAR:
    {
        return NULL;
    }
    case SLASHSLASH:
    {
        return NULL;
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
        return NULL;
    }
    case CARET:
    {
        return NULL;
    }
    case PERCENT:
    {
        return NULL;
    }    
    case LO_EQ:
    {
        return NULL;
    }
    case GR_EQ:
    {
        return NULL;
    }
    case EQ_EQ:
    case NOT_EQ:
    {
        uint8_t cmp = (a1.real == b1.real && a1.imag == b1.imag );
        return new_fbgc_int_object( op == NOT_EQ ? !cmp : cmp );
    }
    case LOWER:
    {
        return NULL;
    }
    case GREATER:
    {
        return NULL;
    }
    case PIPE:
    {
        return NULL;
    }
    case AMPERSAND:
    {
        return NULL;
    }
    case EXCLAMATION:
    {
        return NULL;  
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

    return new_fbgc_complex_object(c.real,c.imag);
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
