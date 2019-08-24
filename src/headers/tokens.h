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
#define INT 1
#define INT16 2
#define INT2 3
#define DOUBLE 4
#define COMPLEX 5
#define TRUE 6
#define FALSE 7
#define STRING 8
#define IDENTIFIER 9
#define WORD 10
#define CFUN 11
#define FUN 12
#define SUBSCRIPT 13
#define REFERENCE 14
#define LIST 15
#define RAW_MATRIX 16
#define MATRIX 17
#define NUPLE 18
#define MONUPLE 19
#define TUPLE 20
#define BUILD_TUPLE 21
#define LINKED_LIST 22
#define LINKED_LIST_TAIL 23
#define STACK 24
#define CMODULE 25
#define GARBAGE 26
#define FIELD 27
#define CSTRING 28
#define OP 29
#define PLUS 30
#define MINUS 31
#define STAR 32
#define SLASH 33
#define MOD 34
#define STARSTAR 35
#define CARET 36
#define OR_OP 37
#define AND_OP 38
#define GREATER 39
#define LOWER 40
#define LO_EQ 41
#define GR_EQ 42
#define EQ_EQ 43
#define NOT_EQ 44
#define IS_EQ 45
#define IF_THEN 46
#define R_ARROW 47
#define L_ARROW 48
#define COL_LO 49
#define RW_ARROW 50
#define LW_ARROW 51
#define COLSTAR 52
#define COLSLASH 53
#define SLASHSLASH 54
#define COLON 55
#define RSHIFT 56
#define LSHIFT 57
#define ASSIGN 58
#define PLUS_ASSIGN 59
#define MINUS_ASSIGN 60
#define STAR_ASSIGN 61
#define SLASH_ASSIGN 62
#define CONST_ASSIGN 63
#define INC 64
#define DECR 65
#define UPLUS 66
#define UMINUS 67
#define NOT_OP 68
#define SEMICOLON 69
#define TWO_DOT 70
#define THREE_DOT 71
#define TWO_COLON 72
#define TILDE 73
#define COMMA 74
#define DOT 75
#define QUESTION_MARK 76
#define DOLLAR 77
#define PARA 78
#define LPARA 79
#define RPARA 80
#define LBRACE 81
#define RBRACE 82
#define LBRACK 83
#define RBRACK 84
#define FOR 85
#define WHILE 86
#define UNTIL 87
#define JUMP 88
#define BREAK 89
#define CONT 90
#define IF 91
#define ELSE 92
#define ELIF 93
#define END 94
#define START 95
#define LOAD 96
#define AND 97
#define OR 98
#define NOT 99
#define SPACE 100
#define TAB 101
#define NEWLINE 102
#define RETURN 103
#define ROW 104
#define EXPRESSION 105
#define ASSIGNMENT 106
#define STATEMENT 107
#define ASSIGNMENT_EXPRESSION 108
#define UNARY_EXPRESSION 109
#define BINARY_EXPRESSION 110
#define BALANCED_EXPRESSION_LIST 111
#define UNBALANCED_EXPRESSION_LIST 112
#define IF_BEGIN 113
#define ELIF_BEGIN 114
#define WHILE_BEGIN 115
#define FOR_BEGIN 116
#define LOAD_LOCAL 117
#define LOAD_GLOBAL 118
#define FUN_CALL 119
#define FUN_MAKE 120
#define TOKEN_LIST_AS_STRINGS()\
"UNKNOWN",\
"INT",\
"INT16",\
"INT2",\
"DOUBLE",\
"COMPLEX",\
"TRUE",\
"FALSE",\
"STRING",\
"IDENTIFIER",\
"WORD",\
"CFUN",\
"FUN",\
"SUBSCRIPT",\
"REFERENCE",\
"LIST",\
"RAW_MATRIX",\
"MATRIX",\
"NUPLE",\
"MONUPLE",\
"TUPLE",\
"BUILD_TUPLE",\
"LINKED_LIST",\
"LINKED_LIST_TAIL",\
"STACK",\
"CMODULE",\
"GARBAGE",\
"FIELD",\
"CSTRING",\
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
"END",\
"START",\
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
"IF_BEGIN",\
"ELIF_BEGIN",\
"WHILE_BEGIN",\
"FOR_BEGIN",\
"LOAD_LOCAL",\
"LOAD_GLOBAL",\
"FUN_CALL",\
"FUN_MAKE",\

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
#define is_fbgc_ATOM(T)(T>= INT && T<=STRING)
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
#define is_fbgc_TUPLE(x)(x == NUPLE || x == MONUPLE ||x == BUILD_TUPLE || x == TUPLE)

#define is_fbgc_FUNCTIONABLE(x)(x == IF || x == ELIF || x == ELSE || x == WHILE || x == FUN || x == LOAD)

#define is_fbgc_LIST(x) (x == LIST)
#define is_fbgc_EXPRESSION(x)(is_fbgc_ATOM(x) || is_fbgc_IDENTIFIER(x)|| x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_TUPLE(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x) || x == CFUN || x == SEMICOLON)

#define is_fbgc_JUMP_STATEMENT(x)(x == BREAK || x == CONT)

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x) || is_fbgc_JUMP_STATEMENT(x))



#ifdef  __cplusplus
}
#endif








#endif

