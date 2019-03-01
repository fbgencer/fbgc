#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined CLOSE_PRINTF
#define cprintf(...)
#else
void cprintf(char color,const char *format, ...);
#endif

//#define LEXER_DEBUG
//#define GRAMMAR_DEBUG
//#define PARSER_DEBUG
//#define INTERPRETER_DEBUG


#ifdef  __cplusplus
}
#endif

#endif