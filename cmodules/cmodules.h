#ifndef CMODULES_H
#define CMODULES_H
//stl and io should be loaded
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_stl.h"


#include "../cmodules/fbgc_math.h"
#include "../cmodules/fbgc_file.h"
#include "../cmodules/fbgc_time.h"

#ifdef __cplusplus
extern "C" {
#endif



const struct fbgc_object_property_holder  * __property_holders[] =
{
	&_fbgc_math_property_holder,
	&_fbgc_file_cstruct_property_holder,
	//&fbgc_time_module			
};




#ifdef  __cplusplus
}
#endif

#endif