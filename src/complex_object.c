// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"


struct fbgc_object * new_fbgc_complex_object(double r, double z){
	struct fbgc_complex_object * co =  (struct fbgc_complex_object*) fbgc_malloc_object(sizeof_fbgc_complex_object());
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

struct fbgc_object * real_fbgc_complex_object_to_double(struct fbgc_object * self){
	return new_fbgc_double_object(cast_fbgc_object_as_complex(self)->z.real);
}

struct fbgc_object * imag_fbgc_complex_object_to_double(struct fbgc_object * self){
	return new_fbgc_double_object(cast_fbgc_object_as_complex(self)->z.imag);
}


static struct fbgc_object * conjugate_fbgc_complex_object(struct fbgc_cfun_arg * arg){
	//if(argc == 1)
	//	return new_fbgc_complex_object(cast_fbgc_object_as_complex(arg[0])->z.real,-cast_fbgc_object_as_complex(arg[0])->z.imag); 
	return NULL;
};


struct raw_complex convert_to_radian_fbgc_complex_object(struct fbgc_object * a){
	struct raw_complex c;
	c.real = hypot(cast_fbgc_object_as_complex(a)->z.real,cast_fbgc_object_as_complex(a)->z.imag);
	c.imag = atan2(cast_fbgc_object_as_complex(a)->z.imag,cast_fbgc_object_as_complex(a)->z.real);
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
	fbgc_token result_type = (op>=LOEQ && op<=EXCLAMATION) ? LOGIC : COMPLEX;

	switch(op)
	{
		case RSHIFT:
		{
			assert(0);
		}
		case LSHIFT:
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
		case LOEQ:
		{
			assert(0);
		}
		case GREQ:
		{
			assert(0);
		}
		case EQEQ:
		case NOTEQ:
		{
			uint8_t cmp = (a1.real == b1.real && a1.imag == b1.imag );
			c.real = (op == NOTEQ) ? !cmp : cmp ;
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




struct fbgc_object * get_set_fbgc_complex_object_real_member(struct fbgc_object * self, struct fbgc_object * rhs){
	if(rhs == NULL) return real_fbgc_complex_object_to_double(self);
	cast_fbgc_object_as_complex(self)->z.real = convert_fbgc_object_to_double(rhs);
	return self;
}


struct fbgc_object * get_set_fbgc_complex_object_imag_member(struct fbgc_object * self, struct fbgc_object * rhs){
	if(rhs == NULL) return imag_fbgc_complex_object_to_double(self);
	
	cast_fbgc_object_as_complex(self)->z.imag = convert_fbgc_object_to_double(rhs);
	return self;
}

static struct fbgc_object_member _complex_members = {
	.len = 2,
	.member = {
		{.name = "real", .function = get_set_fbgc_complex_object_real_member},
		{.name = "imag", .function = get_set_fbgc_complex_object_imag_member}
	}
};


static struct fbgc_object_method _complex_methods = {
	.len = 1,
	.method = {
		{.name = "conj", .function = &conjugate_fbgc_complex_object},
	}
};


uint8_t print_fbgc_complex_object(struct fbgc_object * obj){
   return printf("%g%+gj",cast_fbgc_object_as_complex(obj)->z.real,cast_fbgc_object_as_complex(obj)->z.imag);  
}

void print_raw_complex(struct raw_complex z){
   cprintf(011,"%g%+gj",z.real,z.imag);  
}

static 
struct fbgc_object * fbgc_complex_object_to_str(struct fbgc_object * obj){
	const char * format = "%.10g%+.10gj";
	int len = snprintf( NULL,0,format, real_fbgc_complex_object(obj),imag_fbgc_complex_object(obj));
	struct fbgc_object * s = new_fbgc_str_object_empty(len);
	snprintf(content_fbgc_str_object(s),len+1,format,real_fbgc_complex_object(obj),imag_fbgc_complex_object(obj));
	return s;
}


static struct fbgc_object * subscript_operator_fbgc_complex_object(struct fbgc_object * iterable,struct fbgc_object * index_obj){
	if(index_obj->type != INT){
		FBGC_LOGE("Index value must be integer");
		return NULL;
	}
	
	if(cast_fbgc_object_as_int(index_obj)->content == 0)
		return real_fbgc_complex_object_to_double(iterable);
	else if(cast_fbgc_object_as_int(index_obj)->content == 1)
		return imag_fbgc_complex_object_to_double(iterable);
	
	return NULL;    
}
//{.get_set_member = &get_set_fbgc_complex_object_member},


static struct fbgc_object * abs_fbgc_complex_object(struct fbgc_object * a){
	return new_fbgc_double_object(hypot(cast_fbgc_object_as_complex(a)->z.real,cast_fbgc_object_as_complex(a)->z.imag));
}


static inline size_t size_of_fbgc_complex_object(struct fbgc_object * self){
    return sizeof(struct fbgc_complex_object);
}

const struct fbgc_object_property_holder fbgc_complex_object_property_holder = {
	.bits = 
	_BIT_PRINT |
	_BIT_TO_STR |
	_BIT_BINARY_OPERATOR |
	_BIT_SUBSCRIPT_OPERATOR |
	_BIT_MEMBERS |
	_BIT_METHODS |
	_BIT_ABS_OPERATOR |
	_BIT_SIZE_OF 
	,
	.properties ={
		{.print = &print_fbgc_complex_object},
		{.members = &_complex_members},
		{.methods = &_complex_methods},
		{.binary_operator = &operator_fbgc_complex_object},
		{.subscript_operator = &subscript_operator_fbgc_complex_object},
		{.abs_operator = &abs_fbgc_complex_object},
		{.size_of = &size_of_fbgc_complex_object},
		{.to_str = &fbgc_complex_object_to_str},
		
	}
};
