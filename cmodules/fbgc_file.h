#ifndef FBGC_FILE_H
#define FBGC_FILE_H

#include "fbgc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_object_property_holder _fbgc_file_cstruct_property_holder;


struct file_struct{
	FILE * fp;
};


#ifdef  __cplusplus
}
#endif

#endif