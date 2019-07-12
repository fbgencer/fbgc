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
#define FIELD 27
#define OP 28
#define PLUS 29
#define MINUS 30
#define STAR 31
#define SLASH 32
#define MOD 33
#define STARSTAR 34
#define CARET 35
#define OR_OP 36
#define AND_OP 37
#define GREATER 38
#define LOWER 39
#define LO_EQ 40
#define GR_EQ 41
#define EQ_EQ 42
#define NOT_EQ 43
#define IS_EQ 44
#define IF_THEN 45
#define R_ARROW 46
#define L_ARROW 47
#define COL_LO 48
#define RW_ARROW 49
#define LW_ARROW 50
#define COLSTAR 51
#define COLSLASH 52
#define SLASHSLASH 53
#define COLON 54
#define RSHIFT 55
#define LSHIFT 56
#define ASSIGN 57
#define PLUS_ASSIGN 58
#define MINUS_ASSIGN 59
#define STAR_ASSIGN 60
#define SLASH_ASSIGN 61
#define CONST_ASSIGN 62
#define INC 63
#define DECR 64
#define UPLUS 65
#define UMINUS 66
#define NOT_OP 67
#define SEMICOLON 68
#define TWO_DOT 69
#define THREE_DOT 70
#define TWO_COLON 71
#define TILDE 72
#define COMMA 73
#define DOT 74
#define QUESTION_MARK 75
#define DOLLAR 76
#define PARA 77
#define LPARA 78
#define RPARA 79
#define LBRACE 80
#define RBRACE 81
#define LBRACK 82
#define RBRACK 83
#define FOR 84
#define WHILE 85
#define UNTIL 86
#define JUMP 87
#define BREAK 88
#define CONT 89
#define IF 90
#define ELSE 91
#define ELIF 92
#define IF_BEGIN 93
#define ELSE_BEGIN 94
#define BEGIN 95
#define END 96
#define START 97
#define FINISH 98
#define LOAD 99
#define AND 100
#define OR 101
#define NOT 102
#define SPACE 103
#define TAB 104
#define NEWLINE 105
#define RETURN 106
#define ROW 107
#define EXPRESSION 108
#define ASSIGNMENT 109
#define STATEMENT 110
#define ASSIGNMENT_EXPRESSION 111
#define UNARY_EXPRESSION 112
#define BINARY_EXPRESSION 113
#define BALANCED_EXPRESSION_LIST 114
#define UNBALANCED_EXPRESSION_LIST 115
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
"FIELD",\
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

