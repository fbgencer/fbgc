#ifndef TOKENS_H
#define TOKENS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t fbgc_token;

typedef struct {
	fbgc_token tokencode;
	const char * name;
}token_struct;

#define UNKNOWN 0
#define ATOM 1
#define INT 2
#define INT16 3
#define INT2 4
#define DOUBLE 5
#define COMPLEX 6
#define STRING 7
#define IDENTIFIER 8
#define WORD 9
#define VAR 10
#define CFUN 11
#define FUN 12
#define SUBSCRIPT 13
#define REFERENCE 14
#define LIST 15
#define RAW_MATRIX 16
#define MATRIX 17
#define NUPLE 18
#define MONUPLE 19
#define RAW_TUPLE 20
#define TUPLE 21
#define LINKED_LIST 22
#define LINKED_LIST_TAIL 23
#define STACK 24
#define CMODULE 25
#define GARBAGE 26
#define OP 27
#define PLUS 28
#define MINUS 29
#define STAR 30
#define SLASH 31
#define MOD 32
#define STARSTAR 33
#define CARET 34
#define OR_OP 35
#define AND_OP 36
#define GREATER 37
#define LOWER 38
#define LO_EQ 39
#define GR_EQ 40
#define EQ_EQ 41
#define NOT_EQ 42
#define IS_EQ 43
#define IF_THEN 44
#define R_ARROW 45
#define L_ARROW 46
#define COL_LO 47
#define RW_ARROW 48
#define LW_ARROW 49
#define COLSTAR 50
#define COLSLASH 51
#define SLASHSLASH 52
#define COLON 53
#define RSHIFT 54
#define LSHIFT 55
#define ASSIGN 56
#define PLUS_ASSIGN 57
#define MINUS_ASSIGN 58
#define STAR_ASSIGN 59
#define SLASH_ASSIGN 60
#define CONST_ASSIGN 61
#define INC 62
#define DECR 63
#define UPLUS 64
#define UMINUS 65
#define NOT_OP 66
#define SEMICOLON 67
#define TWO_DOT 68
#define THREE_DOT 69
#define TWO_COLON 70
#define TILDE 71
#define COMMA 72
#define DOT 73
#define QUESTION_MARK 74
#define DOLLAR 75
#define PARA 76
#define LPARA 77
#define RPARA 78
#define LBRACE 79
#define RBRACE 80
#define LBRACK 81
#define RBRACK 82
#define FOR 83
#define WHILE 84
#define UNTIL 85
#define JUMP 86
#define BREAK 87
#define CONT 88
#define IF 89
#define ELSE 90
#define ELIF 91
#define IF_BEGIN 92
#define ELSE_BEGIN 93
#define BEGIN 94
#define END 95
#define START 96
#define FINISH 97
#define LOAD 98
#define AND 99
#define OR 100
#define NOT 101
#define SPACE 102
#define TAB 103
#define NEWLINE 104
#define RETURN 105
#define ROW 106
#define EXPRESSION 107
#define ASSIGNMENT 108
#define STATEMENT 109
#define ASSIGNMENT_EXPRESSION 110
#define UNARY_EXPRESSION 111
#define BINARY_EXPRESSION 112
#define BALANCED_EXPRESSION_LIST 113
#define UNBALANCED_EXPRESSION_LIST 114
#define TOKEN_LIST_AS_STRINGS()\
"UNKNOWN",\
"ATOM",\
"INT",\
"INT16",\
"INT2",\
"DOUBLE",\
"COMPLEX",\
"STRING",\
"IDENTIFIER",\
"WORD",\
"VAR",\
"CFUN",\
"FUN",\
"SUBSCRIPT",\
"REFERENCE",\
"LIST",\
"RAW_MATRIX",\
"MATRIX",\
"NUPLE",\
"MONUPLE",\
"RAW_TUPLE",\
"TUPLE",\
"LINKED_LIST",\
"LINKED_LIST_TAIL",\
"STACK",\
"CMODULE",\
"GARBAGE",\
"OP",\
"PLUS",\
"MINUS",\
"STAR",\
"SLASH",\
"MOD",\
"STARSTAR",\
"CARET",\
"OR_OP",\
"AND_OP",\
"GREATER",\
"LOWER",\
"LO_EQ",\
"GR_EQ",\
"EQ_EQ",\
"NOT_EQ",\
"IS_EQ",\
"IF_THEN",\
"R_ARROW",\
"L_ARROW",\
"COL_LO",\
"RW_ARROW",\
"LW_ARROW",\
"COLSTAR",\
"COLSLASH",\
"SLASHSLASH",\
"COLON",\
"RSHIFT",\
"LSHIFT",\
"ASSIGN",\
"PLUS_ASSIGN",\
"MINUS_ASSIGN",\
"STAR_ASSIGN",\
"SLASH_ASSIGN",\
"CONST_ASSIGN",\
"INC",\
"DECR",\
"UPLUS",\
"UMINUS",\
"NOT_OP",\
"SEMICOLON",\
"TWO_DOT",\
"THREE_DOT",\
"TWO_COLON",\
"TILDE",\
"COMMA",\
"DOT",\
"QUESTION_MARK",\
"DOLLAR",\
"PARA",\
"LPARA",\
"RPARA",\
"LBRACE",\
"RBRACE",\
"LBRACK",\
"RBRACK",\
"FOR",\
"WHILE",\
"UNTIL",\
"JUMP",\
"BREAK",\
"CONT",\
"IF",\
"ELSE",\
"ELIF",\
"IF_BEGIN",\
"ELSE_BEGIN",\
"BEGIN",\
"END",\
"START",\
"FINISH",\
"LOAD",\
"AND",\
"OR",\
"NOT",\
"SPACE",\
"TAB",\
"NEWLINE",\
"RETURN",\
"ROW",\
"EXPRESSION",\
"ASSIGNMENT",\
"STATEMENT",\
"ASSIGNMENT_EXPRESSION",\
"UNARY_EXPRESSION",\
"BINARY_EXPRESSION",\
"BALANCED_EXPRESSION_LIST",\
"UNBALANCED_EXPRESSION_LIST",\

