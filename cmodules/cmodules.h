#ifndef CMODULES_H
#define CMODULES_H

#ifdef __cplusplus
extern "C" {
#endif

//stl and io are already loaded
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_stl.h"


#include "../cmodules/fbgc_math.h"
#include "../cmodules/fbgc_file.h"



const struct fbgc_cmodule  * __cmodules[] =
{
	&fbgc_math_module,
	&fbgc_file_module,			
};



#ifdef  __cplusplus
}
#endif

#endif