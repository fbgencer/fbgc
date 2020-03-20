#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


#define _FBGC_NO_ERROR 0
#define _FBGC_LEXER_ERROR 1
#define _FBGC_SYNTAX_ERROR 2
#define _FBGC_UNDEFINED_IDENTIFIER_ERROR 3
#define _FBGC_DUMMY2_ERROR 4


//#ifdef INFO_PRINT_AVAILABLE
#define _info(s,...)(cprintf(111,s,##__VA_ARGS__))
#define _info_green(s,...)(cprintf(010,s,##__VA_ARGS__))
#define _warning(s,...)(cprintf(110,s,##__VA_ARGS__))
#define _print(s,...)(cprintf(101,s,##__VA_ARGS__))
#define _cprint(c,s,...)(cprintf(c,s,##__VA_ARGS__))
#define _debug(s,...)(cprintf(011,s,##__VA_ARGS__))
#define _print_object(s,obj)(cprintf(001,s), print_fbgc_object(obj))
#define _println_object(s,obj)(cprintf(001,s), print_fbgc_object(obj),cprintf(001,"\n"))
#define _obj2str(obj)(object_name_array[obj->type])
#define _ll2str(obj)(object_name_array[obj->type])
#define _gm2str(gm)(gm_name_array[gm])

//#endif

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