extern const char * object_name_array[];
extern const token_struct operator_token_array[];
extern const token_struct reserved_words_token_array[];


fbgc_token get_operator_code(const char *str);
fbgc_token get_operator_code_from_substr(const char *str1,const char *str2);
fbgc_token get_reserved_word_code(const char *str);
fbgc_token get_reserved_word_code_from_substr(const char *str1,const char *str2);


const char * get_token_as_str(fbgc_token tok);


#define is_fbgc_OPERATOR(T)(T>=OP && T<= RBRACK)
#define is_fbgc_IDENTIFIER(T)(T>=IDENTIFIER && T<=REFERENCE)
#define is_fbgc_ATOM(T)(T>= ATOM && T<=STRING)
#define is_fbgc_INT(T) (T == INT)
#define is_fbgc_DOUBLE(T) (T == DOUBLE)
#define is_fbgc_STRING(T) (T == STRING)
#define is_fbgc_PARA(T)(T>=PARA && T<= RBRACK)
#define is_fbgc_BINARY_OPERATOR(T)(T>=PLUS && T<= LSHIFT)
#define is_fbgc_UNARY_OPERATOR(T)((T>=INC && T<=NOT_OP) )
#define is_fbgc_ASSIGNMENT_OPERATOR(T)(T>= ASSIGN && T<= CONST_ASSIGN)
#define is_fbgc_VAR(T)(T == VAR )
#define is_fbgc_FUN(T)(T == FUN)
#define is_fbgc_WORD(T)(T == WORD)

#define is_fbgc_MATRIX(x)(x == MATRIX || x == RAW_MATRIX)
#define is_fbgc_TUPLE(x)(x == NUPLE || x == MONUPLE  || x == RAW_TUPLE ||x == TUPLE)

#define is_fbgc_FUNCTIONABLE(x)(x == IF || x == ELIF || x == ELSE || x == WHILE)

#define is_fbgc_START(x)(x == START || x == BEGIN || x == IF_BEGIN)
#define is_fbgc_LIST(x) (x == LIST)
#define is_fbgc_EXPRESSION(x)(is_fbgc_ATOM(x) || is_fbgc_IDENTIFIER(x)|| x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_TUPLE(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x) || x == CFUN)

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x))



#ifdef  __cplusplus
}
#endif








#endif

