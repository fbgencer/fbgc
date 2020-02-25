#ifndef FBGC_MATH_H
#define FBGC_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

extern const struct fbgc_cmodule fbgc_math_module;


declare_new_fbgc_cfunction(fbgc_math_initializer);
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
declare_new_fbgc_cfunction(fbgc_sqrt);
declare_new_fbgc_cfunction(fbgc_exp);


declare_new_fbgc_cfunction(fbgc_gsl);



declare_new_fbgc_cfunction(fbgc_random);
declare_new_fbgc_cfunction(fbgc_randint);
declare_new_fbgc_cfunction(fbgc_rand);


#define FBGC_MATH_PI 3.14159265358979323846264
#define FBGC_MATH_E 2.7182818284590452354


#ifdef  __cplusplus
}
#endif

#endif