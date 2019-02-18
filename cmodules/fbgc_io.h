#ifndef FBGC_IO_H
#define FBGC_IO_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_io_module;

//static struct fbgc_object * fbgc_print(struct fbgc_object * sm);
//extern const struct fbgc_cfunction fbgc_print_struct;

//declare_new_fbgc_cfunction(fbgc_print);
declare_new_fbgc_cfunction(fbgc_read);



#ifdef  __cplusplus
}
#endif

#endif