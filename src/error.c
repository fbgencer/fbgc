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
#include "fbgc.h"


uint8_t cprintf(char color,const char *format, ...) {

#ifndef CLOSE_CPRINTF



//#define CLOSE_PRINTF

    #ifndef DEBUG_TO_FILE
    va_list args;
      
    va_start(args, format);
        

    if(color == 100) printf(ANSI_COLOR_RED); 
    else if(color == 010) printf(ANSI_COLOR_GREEN);
    else if(color == 001) printf(ANSI_COLOR_BLUE);
    else if(color == 110) printf(ANSI_COLOR_YELLOW);
    else if(color == 101) printf(ANSI_COLOR_MAGENTA);
    else if(color == 011) printf(ANSI_COLOR_CYAN);
    else if(color == 000) printf(ANSI_COLOR_BLACK);
    else if(color == 111) printf(ANSI_COLOR_WHITE);
    
    vprintf(format, args); printf("\033[0m");
    
    va_end(args);

    #else

    FILE * fl;
    fl = fopen ("terminal_debug.txt","a");
    
    va_list args;
    va_start(args, format);
    vfprintf(fl,format, args); 
    //vprintf(format, args); printf("\033[0m");
    
    va_end(args);    
    fclose(fl);    

    #endif

#endif

    return 1;
}




int fbgc_assert(unsigned char cond,const char * format,...){
    
    if(!cond){
    va_list args;
      
    va_start(args, format);
    
    printf(ANSI_COLOR_RED); 
    vprintf(format, args); 
    printf("\033[0m");
    
    va_end(args);
    assert(cond);
        //raise(SIGTERM);
    }
    return 1;
}


int fbgc_error(unsigned char error_code, int line_no){
    
    int state = 0;
    switch(error_code){
        case _FBGC_NO_ERROR:
        {
            state = 1;
            break;
        }
        case _FBGC_LEXER_ERROR:
        {
            cprintf(100,"Bad token\n");
            break;
        }
        case _FBGC_SYNTAX_ERROR:
        {
            cprintf(100,"Syntax error at line %d:",line_no);
            break;
        }
        case _FBGC_UNDEFINED_IDENTIFIER_ERROR:
        {
            cprintf(100,"Undefined identifier at line %d\n",line_no);
            break;
        }

        default:
        {
            cprintf(100,"Undefined error type\n");
            break;
        }

    }

    return state;
}


int fbgc_warning(unsigned char cond,const char * message){

    printf("%s\n",message );

    return 1;
}


