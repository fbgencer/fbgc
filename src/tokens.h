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
	INT,DOUBLE,COMPLEX,HEX,BIN,INDEX_L,INDEX_S,
	WORD,
	VAR,
	FUN,CFUN,UFUN,
	STRING,
	LINKED_LIST,
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
	HALT,
	VALID,
	IDENTIFIER,
	NUMBER,
	EXPRESSION,
	CLOSED_EXPRESSION,
	COMP_EXPRESSION,
	CLOSED_COMP_EXPRESSION,
	ASSIGNMENT,
};
extern const char * object_name_array[];

static const token_struct operator_token_array[];


static const token_struct reserved_words_token_array[];

static const token_struct paranthesis_token_array[];

fbgc_token get_operator_code(const char *str);
fbgc_token get_operator_code_from_substr(const char *str1,const char *str2);
fbgc_token get_reserved_word_code(const char *str);
fbgc_token get_reserved_word_code_from_substr(const char *str1,const char *str2);


const char * get_token_as_str(fbgc_token tok);


#define is_fbgc_OPERATOR(T)(T>=OP && T< PARA)
#define is_fbgc_NUMBER(T)(T>= INT && T<WORD) //INT,DOUBLE,COMPLEX
#define is_fbgc_INT(T) (T == INT)
#define is_fbgc_DOUBLE(T) (T == DOUBLE)
#define is_fbgc_STRING(T) (T == STRING)
#define is_fbgc_PARA(T)(T>=PARA && T<= RBRACK)
#define is_fbgc_BINARY_OPERATOR(T)(T == PLUS || T == MINUS || T == MULT || T == DIV || T == POWER1 || T == POWER2 || T == DIVDIV \
|| T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ || T == EQ_EQ || T == IS_EQ || T == NOT_EQ|| T== AND_OP|| T == OR_OP )
#define is_fbgc_UNARY_OPERATOR(T)(T ==  UMINUS || T == UPLUS || T == INC || T == DECR)

#define is_fbgc_VAR(T)(T==VAR )
#define is_fbgc_FUN(T)(T>=FUN && T<= UFUN)
#define is_fbgc_WORD(T)(T==WORD)

#define is_fbgc_BASIC_OPERATOR(T)(T == PLUS || T == MINUS || T == MULT || T == DIV || T == UMINUS\
|| T== UPLUS || T == LOWER || T == GREATER || T == LO_EQ || T == GR_EQ || T == EQ_EQ \
|| T == IS_EQ || T == NOT_EQ|| T== AND_OP|| T == OR_OP || T == NOT_OP)

#define isFUNCTIONABLE(T)(T == IF || T == ELIF || T == WHILE)
#define isCONDITION(T)(T == IF||T==ELIF||T == ELSE)
#define isLOOP(T) (T == WHILE || T == FOR || T == UNTIL)




#endif

