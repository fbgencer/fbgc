#ifndef TOKENS_H
#define TOKENS_H
#include "include.h"

//uFBG token struct takes token name and tokencode

typedef struct {
	fbgtoken tokencode;
	string name;
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
	/*Loops*/
	FOR,WHILE,UNTIL,JUMP,BREAK,CONT,
	/*If-Else*/
	IF,ELSE,ELSIF,
	/*Logical*/
	AND,OR,NOT,
	//Some Special cases
	SPACE,TAB,NLINE, 
	EOS, // end of string
	RETURN,IS,DEL,ARG,COMMA,
	HALT
};
const string TYPES_ARRAY[]={
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
	/*Loops*/
	"FOR","WHILE","UNTIL","JUMP","BREAK","CONT",
	/*If-Else*/
	"IF","ELSE","ELSIF",
	/*Logical*/
	"AND","OR","NOT",
	//Some Special cases
	"SPACE", "TAB","NLINE", 
	"EOS", // end of string
	"RETURN","IS","DEL","ARG","COMMA",
	"HALT"
};

#define isOP(T)(T>=OP && T< PARA)
#define isNUM(T)(T>= INT && T<WORD) //INT,DOUBLE,COMPLEX
#define isINT(T) (T == INT)
#define isDOUBLE(T) (T == DOUBLE)
#define isSTR(T) (T == STRING)
#define isPARA(T)(T>=PARA && T<= RBRACK)
#define isBINARY(T)(T == PLUS || T == MINUS || T == MULT || T == DIV || T == POWER1 || T == POWER2 || T == DIVDIV \
|| T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ || T == EQ_EQ || T == IS_EQ || T == NOT_EQ|| T== AND_OP|| T == OR_OP )
#define isUNARY(T)(T ==  UMINUS || T == UPLUS || T == INC || T == DECR)

#define isVAR(T)(T==VAR )
#define isFUN(T)(T>=FUN && T<= UFUN)
#define isWORD(T)(T==WORD)
#define isBASIC_OPERATOR(T)(T == PLUS || T == MINUS || T == MULT || T == DIV || T == UMINUS\
|| T== UPLUS || T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ || T == EQ_EQ \
|| T == IS_EQ || T == NOT_EQ|| T== AND_OP|| T == OR_OP || T == NOT_OP)

#define isFUNCTIONABLE(T)(T == IF || T == ELSIF || T == WHILE)
#define isCONDITION(T)(T == IF||T==ELSIF||T == ELSE)
#define isLOOP(T) (T == WHILE || T == FOR || T == UNTIL)
/*
fbgtoken get_which_token(const fbgtoken T){
	if(T>= NUM && T<WORD) return NUM;
	else if(T>=VAR && T<= SVAR) return VAR;
	else if(T>=FUN && T<= SFUN) return FUN;
	else if(T>=OP && T< PARA) return OP;
	else if(T>=PARA && T<= RBRACK) return PARA;
	else return T;
}*/


const static token_struct ops[]={
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

const static token_struct res_words[]={
	{IF,"if"},
	{ELSE,"else"},
	{ELSIF,"elsif"},
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

const static token_struct paras[]={
	{LPARA,"("},
	{RPARA,")"},
	{LBRACE,"{"},
	{RBRACE,"}"},
	{LBRACK,"["},
	{RBRACK,"]"},
};


#endif

