#ifndef RELEXER_H
#define RELEXER_H
#include "include.h"
#include "tokens.h"
#include "error.h"


/*
std::vector<ufbg_token> lextok;
std::vector<string> lexstr;
std::vector<string> lexword;
std::vector<int> lexint;
std::vector<double> lexdouble;
*/

typedef struct{
	ufbg_token token;
	const char *rule;
}ufbg_lexer_rule_struct;


/*
	d: digit [0-9]
	l: letter[a-zA-Z]
	o : operator
	h: hexadecimal [0-F]  
	$ : for only one character to see its real value, if you want to match l, do not put l, put $l
	* : at least one occurance
	z: all matching, any character, operator, number except \t \n
	+ : Since we don't have or operator, + is going to try match following immediately after previous one
	><: takes real strings inside it and tries to match, for example >ufbg< is going to match as exactly ufbg in the string

*/
/*

			d	.	j	\E \+\-*
Begin 	0	1	2	0	0
Int 	1	1	2	4	3
Double 	2	2	0	4	3
EXP_DB  3	3	0	4	0
Complex 4	0	0	0	0

*/
// how to initialize 2d array in struct ? 

typedef struct{
	const uint8_t input_size,token_size;
	const char ** inputs;
	const uint8_t * states;
	const ufbg_token * tokens;
}token_table_struct;

static token_table_struct table_0 = {
	.input_size = 4,
	.token_size = 4,
	.inputs = (const char*[]){"\\d",".","j","E \\+\\-*"},
	.states = (const uint8_t[])
	{ 1,2,5,0, //begin  
	  1,2,4,3, //int
	  2,0,4,3, //double
	  3,0,4,0, //exponential double
	  0,0,0,0, //complex
	},
	.tokens = (const ufbg_token[]){INT,DOUBLE,DOUBLE,COMPLEX}
};

static const token_table_struct lexer_table[] = {table_0};
#define TOKEN_TABLE_SIZE 1

//#define RULE_NUMBER 7
#define RULE_NUMBER sizeof(ufbg_lexer_rule_holder)/sizeof(ufbg_lexer_rule_struct)
static ufbg_lexer_rule_struct ufbg_lexer_rule_holder [] = 
{
	{INT,"0 b 01+"},
	{HEX,"0 x \\h\\d+"},
	{STRING,"' \\ \\.\\t\\n\\\"* '"},
	{WORD,"\\l_ \\l\\d_*"},
	{OP,"\\o+"},
	{INT,"[0]"}, //123.875	
};


/*


static ufbg_lexer_rule_struct ufbg_lexer_rule_holder [] = 
{
	{INT,"\\0 \\b \\0\\1+"},
	{HEX,"\\0 \\x hd+"},
	{STRING,"\\' .tn\\\"* \\'"},
	{WORD,"l\\_ ld\\_*"},
	{INC,"\\+ \\+"},
	{OP,"o+"},
	{INT,"[0]"}, //123.875

};
*/

#define RULE_NUMBER sizeof(ufbg_lexer_rule_holder)/sizeof(ufbg_lexer_rule_struct)

/*
	
	
	{"*d $. *d $j",COMPLEX},
	
	{"*d $j",COMPLEX}, //123j
	{"*d $f",DOUBLE}, //123j
	{"*d",INT},//12346
	{"l$_ *ld$_",WORD},
	{"*o",OP}, // ==, ++, -, *	
	{"$0 $b *$0$1",INT},	
	{"*d $. *d $j",COMPLEX}, //1234.56547j
	{"(4) $j",COMPLEX},	// .012345j		
	{">0b< *$0$1",INT},//0b0101 // binary	
	{">0x< *hd", INT}, // 0xAF
	{"l$_ *ld$_",WORD}, // abc,a12b,a1_,_1_,_2_,____,a_2_b
	{"*o",OP}, // ==, ++, -, *
	{"$. *d",DOUBLE},
	{"*d $E d$+$- *d", DOUBLE}, //5E8,5E+3,5E-3
	{"$\" *tnz $\"",STRING}, //"u\nf\nb\ng" z:
	{"$' *tnz $'",STRING},//'fbgc'
		
*/


#define fbgc_bool uint8_t

fbgc_bool is_digit(const char c){return (c>='0' &&c<= '9');}
fbgc_bool is_letter(const char c) {return ( (c>='a' && c<='z' ) || (c>='A' && c <='Z')); }
fbgc_bool is_operator(const char c){ return
    (  c == '+' || c == '-' || c == '*' || c == '/' || c == '='
	|| c == '^' || c == '?' || c == '<' || c == '>' || c == '%' || c == '~'
	|| c == '|' || c == ';' || c == '&' || c == ':' || c == '!' || c == '$');}

fbgc_bool is_hexadecimal(const char c) {return( c >= 'a' && c <= 'f' )|| ( c >= 'A' &&c <= 'F' );}
fbgc_bool is_all(const char c){ return ( c>=32 && c<=126 && c!='\'' && c!='"' );}
fbgc_bool is_newline(const char c){return c == '\n';}
fbgc_bool is_tab(const char c){return c == '\t';}


#define COMPARATOR_FUNCTION_SIZE 7

#define fun_index(x)({\
x == 'd' ? 0 : x == 'l' ? 1 : x == 'o' ? 2 : x == 'h' ? 3 : x == '.' ? 4 : x == 'n' ? 5 : x == 't' ? 6: 7;  })

#define LXR_DIGIT 0 //for digit:d
#define LXR_LETTER 1 //for letter:l
#define	LXR_OPERATOR 2 //for op : o
#define LXR_HEXADECIMAL 3
#define LXR_ALL 4
#define LXR_NEWLINE 5
#define LXR_TAB 6
#define LXR_PATTERN_NUM 7

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

#define LXR_META_PLUS '+'
#define LXR_META_STAR '*'
#define LXR_META_BACKSLASH '\\'
#define LXR_META_TABLE '['


#define LXR_META_PLUS_INDEX 0 // for + metach
#define LXR_META_STAR_INDEX 0 // for * metach
#define LXR_META_BACKSLASH_INDEX 1 //for \ metach
#define LXR_META_TABLE_INDEX 2

#define LXR_FLAGS_NUM 4 //it has to be equal last one + 1

#endif

