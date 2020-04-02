#ifndef INTERPRETER_H
#define INTERPRETER_H

#ifdef __cplusplus
extern "C" {
#endif

uint8_t interpreter(struct fbgc_object ** field_obj);
struct fbgc_object * run_code(struct fbgc_ll_base * code,struct fbgc_object ** sp,int sctr, int fctr );

#ifdef  __cplusplus
}
#endif

#endif