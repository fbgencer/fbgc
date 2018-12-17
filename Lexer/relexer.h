#ifndef RELEXER_H
#define RELEXER_H


//#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tokens.h"
//#include "error.h"


/*
	d: digit [0-9]
	l: letter[a-zA-Z]
	o : operator
	h: hexadecimal [0-F]  
	$ : for only one character to see its real value, if you want to match l, do not put l, put $l
	* : at least one occurance
	z: all matching, any character, operator, number except \t \n
	+ : Since we don't have or operator, + is going to try match following immediately after previous one
	><: takes real strings inside it and tries to match, for example >fbgc< is going to match as exactly fbgc in the string

*/
/*

			d	.	j	\E \+\-*
Begin 	0	1	2	0	0
Int 	1	1	2	4	3
Double 	2	2	0	4	3
EXP_DB  3	3	0	4	0
Complex 4	0	0	0	0

*/

typedef struct{
	fbgc_token token;
	const char *rule;
}fbgc_lexer_rule_struct;


//==========================================================================

typedef struct{
	const uint8_t input_size,token_size;
	const char ** inputs;
	const uint8_t * states;
	const fbgc_token * tokens;
}token_table_struct;

token_table_struct lexer_table[] = 
{
	{
		.input_size = 4,
		.token_size = 4,
		.inputs = (const char*[]){"\\d",".","j","E \\+\\-\\*"},
		.states = (const uint8_t[])
		{ 1,2,5,5, //begin  
		  1,2,4,3, //int
		  2,0,4,3, //double
		  3,0,4,0, //exponential double
		  0,0,0,0, //complex
		},
		.tokens = (const fbgc_token[]){INT,DOUBLE,DOUBLE,COMPLEX}
	}	
};
#define TOKEN_TABLE_SIZE 1

//==========================================================================

#define RULE_NUMBER sizeof(fbgc_lexer_rule_holder)/sizeof(fbgc_lexer_rule_struct)
static const fbgc_lexer_rule_struct fbgc_lexer_rule_holder [] = 
{
	{BIN,"0b 0|1\\+"},
	{HEX,"0x \\h\\d\\+"},
	{OP,"\\o\\+"},
	{INT,"\\#0"}, //# for table		
	{STRING,"' \\ \\.\\t\\n\"\\* '"},
	{WORD,"_\\l _\\l\\d\\*"},	

};

/*


	
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"}
*/

#define RULE_NUMBER sizeof(fbgc_lexer_rule_holder)/sizeof(fbgc_lexer_rule_struct)

#define fbgc_bool uint8_t


//=======================[SPECIAL CHARACTERS]=====================



fbgc_bool is_digit(const char c){return (c>='0' && c<= '9');}
fbgc_bool is_letter(const char c) {return ( (c>='a' && c<='z' ) || (c>='A' && c <='Z')); }
fbgc_bool is_operator(const char c){ 
	return 
	(c>= 42 && c<= 47) // * + , - , /
	|| (c>= 35 && c<=38 ) // # $ % & 
	|| (c>= 58 && c<=64) // : ; < = > ? @ 
	|| c == '^' || c=='~' || c == '|'; // ^ ~ |
}
/*
old op definition
	(  c == '+' || c == '-' || c == '*' || c == '/' || c == '='
	|| c == '^' || c == '?' || c == '<' || c == '>' || c == '%' || c == '~'
	|| c == '|' || c == ';' || c == '&' || c == ':' || c == '!' || c == '$')

Use range to define operators
look at this table repsentation
from 35 to 38 is operator
from 42 to 64 is operator
33 : ! op
34 : " 
35 : # op
36 : $ op
37 : % op
38 : & op
39 : ' op
40 : (
41 : )
42 : * op
43 : + op
44 : , op
45 : - op
46 : . op
47 : / op

58 : : op
59 : ; op
60 : < op
61 : = op
62 : > op
63 : ? op
64 : @ op
94 : ^ op
124 : | op
126 : ~ op
*/


fbgc_bool is_hexadecimal(const char c) {return( c >= 'a' && c <= 'f' )|| ( c >= 'A' &&c <= 'F' );}
fbgc_bool is_all(const char c){ return ( c>=32 && c<=126 && c!='\'' && c!='"' );}
fbgc_bool is_newline(const char c){return c == '\n';}
fbgc_bool is_tab(const char c){return c == '\t';}


#define COMPARATOR_FUNCTION_SIZE 7

#define LXR_DIGIT 0 //for digit:d
#define LXR_LETTER 1 //for letter:l
#define	LXR_OPERATOR 2 //for op : o
#define LXR_HEXADECIMAL 3
#define LXR_ALL 4
#define LXR_NEWLINE 5
#define LXR_TAB 6
#define LXR_PATTERN_NUM 7

#define SET_PATTERN_FLAG_FROM_RULE(x)({\
x == 'd' ? 0x01 :\
x == 'l' ? 0x02 :\
x == 'o' ? 0x04 :\
x == 'h' ? 0x08 :\
x == '.' ? 0x10 :\
x == 'n' ? 0x20 :\
x == 't' ? 0x40 : 0;})

