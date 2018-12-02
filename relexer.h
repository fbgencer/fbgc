#ifndef RELEXER_H
#define RELEXER_H
#include "include.h"
#include "tokens.h"
#include "error.h"


std::vector<ufbg_token> lextok;
std::vector<string> lexstr;
std::vector<string> lexword;
std::vector<int> lexint;
std::vector<double> lexdouble;

typedef struct{
	const char *rule;
	ufbg_token token;
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
/*	Table for numbers
		 |d|.|j|
-------------------------------
begin0 	 |1|2|4
int 1    |1|2|3|
double 2 |2|4|3|
complex3 |0|0|0|
*/


// how to initialize 2d array in struct ? 

typedef struct{
	const uint8_t size;
	const char ** inputs;
	const uint8_t * states;
	const ufbg_token * tokens;
}token_table_struct;

static token_table_struct table_0 = {
	.size = 3,
	.inputs = (const char*[]){"d","\\.","\\j"},
	.states = (const uint8_t[]){1,2,4,  1,2,3,  2,4,3,  0,0,0},
	.tokens = (const ufbg_token[]){INT,DOUBLE,COMPLEX}
};

static const token_table_struct lexer_table[] = {table_0};
#define TOKEN_TABLE_SIZE 1

#define RULE_NUMBER 5
static ufbg_lexer_rule_struct ufbg_lexer_rule_holder [] = 
{
	{"\\0 \\b \\0\\1+",INT},
	{"\\0 \\x hd+",HEX},
	{"l\\_ ld\\_*",WORD},
	{"o+",OP},
	{"[0]",INT}, //123.875
};
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
fbgc_bool is_letter(const char c) {return ( (c>='a' &&c<='z' ) || (c>='A' &&c<='Z')); }
fbgc_bool is_operator(const char c){ return
    (  c == '+' || c == '-' || c == '*' || c == '/' || c == '='
	|| c == '^' || c == '?' || c == '<' || c == '>' || c == '%' || c == '~'
	|| c == '|' || c == ';' || c == '&' || c == ':' || c == '!' || c == '$');}

fbgc_bool is_hexadecimal(const char c) {return( c >= 'a' && c <= 'f' )|| ( c >= 'A' &&c <= 'F' );}
fbgc_bool is_all(const char c){
	return ( (c>='0' &&c<= '9') || (c>='a' &&c<='z' ) || (c>='A' &&c<='Z') || is_operator(c) );
}

//make sure that str at least size 3
fbgc_bool is_two_chars_equal(const char c1, const char c2){ return c1==c2;}

#define COMPARATOR_FUNCTION_SIZE 6

#define fun_index(x)({x == 'd' ? 0 : x == 'l' ? 1 : x == 'o' ? 2 : x == 'h' ? 3 : x == 'z' ? 4 : 5;  })

#define LXR_DIGIT 0 //for digit:d
#define LXR_LETTER 1 //for letter:l
#define	LXR_OPERATOR 2 //for op : o
#define LXR_HEXADECIMAL 3
#define LXR_ALL 4
#define LXR_PATTERN_NUM 5

fbgc_bool (*comparator[COMPARATOR_FUNCTION_SIZE])(const char) = 
{
	is_digit ,
	is_letter,
	is_operator,
	is_hexadecimal,
	is_all
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

