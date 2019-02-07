#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

int error(const char *x);
int error2(const char* fmt, ...);

void fbgc_assert(int cond,const char* fmt, ...);


void cprintf(char color,const char *format, ...);


#ifdef  __cplusplus
}
#endif

#endif