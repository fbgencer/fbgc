#ifndef INTERPRETER_H
#define INTERPRETER_H

#ifdef __cplusplus
extern "C" {
#endif

uint8_t interpreter(struct fbgc_object ** field_obj);
struct fbgc_object * run_code(struct fbgc_ll_base * code,struct fbgc_object ** sp,int sctr, int fctr );


#ifdef LOG_INTERPRETER
#define INTERPRETER_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define INTERPRETER_LOGD(format,...) LOGD(format,##__VA_ARGS__) 
#else
#define INTERPRETER_LOGV(...)
#define INTERPRETER_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif