#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


#define _FBGC_NO_ERROR 0
#define _FBGC_LEXER_ERROR 1
#define _FBGC_SYNTAX_ERROR 2
#define _FBGC_DUMMY_ERROR 3
#define _FBGC_DUMMY2_ERROR 4


#if defined CLOSE_PRINTF
#define cprintf(...)
#else
int cprintf(char color,const char *format, ...);
#endif

int fbgc_assert(unsigned char cond,const char * format,...);
int fbgc_error(unsigned char error_code,int line_no);
//#define LEXER_DEBUG
//#define GRAMMAR_DEBUG
//#define PARSER_DEBUG
//#define INTERPRETER_DEBUG


#ifdef  __cplusplus
}
#endif

#endif