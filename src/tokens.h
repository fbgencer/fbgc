#ifndef TOKENS_H
#define TOKENS_H
#include <stdint.h>

typedef uint8_t fbgc_token;
typedef struct {
	fbgc_token tokencode;
	const char * name;
}token_struct;



enum TYPES{
	//PRIMITIVE DATA
	UNKNOWN=0, // always give zero for unknown
	BEGIN,END,END_LOOP,END_FUN,RESET,
	INT,DOUBLE,COMPLEX,HEX,INDEX_L,INDEX_S,
	WORD,
	VAR,
	FUN,CFUN,UFUN,
	STRING,
	OP,
	PLUS,UPLUS,MINUS,UMINUS,
	MULT,DIV,MOD,Q_MARK,POWER1,POWER2,INC,DECR,
	ASSIGN,PLUS_ASSIGN,MIN_ASSIGN,MULT_ASSIGN,DIV_ASSIGN,CONST_ASSIGN,
	OR_OP,AND_OP,
	GREATER,LOWER,//>
	LO_EQ,GR_EQ,EQ_EQ,NOT_EQ,IS_EQ,BIT_NOT_EQ,  // ?=
	ISE, // =>
	R_ARROW,L_ARROW,COL_LO,RW_ARROW,LW_ARROW,BIT_NOT,DOLLAR,
	BIT_AND_OP,BIT_OR_OP,
	COLMULT,COLDIV,

	DIVDIV,// //
	COLON,SEMICOLON, //;
	NOT_OP,
	RSHIFT,LSHIFT, //<<
	DOT,TWO_DOT,THREE_DOT,TWO_COLON,

	PARA,LPARA,RPARA,LBRACE,RBRACE, LBRACK,RBRACK,
	//Loops
	FOR,WHILE,UNTIL,JUMP,BREAK,CONT,
	//If-Else
	IF,ELSE,ELIF,
	//Logical
	AND,OR,NOT,
	//Some Special cases
	SPACE,TAB,NLINE, 
	EOS, // end of string
	RETURN,IS,DEL,ARG,COMMA,
	HALT
};

const char * TYPES_ARRAY[]={
	"UNKNOWN",
	"BEGIN","END","END_LOOP","END_FUN","RESET",
	"INT","DOUBLE","COMPLEX","HEX","INDEX_L","INDEX_S",
	"WORD",
	"VAR",
	"FUN","CFUN","UFUN",
	"STRING",
	"OP",
	"PLUS","UPLUS","MINUS","UMINUS",
	"MULT","DIV","MOD","Q_MARK","POWER1","POWER2","INC","DECR",
	"ASSIGN","PLUS_ASSIGN","MIN_ASSIGN","MULT_ASSIGN","DIV_ASSIGN","CONST_ASSIGN",
	"OR_OP","AND_OP",
	"GREATER","LOWER",//>
	"LO_EQ","GR_EQ","EQ_EQ","NOT_EQ","IS_EQ","BIT_NOT_EQ",  // ?=
	"ISE", // =>
	"R_ARROW","L_ARROW","COL_LO","RW_ARROW","LW_ARROW","BIT_NOT","DOLLAR",
	"BIT_AND_OP","BIT_OR_OP",
	"COLMULT","COLDIV",

	"DIVDIV",// //
	"COLON","SEMICOLON", //;
	"NOT_OP",
	"RSHIFT","LSHIFT",
	"DOT","TWO_DOT","THREE_DOT","TWO_COLON",

	"PARA","LPARA","RPARA","LBRACE","RBRACE", "LBRACK","RBRACK",
	//Loops
	"FOR","WHILE","UNTIL","JUMP","BREAK","CONT",
	//If-Else
	"IF","ELSE","ELIF",
	//Logical
	"AND","OR","NOT",
	//Some Special cases
	"SPACE", "TAB","NLINE", 
	"EOS", // end of string
	"RETURN","IS","DEL","ARG","COMMA",
	"HALT"
};

