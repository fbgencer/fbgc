#include "fbgc.h"

void cprintf(char color,const char *format, ...) {


#define ANSI_COLOR_BLACK   "\033[1;30m" //000
#define ANSI_COLOR_RED     "\033[1;31m" //100
#define ANSI_COLOR_GREEN   "\033[1;32m" //010
#define ANSI_COLOR_YELLOW  "\033[1;33m" //011
#define ANSI_COLOR_BLUE    "\033[1;34m" //001
#define ANSI_COLOR_MAGENTA "\033[1;35m" //101
#define ANSI_COLOR_CYAN    "\033[1;36m" //011
#define ANSI_COLOR_WHITE   "\033[1;37m" //111
#define ANSI_COLOR_RESET   "\033[0m" //0


//#define CLOSE_PRINTF


    va_list args;
      
    va_start(args, format);
    #ifndef CLOSE_PRINTF
    if(color == 100) printf(ANSI_COLOR_RED); 
    else if(color == 010) printf(ANSI_COLOR_GREEN);
    else if(color == 001) printf(ANSI_COLOR_BLUE);
    else if(color == 110) printf(ANSI_COLOR_YELLOW);
    else if(color == 101) printf(ANSI_COLOR_MAGENTA);
    else if(color == 011) printf(ANSI_COLOR_CYAN);
    else if(color == 000) printf(ANSI_COLOR_BLACK);
    else if(color == 111) printf(ANSI_COLOR_WHITE);
    
    vprintf(format, args); printf("\033[0m");
    #endif
    va_end(args);


}

int error(const char *x){
    
      printf("\n[ERROR code]: %s\n",x);
      //printf("EVERYTHING WERE IMMEDIATELY STOPPED\n");
    return 0;
}
int error2(const char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    while (*fmt != '\0') {
        if (*fmt == 'd') {
            printf("%d", va_arg(args, int));
        } else if (*fmt == 'c') {
            printf("%c", va_arg(args, int));
        } else if (*fmt == 'f') {
            printf("%f", va_arg(args, double));
        }else if(*fmt == 'l'){
            printf("%ld",va_arg(args,long) );
        }else if(*fmt=='s'){
            printf("%s",va_arg(args,char*));
        }
        ++fmt;
    }
    
    va_end(args);
    return 0;
}
