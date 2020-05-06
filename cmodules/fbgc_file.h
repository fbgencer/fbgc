#ifndef FBGC_FILE_H
#define FBGC_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_file_module;


struct file_struct{
	FILE * fp;
};


#ifdef  __cplusplus
}
#endif

#endif