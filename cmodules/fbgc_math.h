#ifndef FBGC_MATH_H
#define FBGC_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_math_module;

declare_new_fbgc_cfunction(fbgc_sin);
declare_new_fbgc_cfunction(fbgc_cos);
declare_new_fbgc_cfunction(fbgc_pi);


#ifdef  __cplusplus
}
#endif

#endif