#define IS_SPECIAL_CHAR(x)(x == 'd' || x == 'l' || x == 'o' || x == 'h' || x == '.' || x == 'n' || x == 't')

#define GET_PATTERN_FUNCTION_INDEX(x)({\
x == 0x01 ? 0 :\
x == 0x02 ? 1 :\
x == 0x04 ? 2 :\
x == 0x08 ? 3 :\
x == 0x10 ? 4 :\
x == 0x20 ? 5 :\
x == 0x40 ? 6: 7;})

fbgc_bool (*comparator[COMPARATOR_FUNCTION_SIZE])(const char) = 
{
	is_digit , //0
	is_letter, //1
	is_operator,
	is_hexadecimal,
	is_all, //4
	is_newline, //5
	is_tab //6
};

fbgc_bool check_character_with_pattern(const char c, uint8_t pattern_flag){
	//first check the pattern flag
	fbgc_bool result = 0;
	uint8_t flag_controller = 0x01;
	while(!result && flag_controller != 0 && flag_controller<=pattern_flag){
		if(pattern_flag & flag_controller) {
			result = comparator[GET_PATTERN_FUNCTION_INDEX(flag_controller)](c);
		}
		flag_controller <<= 1;
	}
	return result;
}


//============================[METACHARACTERS]==========================

#define LXR_META_PLUS '+'
#define LXR_META_STAR '*'
#define LXR_META_BACKSLASH '\\'
#define LXR_META_TABLE '#'
#define LXR_META_OR '|'

#define LXR_META_PLUS_INDEX 0 // for + metach
#define LXR_META_STAR_INDEX 1 // for * metach
#define LXR_META_BACKSLASH_INDEX 2 //for \ metach
#define LXR_META_TABLE_INDEX 3
#define LXR_META_OR_INDEX 4

#define LXR_FLAGS_NUM 5 //it has to be equal last one + 1

#define IS_METACHAR(x)(x == '+' || x == '*' || x == '#' || x == '\\' || x == '|' || x == ' ')

#define SET_METACHARACTER_FLAG_FROM_RULE(x)({\
x == '+' ? 0x01:\
x == '*' ? 0x02:\
x == '\\' ? 0x04:\
x == '#' ? 0x08:0;})


#define IS_METACHAR_PLUS_OPEN(x)(x & 0x01)
#define IS_METACHAR_STAR_OPEN(x)(x & 0x02)
#define IS_METACHAR_BACKSLASH_OPEN(x)(x & 0x04)
#define IS_METACHAR_TABLE_OPEN(x)(x & 0x08)

#define SET_METACHAR_PLUS_FLAG_CLOSE(x)(x &= 0b11111110) // for plus flag is 0x01
#define SET_METACHAR_PLUS_FLAG_OPEN(x) (x |=  0b00000001)

#define SET_METACHAR_STAR_FLAG_CLOSE(x)(x &= 0b11111101)// for star flag is 0x02
#define SET_METACHAR_STAR_FLAG_OPEN(x)( x |= 0b00000010)

#define SET_METACHAR_BACKSLASH_FLAG_CLOSE(x)(x &= 0b11111011) // for backslash flag is 0x04
#define SET_METACHAR_BACKSLASH_FLAG_OPEN(x)( x |= 0b00000100)

#define SET_METACHAR_TABLE_FLAG_CLOSE(x)(x &= 0b11110111) //// for table flag is 0x08
#define SET_METACHAR_TABLE_FLAG_OPEN(x)( x |= 0b00001000)



#endif

/*
ASCII TABLE
32 :  
33 : !
34 : "
35 : #
36 : $
37 : %
38 : &
39 : '
40 : (
41 : )
42 : *
43 : +
44 : ,
45 : -
46 : .
47 : /
48 : 0
49 : 1
50 : 2
51 : 3
52 : 4
53 : 5
54 : 6
55 : 7
56 : 8
57 : 9
58 : :
59 : ;
60 : <
61 : =
62 : >
63 : ?
64 : @
65 : A
66 : B
67 : C
68 : D
69 : E
70 : F
71 : G
72 : H
73 : I
74 : J
75 : K
76 : L
77 : M
78 : N
79 : O
80 : P
81 : Q
82 : R
83 : S
84 : T
85 : U
86 : V
87 : W
88 : X
89 : Y
90 : Z
91 : [
92 : \
93 : ]
94 : ^
95 : _
96 : `
97 : a
98 : b
99 : c
100 : d
101 : e
102 : f
103 : g
104 : h
105 : i
106 : j
107 : k
108 : l
109 : m
110 : n
111 : o
112 : p
113 : q
114 : r
115 : s
116 : t
117 : u
118 : v
119 : w
120 : x
121 : y
122 : z
123 : {
124 : |
125 : }
126 : ~


*/