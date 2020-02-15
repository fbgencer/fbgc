#include "../src/fbgc.h"
#include "fbgc_time.h"
#include <time.h>



new_fbgc_cfunction(fbgc_tic,"tic")
{	
	if(argc != 0) return NULL;

	struct fbgc_cstruct_object * so = new_fbgc_cstruct_object(sizeof(struct time_struct), &fbgc_time_module);
	struct time_struct * ts = (struct time_struct *) so->cstruct; 	

	ts->clk = clock();

	return (struct fbgc_object *) so;
}

new_fbgc_cfunction(fbgc_toc,"toc")
{	
	if(argc != 1) return NULL;

	struct fbgc_cstruct_object * so = cast_fbgc_object_as_cstruct(arg[0]);
	assert(so->parent == &fbgc_time_module);

	struct time_struct * ts = (struct time_struct *) so->cstruct; 	

	clock_t begin = ts->clk;
	clock_t end = clock();

	double tm = (double)(end - begin) / CLOCKS_PER_SEC; 	

	return (struct fbgc_object *) new_fbgc_double_object(tm);
}







new_fbgc_cfunction(fbgc_time_initializer,"time")
{	
	return NULL;
}
const struct fbgc_cmodule fbgc_time_module = 
{
	.initializer = &fbgc_time_initializer_struct,
	.functions = (const struct fbgc_cfunction*[])
	{
		&fbgc_tic_struct,
		&fbgc_toc_struct,
		NULL
	}
};

