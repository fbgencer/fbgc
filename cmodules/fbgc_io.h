#ifndef FBGC_IO_H
#define FBGC_IO_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_io_module;

declare_new_fbgc_cfunction(fbgc_print);
declare_new_fbgc_cfunction(fbgc_read);



#ifdef  __cplusplus
}
#endif

#endif