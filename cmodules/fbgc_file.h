#ifndef FBGC_FILE_H
#define FBGC_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_file_module;


struct file_struct{
	FILE * fp;
};

declare_new_fbgc_cfunction(fbgc_file_initializer);
declare_new_fbgc_cfunction(fbgc_fopen);
declare_new_fbgc_cfunction(fbgc_fprint);
declare_new_fbgc_cfunction(fbgc_fread);
declare_new_fbgc_cfunction(fbgc_freadline);
declare_new_fbgc_cfunction(fbgc_fclose);



#ifdef  __cplusplus
}
#endif

#endif