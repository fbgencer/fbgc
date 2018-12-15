#ifndef TOKENS_H
#define TOKENS_H


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
static const char * TYPES_ARRAY[];

static const token_struct operator_token_array[];


static const token_struct reserved_words_token_array[];

static const token_struct paranthesis_token_array[];

fbgc_token get_operator_code(const char *str);
fbgc_token get_reserved_word_code(const char *str);






#endif

