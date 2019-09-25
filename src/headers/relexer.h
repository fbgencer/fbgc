#ifndef RELEXER_H
#define RELEXER_H

#ifdef __cplusplus
extern "C" {
#endif



#define lexer_token uint8_t

#define LEXER_TOK_COMMENT 0
#define LEXER_TOK_SPACE 1
#define LEXER_TOK_NEWLINE 2
#define LEXER_TOK_BASE2_INT 3
#define LEXER_TOK_BASE10_INT 4
#define LEXER_TOK_BASE16_INT 5
#define LEXER_TOK_DOUBLE 6
#define LEXER_TOK_COMPLEX 7
#define LEXER_TOK_STRING 8
#define LEXER_TOK_OP0 9
#define LEXER_TOK_OP1 10
#define LEXER_TOK_OP2 11	
#define LEXER_TOK_PARA 12
#define LEXER_TOK_KEYWORDS 13	
#define LEXER_TOK_NAME 14		
#define LEXER_TOK_AS_STRINGS()\
"LEXER_TOK_COMMENT",\
"LEXER_TOK_SPACE",\
"LEXER_TOK_NEWLINE",\
"LEXER_TOK_BASE2_INT",\
"LEXER_TOK_BASE10_INT",\
"LEXER_TOK_BASE16_INT",\
"LEXER_TOK_DOUBLE",\
"LEXER_TOK_COMPLEX",\
"LEXER_TOK_STRING",\
"LEXER_TOK_OP0",\
"LEXER_TOK_OP1",\
"LEXER_TOK_OP2",\
"LEXER_TOK_PARA",\
"LEXER_TOK_KEYWORDS",\
"LEXER_TOK_NAME"


/*
	{COMMENT,":>  !.!* \n"},
	{NEWLINE,"\n"},
	{SPACE,"! |\t!+"},
	{INT2,"0b 1|0!+"},
	{INT16,"0x !x!+"},
	{STRING,"' _|\\'!s!d!w!o!* '"},
	//{DOUBLE,"!d!+ . !d!+ E +|-!* !d!+"}, 	//just experimental, it works..
	{COMPLEX,"!d!+ . !d!+ j"},
	{COMPLEX,"!d!+ j"}, 
	{DOUBLE,". !d!+"}, 
	{DOUBLE,"!d!+ . !d!+"}, 
	{INT,"!d!+"},
	{OP,"(|)|[|]|{|}"},	
	{OP,"+|-|.|**|^|*|/|<|>|=>|<=|=|,|;|:"},	
	{WORD,"_!w _!w!d!*"},	

*/
typedef struct{
	const char * char_match_begin;
	uint8_t char_match_end;
	uint8_t pattern_flag;
	uint8_t metachar_flag;
	uint8_t table_index;
}rule_flag_struct;

typedef struct{
	const uint8_t input_size,token_size;
	const char ** inputs;
	const uint8_t * states;
	const fbgc_token * tokens;
}token_table_struct;

typedef struct{
	fbgc_token token;
	const char *rule;
}fbgc_lexer_rule_struct;


typedef struct{
	int start;
	int length;
}match_where;


//==========================================================================

extern const token_table_struct lexer_table[1];
#define TOKEN_TABLE_SIZE sizeof(lexer_table)/sizeof(token_table_struct)

//==========================================================================


extern const fbgc_lexer_rule_struct fbgc_lexer_rule_holder[];
#define RULE_NUMBER sizeof(fbgc_lexer_rule_holder)/sizeof(fbgc_lexer_rule_struct)


char * fbgc_getline_from_file(char * s, int n, FILE *fp);
void pretty_print_pointer(const char *buffer ,const char * ptr);
//static const char * rule_reader(rule_flag_struct * rfs,const char * rule_ptr);
//static uint8_t check_char(rule_flag_struct *rfs,char ** buffer_ptr);
uint8_t regex_lexer(struct fbgc_object **head_obj,char *first_ptr);

//struct fbgc_object * tokenize_substr(const char *str1, const char*str2, lexer_token token, uint8_t where);


struct
fbgc_object * tokenize_substr(const char *str1, const char*str2, lexer_token token, uint8_t where);

uint8_t match(match_where *mw, const char * inc_rule_ptr, const char * buffer);
uint8_t match_and_replace(const char * inc_rule_ptr,char buffer[],const char *replace_str);




/*
	const char *str = "The quick brown fox jumps over the lazy wolf";
	match_where mw;
	int q = match(&mw,"f !. x",str);
	if(q){
		cprintf(111,"Match  at : %d , length %d\n",mw.start,mw.length);
		cprintf(111,"%s\n",str);
		int i = mw.start;
		while((i--)>0) printf(" ");
	
		int q = mw.length;
		while((q--)>0) printf("^");
		printf("\n");		
	}
	else {
		cprintf(111,"No match\n");

	}
*/
	

	//bsr_fbg@hotmail.com
	//example mail {ARG,"_|+|-|.\\l\\d\\+ @ -\\l\\d\\+ . co m\\*"},

/*
	{SPACE,"!+!s"},
	{BIN,"0b 1|0!+"},
	{HEX,"0x !x!+"},
	{STRING,"' _!s!d!w!* '"},
	{COMPLEX,"!d!+ . !d!+ j"},
	{COMPLEX,"!d!+ j"}, 
	{DOUBLE,". !d!+"}, 
	{DOUBLE,"!d!+ . !d!+"}, 
	{INT,"!d!+"}, 	
	{OP,"!o!+"},
	//# for table
	{WORD,"_!w _!w!d!*"},
	
	\"(\\.|[^"\\])*\"
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"}
*/


#define fbgc_bool uint8_t


//=======================[SPECIAL CHARACTERS]=====================

#define COMPARATOR_FUNCTION_SIZE 8

#define LXR_DIGIT 0 //for digit:d
#define LXR_LETTER 1 //for letter:w
#define	LXR_OPERATOR 2 //for op : o see ispunct
#define LXR_HEXADECIMAL 3 // for hexadecimals, see isxdigit
#define LXR_ALL 4 // see isprint() function
#define LXR_NEWLINE 5
#define LXR_TAB 6
#define LXR_PATTERN_NUM 7

#define SET_PATTERN_FLAG_FROM_RULE(x)({\
x == 'd' ? 0x01 :\
x == 'w' ? 0x02 :\
x == 'o' ? 0x04 :\
x == 'x' ? 0x08 :\
x == '.' ? 0x10 :\
x == 's' ? 0x20 :\
x == ' ' ? 0x40 :0;}) 