#define is_token_OP(T)(T>=OP && T< PARA)
#define is_token_NUM(T)(T>= INT && T<WORD) //INT,DOUBLE,COMPLEX
#define is_token_INT(T) (T == INT)
#define is_token_DOUBLE(T) (T == DOUBLE)
#define is_token_STRING(T) (T == STRING)
#define is_token_PARA(T)(T>=PARA && T<= RBRACK)
#define is_token_BINARY_OP(T)(T == PLUS || T == MINUS || T == MULT || T == DIV || T == POWER1 || T == POWER2 || T == DIVDIV \
|| T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ || T == EQ_EQ || T == IS_EQ || T == NOT_EQ|| T== AND_OP|| T == OR_OP )
#define is_token_UNARY_OP(T)(T ==  UMINUS || T == UPLUS || T == INC || T == DECR)

#define is_token_VAR(T)(T==VAR )
#define is_token_FUN(T)(T>=FUN && T<= UFUN)
#define is_token_WORD(T)(T==WORD)

#define is_token_BASIC_OPERATOR(T)(T == PLUS || T == MINUS || T == MULT || T == DIV || T == UMINUS\
|| T== UPLUS || T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ || T == EQ_EQ \
|| T == IS_EQ || T == NOT_EQ|| T== AND_OP|| T == OR_OP || T == NOT_OP)

#define isFUNCTIONABLE(T)(T == IF || T == ELIF || T == WHILE)
#define isCONDITION(T)(T == IF||T==ELIF||T == ELSE)
#define isLOOP(T) (T == WHILE || T == FOR || T == UNTIL)



static const token_struct operator_token_array[]={
	{ASSIGN,"="},
	{PLUS,"+"},
	{MINUS,"-"},
	{MULT,"*"},
	{DIV,"/"},
	{DOT,"."},
	{POWER1,"^"},
	{POWER2,"**"},
	{PLUS_ASSIGN,"+="},
	{MIN_ASSIGN,"-="},
	{MULT_ASSIGN,"*="},
	{DIV_ASSIGN,"/="},
	{CONST_ASSIGN,":="},
	{AND_OP,"&&"},
	{OR_OP,"||"},
	{BIT_AND_OP,"&"},
	{BIT_OR_OP,"|"},
	{LOWER,"<"},
	{GREATER,">"},
	{LO_EQ,"<="},
	{GR_EQ,">="},
	{EQ_EQ,"=="},
	{IS_EQ,"?="},
	{NOT_EQ,"!="},
	{BIT_NOT_EQ,"~="},
	{INC,"++"},
	{DECR,"--"},
	{ISE,"=>"},
	{R_ARROW,"->"},
	{L_ARROW,"<-"},
	{MOD,"%"},
	{DIVDIV,"//"},
	{NOT_OP,"!"},
	{COLON,":"},
	{SEMICOLON,";"},
	{RSHIFT,">>"},
	{LSHIFT,"<<"},
	{COL_LO,":>"},
	{RW_ARROW,"~>"},
	{LW_ARROW,"<~"},
	{BIT_NOT,"~"},
	{TWO_DOT,".."},
	{THREE_DOT,"..."},
	{TWO_COLON,"::"},
	{DOLLAR,"$"},
	{COLMULT,":*"},
	{COLDIV,":/"},

};

static const token_struct reserved_words_token_array[]={
	{IF,"if"},
	{ELSE,"else"},
	{ELIF,"elif"},
	{END,"end"},
	{BEGIN,"begin"},
	{FOR,"for"},
	{WHILE,"while"},
	{UNTIL,"until"},
	{VAR,"var"},
	{FUN,"fun"},
	{BREAK,"break"},
	{CONT,"cont"},
	{AND,"and"},
	{OR,"or"},
	{NOT,"not"},
	{RETURN,"ret"},
	{JUMP,"jump"},
	{IS,"is"},
	{DEL,"del"},
	{END_FUN,"endfun"}
};

static const token_struct paranthesis_token_array[]={
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"},
};

fbgc_token get_operator_code(const char *str){
  for( const token_struct * ptr = operator_token_array; ptr < ( operator_token_array + (sizeof(operator_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(str == ptr->name ) return ptr->tokencode;
  return 0;
}

fbgc_token get_reserved_word_code(const char *str){ 
  for( 
  	const token_struct * ptr = reserved_words_token_array; 
  	ptr < ( reserved_words_token_array + (sizeof(reserved_words_token_array)/sizeof(token_struct)) ); ptr++ ) 
  	if(str == ptr->name ) return ptr->tokencode;
  return 0;
}

#endif

