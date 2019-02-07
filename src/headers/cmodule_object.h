#ifndef CMODULE_OBJECT_H
#define CMODULE_OBJECT_H

/*
	module name : math
	module functions : sin,cos,tan..
	module constants : pi, e,
*/

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_cmodule_object{
    struct fbgc_object base;
    const char *name;
};




#ifdef  __cplusplus
}
#endif

#endif