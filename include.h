#ifndef INCLUDE_H
#define INCLUDE_H
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstdint>
#include <cmath>
#include <cstring>

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <iterator>

using namespace std;

#define ufbg_token unsigned char


//printf("%d %d %d %d \n",PLUS,MINUS,MULT,DIV );
//24,26,28,29
/*#define OPO(T) ({\
	int a = T;\
	c##a\
})
#define c24 + 
#define c26 -
#define c28 * 
#define c29 /-*
/*
#define DO(a1,a2,F)({\
	int a = a1,b=a2;
	a1 F a2;\
})
#define fbgop(F)({\
	int a = 2,b=3;\
	printf("%d\n",a F b);\
})*/


//#define bool unsigned char
//#define byte unsigned char
#define true 1
#define false 0


//#define int(s) (atoi(s))
//#define long(s) (atol(s))
#define long_16(s) (strtol(s,NULL,16))
#define arlen(n,T) (sizeof(n)/sizeof(T))
//#define new(T,s) (malloc(sizeof(T)*s))
//#define renew(p,T,s)(realloc(p,sizeof(T)*s))

#define allowletter(x)(x == 'f' || x == 'k')

//typedef char* string;

#endif


/*
if(*ps == '_' || isalpha(ps)){
	while(*ps == '_' || isalpha(*ps) || isdigit(*ps)){
		push_character();
		next();
	}
	check_current_token_type();

}
*/