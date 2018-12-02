#ifndef MATH_UFBG_H
#define MATH_UFBG_H
#include <string>
#include "math.h"


#define MATH_UFBG_CODE 1


const static string math_function_table[] =
{
	"cos",
	"sin",
	"tan",
	"cot",
	"acos",
	"asin",
	"atan",
	"acot",
	"atan2",
	"cosh",
	"sinh",
	"tanh",
	"acosh",
	"asinh",
	"atanh",	
};

double math_ufbg_function(char func,double x, double y){
	switch(func){
		case 0: return cos(x);
		case 1: return sin(x);
		case 2: return tan(x);
		//case 3: return cot(x);
		case 4: return acos(x);
		case 5: return asin(x);
		case 6: return atan(x);
		//case 7: return acot(x);
		case 8: return atan2(x,y);
		case 9: return cosh(x);
		case 10: return sinh(x);
		case 11: return tanh(x);
	}
}

#endif