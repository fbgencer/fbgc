#ifndef FBGC_TIME_H
#define FBGC_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_time_module;

//static struct fbgc_object * fbgc_print(struct fbgc_object * sm);
//extern const struct fbgc_cfunction fbgc_print_struct;


struct time_struct{
	clock_t clk;
};

declare_new_fbgc_cfunction(fbgc_time_initializer);
declare_new_fbgc_cfunction(fbgc_tic);
declare_new_fbgc_cfunction(fbgc_toc);



#ifdef  __cplusplus
}
#endif

#endif