// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#ifndef ERROR_H
#define ERROR_H

#ifdef __cplusplus
extern "C" {
#endif


#define ANSI_COLOR_BLACK    "\033[1;30m" //000
#define ANSI_COLOR_RED      "\033[1;31m" //100
#define ANSI_COLOR_GREEN    "\033[1;32m" //010
#define ANSI_COLOR_YELLOW   "\033[1;33m" //011
#define ANSI_COLOR_BLUE     "\033[1;34m" //001
#define ANSI_COLOR_MAGENTA  "\033[1;35m" //101
#define ANSI_COLOR_CYAN     "\033[1;36m" //011
#define ANSI_COLOR_WHITE    "\033[1;37m" //111
#define ANSI_COLOR_RESET    "\033[0m" //0

#define ANSI_COLOR_RED_UNDERLINE    "\e[4;31m"

#define LOG_LEVEL_ERROR     0
#define LOG_LEVEL_WARNING 	1
#define LOG_LEVEL_DEBUG     2
#define LOG_LEVEL_VERBOSE   3

#ifdef LOG_LEVEL 
    #define LOG_LEVEL LOG_LEVEL_ERROR
#else
    #define LOG_LEVEL LOG_LEVEL_VERBOSE
#endif


#define FBGC_LOGFORMATSIMPLE(color,format) color "" format "" ANSI_COLOR_RESET 
#define FBGC_LOGFORMATDETAILED(color,format) color "%s/%s[%u]: " format "" ANSI_COLOR_RESET ,__FILE__,__FUNCTION__,  __LINE__

//These loge and logw are for compiler not for users or developers

#define LOGE(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_RED,format),##__VA_ARGS__)
#define _LOGE(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_RED,format),##__VA_ARGS__)

#define LOGW(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_YELLOW,format),##__VA_ARGS__)
#define _LOGW(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_YELLOW,format),##__VA_ARGS__)


#if LOG_LEVEL >= LOG_LEVEL_DEBUG
	#define LOGD(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_CYAN,format),##__VA_ARGS__)
	#define _LOGD(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_CYAN,format),##__VA_ARGS__)
#else
	#define LOGD(...)
	#define _LOGD(...)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
	#define LOGV(format, ...) printf(FBGC_LOGFORMATSIMPLE(ANSI_COLOR_WHITE,format),##__VA_ARGS__)
	#define _LOGV(format, ...) printf(FBGC_LOGFORMATDETAILED(ANSI_COLOR_WHITE,format),##__VA_ARGS__)
#else
	#define LOGV(...)
	#define _LOGV(...)
#endif	


#define FBGC_LOGE(format,...) LOGE(format,##__VA_ARGS__)
#define _FBGC_LOGE(format,...) _LOGE(format,##__VA_ARGS__)

#define FBGC_LOGW(format,...) LOGW(format,##__VA_ARGS__)
#define _FBGC_LOGW(format,...) _LOGW(format,##__VA_ARGS__)

#define FBGC_LOGD(f,format,...) f ## _ ##  LOGD(format,##__VA_ARGS__)
#define _FBGC_LOGD(f,format,...) _ ## f ## _ ## LOGD(format,##__VA_ARGS__)

#define FBGC_LOGV(f,format,...) f ## _  ##  LOGV(format,##__VA_ARGS__)
#define _FBGC_LOGV(f,format,...) _ ## f ## _ ## LOGV(format,##__VA_ARGS__)


#define _FBGC_NO_ERROR 0
#define _FBGC_LEXER_ERROR 1
#define _FBGC_SYNTAX_ERROR 2
#define _FBGC_UNDEFINED_IDENTIFIER_ERROR 3
#define _FBGC_DUMMY2_ERROR 4



#if defined CLOSE_PRINTF
	#define cprintf(...)
#else
	uint8_t cprintf(char color,const char *format, ...);
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