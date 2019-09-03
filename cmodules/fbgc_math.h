#ifndef FBGC_MATH_H
#define FBGC_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_math_module;

declare_new_fbgc_cfunction(fbgc_sin);
declare_new_fbgc_cfunction(fbgc_cos);
declare_new_fbgc_cfunction(fbgc_tan);
declare_new_fbgc_cfunction(fbgc_cosh);
declare_new_fbgc_cfunction(fbgc_sinh);
declare_new_fbgc_cfunction(fbgc_tanh);
declare_new_fbgc_cfunction(fbgc_acos);
declare_new_fbgc_cfunction(fbgc_asin);
declare_new_fbgc_cfunction(fbgc_atan);
declare_new_fbgc_cfunction(fbgc_pow);



#ifdef  __cplusplus
}
#endif

#endif