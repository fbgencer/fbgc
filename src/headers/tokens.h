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
#define REFERENCE 13
#define LIST 14
#define MONATRIX 15
#define MATRIX 16
#define NUPLE 17
#define MONUPLE 18
#define TUPLE 19
#define LINKED_LIST 20
#define LINKED_LIST_TAIL 21
#define STACK 22
#define CMODULE 23
#define GARBAGE 24
#define FIELD 25
#define CSTRING 26
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
#define END 92
#define START 93
#define LOAD 94
#define AND 95
#define OR 96
#define NOT 97
#define SPACE 98
#define TAB 99
#define NEWLINE 100
#define RETURN 101
#define ROW 102
#define EXPRESSION 103
#define ASSIGNMENT 104
#define STATEMENT 105
#define ASSIGNMENT_EXPRESSION 106
#define UNARY_EXPRESSION 107
#define BINARY_EXPRESSION 108
#define BALANCED_EXPRESSION_LIST 109
#define UNBALANCED_EXPRESSION_LIST 110
#define IF_BEGIN 111
#define ELIF_BEGIN 112
#define WHILE_BEGIN 113
#define FOR_BEGIN 114
#define LOAD_LOCAL 115
#define LOCAL 116
#define GLOBAL 117
#define LOAD_GLOBAL 118
#define FUN_CALL 119
#define FUN_MAKE 120
#define SUBSCRIPT_CALL 121
#define BUILD_TUPLE 122
#define BUILD_MATRIX 123
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
"REFERENCE",\
"LIST",\
"MONATRIX",\
"MATRIX",\
"NUPLE",\
"MONUPLE",\
"TUPLE",\
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
"LOCAL",\
"GLOBAL",\
"LOAD_GLOBAL",\
"FUN_CALL",\
"FUN_MAKE",\
"SUBSCRIPT_CALL",\
"BUILD_TUPLE",\
"BUILD_MATRIX",\

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

#define is_fbgc_MATRIX(x)(x == MATRIX || x == BUILD_MATRIX)
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

