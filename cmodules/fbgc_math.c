#include "../src/fbgc.h"
#include "fbgc_math.h"

new_fbgc_cfunction(fbgc_sin,"sin")
{
	printf("I am a sin functionn\n");

	return NULL;
}

new_fbgc_cfunction(fbgc_cos,"cos"){

	printf("I am a cos functionn\n");

	return arg;
}

new_fbgc_cfunction(fbgc_tan,"tan"){

	printf("I am a tan functionn\n");

	return arg;
}

//Work on this, is it possible to cast ?
const struct fbgc_cmodule fbgc_math_module = 
{
	.name = "math",
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_sin_struct,&fbgc_cos_struct,&fbgc_tan_struct,NULL
	}
};

