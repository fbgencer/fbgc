#include "../src/fbgc.h"
#include "fbgc_math.h"
#include <math.h>

new_fbgc_cfunction(fbgc_sin,"sin")
{
	printf("I am a sin functionn\n");

	return new_fbgc_double_object(sin(convert_fbgc_object_to_double(arg)));
}

new_fbgc_cfunction(fbgc_cos,"cos"){

	printf("I am a cos functionn\n");

	return arg;
}

new_fbgc_cfunction(fbgc_pi,"pi"){

	printf("I am a pi functionn\n");

	return new_fbgc_double_object(3.141592653589793);
}

//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_math_module = 
{
	.name = "math",
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_sin_struct,&fbgc_cos_struct,&fbgc_pi_struct,NULL
	}
};