/*
x == 'D' ? ~0x01:\
x == 'W' ? ~0x02:\
x == 'O' ? ~0x04:\
x == 'X' ? ~0x08:\
x == 'S' ? ~0x40:\
*/

#define IS_SPECIAL_CHAR(x)(x == 'd' || x == 'w' || x == 'o' || x == 'x' || x == '.' || x == 's' || x == ' ')



//============================[METACHARACTERS]==========================

#define LXR_META_PLUS '+'
#define LXR_META_STAR '*'
#define LXR_META_ESCAPE '!'
#define LXR_META_TABLE '#'
#define LXR_META_OR '|'
#define LXR_META_NOT '^'

#define LXR_META_PLUS_INDEX 0 // for + metach
#define LXR_META_STAR_INDEX 1 // for * metach
#define LXR_META_BACKSLASH_INDEX 2 //for \ metach
#define LXR_META_TABLE_INDEX 3
#define LXR_META_OR_INDEX 4

#define LXR_FLAGS_NUM 5 //it has to be equal last one + 1

#define IS_METACHAR(x)(x == '+' || x == '*' || x == '#' || x == '^')

#define SET_METACHARACTER_FLAG_FROM_RULE(x)({\
x == '+' ? 0x01:\
x == '*' ? 0x02:\
x == '#' ? 0x04:0;})


#define IS_METACHAR_PLUS_OPEN(x)(x & 0x01)
#define IS_METACHAR_STAR_OPEN(x)(x & 0x02)
#define IS_METACHAR_TABLE_OPEN(x)(x & 0x04)
//#define IS_METACHAR_TABLE_OPEN(x)(x & 0x08)

#define SET_METACHAR_PLUS_FLAG_CLOSE(x)(x &= 0b11111110) // for plus flag is 0x01
#define SET_METACHAR_PLUS_FLAG_OPEN(x) (x |=  0b00000001)

#define SET_METACHAR_STAR_FLAG_CLOSE(x)(x &= 0b11111101)// for star flag is 0x02
#define SET_METACHAR_STAR_FLAG_OPEN(x)( x |= 0b00000010)

#define SET_METACHAR_TABLE_FLAG_CLOSE(x)(x &= 0b11111011) // for backslash flag is 0x04
#define SET_METACHAR_TABLE_FLAG_OPEN(x)( x |= 0b00000100)

/*
#define SET_METACHAR_TABLE_FLAG_CLOSE(x)(x &= 0b11110111) //// for table flag is 0x08
#define SET_METACHAR_TABLE_FLAG_OPEN(x)( x |= 0b00001000)
*/

#ifdef  __cplusplus
}
#endif

#endif