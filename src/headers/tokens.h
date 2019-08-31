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
#define DOUBLE 2
#define COMPLEX 3
#define STRING 4
#define IDENTIFIER 5
#define WORD 6
#define CFUN 7
#define FUN 8
#define REFERENCE 9
#define LIST 10
#define MONATRIX 11
#define MATRIX 12
#define NUPLE 13
#define MONUPLE 14
#define TUPLE 15
#define LINKED_LIST 16
#define LINKED_LIST_TAIL 17
#define STACK 18
#define CMODULE 19
#define GARBAGE 20
#define FIELD 21
#define CSTRING 22
#define THREE_DOT 23
#define STARSTAR 24
#define PLUSPLUS 25
#define MINUSMINUS 26
#define SLASHSLASH 27
#define LO_EQ 28
#define GR_EQ 29
#define EQ_EQ 30
#define NOT_EQ 31
#define PLUS_ASSIGN 32
#define MINUS_ASSIGN 33
#define STAR_ASSIGN 34
#define SLASH_ASSIGN 35
#define CONST_ASSIGN 36
#define IF_THEN 37
#define R_SHIFT 38
#define L_SHIFT 39
#define R_ARROW 40
#define L_ARROW 41
#define RW_ARROW 42
#define LW_ARROW 43
#define TWO_COLON 44
#define ASSIGN 45
#define SEMICOLON 46
#define COMMA 47
#define DOT 48
#define COLON 49
#define CARET 50
#define TILDE 51
#define PERCENT 52
#define EXCLAMATION 53
#define LOWER 54
#define GREATER 55
#define PIPE 56
#define AMPERSAND 57
#define SLASH 58
#define STAR 59
#define MINUS 60
#define PLUS 61
#define UMINUS 62
#define UPLUS 63
#define LPARA 64
#define RPARA 65
#define LBRACK 66
#define RBRACK 67
#define LBRACE 68
#define RBRACE 69
#define END 70
#define FUN_MAKE 71
#define IF 72
#define ELIF 73
#define ELSE 74
#define FOR 75
#define WHILE 76
#define BREAK 77
#define CONT 78
#define RETURN 79
#define LOAD 80
#define TRUE 81
#define FALSE 82
#define START 83
#define JUMP 84
#define AND 85
#define OR 86
#define NOT 87
#define SPACE 88
#define TAB 89
#define NEWLINE 90
#define ROW 91
#define EXPRESSION 92
#define ASSIGNMENT 93
#define STATEMENT 94
#define ASSIGNMENT_EXPRESSION 95
#define UNARY_EXPRESSION 96
#define BINARY_EXPRESSION 97
#define BALANCED_EXPRESSION_LIST 98
#define UNBALANCED_EXPRESSION_LIST 99
#define IF_BEGIN 100
#define ELIF_BEGIN 101
#define WHILE_BEGIN 102
#define FOR_BEGIN 103
#define LOAD_LOCAL 104
#define LOAD_GLOBAL 105
#define LOAD_SUBSCRIPT 106
#define ASSIGN_LOCAL 107
#define ASSIGN_GLOBAL 108
#define ASSIGN_SUBSCRIPT 109
#define FUN_CALL 110
#define BUILD_TUPLE 111
#define BUILD_MATRIX 112
#define TOKEN_LIST_AS_STRINGS()\
"UNKNOWN",\
"INT",\
"DOUBLE",\
"COMPLEX",\
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
"THREE_DOT",\
"STARSTAR",\
"PLUSPLUS",\
"MINUSMINUS",\
"SLASHSLASH",\
"LO_EQ",\
"GR_EQ",\
"EQ_EQ",\
"NOT_EQ",\
"PLUS_ASSIGN",\
"MINUS_ASSIGN",\
"STAR_ASSIGN",\
"SLASH_ASSIGN",\
"CONST_ASSIGN",\
"IF_THEN",\
"R_SHIFT",\
"L_SHIFT",\
"R_ARROW",\
"L_ARROW",\
"RW_ARROW",\
"LW_ARROW",\
"TWO_COLON",\
"ASSIGN",\
"SEMICOLON",\
"COMMA",\
"DOT",\
"COLON",\
"CARET",\
"TILDE",\
"PERCENT",\
"EXCLAMATION",\
"LOWER",\
"GREATER",\
"PIPE",\
"AMPERSAND",\
"SLASH",\
"STAR",\
"MINUS",\
"PLUS",\
"UMINUS",\
"UPLUS",\
"LPARA",\
"RPARA",\
"LBRACK",\
"RBRACK",\
"LBRACE",\
"RBRACE",\
"END",\
"FUN_MAKE",\
"IF",\
"ELIF",\
"ELSE",\
"FOR",\
"WHILE",\
"BREAK",\
"CONT",\
"RETURN",\
"LOAD",\
"TRUE",\
"FALSE",\
"START",\
"JUMP",\
"AND",\
"OR",\
"NOT",\
"SPACE",\
"TAB",\
"NEWLINE",\
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
"LOAD_SUBSCRIPT",\
"ASSIGN_LOCAL",\
"ASSIGN_GLOBAL",\
"ASSIGN_SUBSCRIPT",\
"FUN_CALL",\
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


#define is_fbgc_OPERATOR(T)(T>=THREE_DOT && T<= PLUS)
#define is_fbgc_IDENTIFIER(T)(T>=IDENTIFIER && T<=REFERENCE)
#define is_fbgc_ATOM(T)(T>= INT && T<=STRING)
#define is_fbgc_INT(T) (T == INT)
#define is_fbgc_DOUBLE(T) (T == DOUBLE)
#define is_fbgc_STRING(T) (T == STRING)
#define is_fbgc_PARA(T)(T>=LPARA && T<= RBRACK)
#define is_fbgc_BINARY_OPERATOR(T)(T>=PLUS && T<= L_SHIFT) //WRONG CHANGE 
#define is_fbgc_UNARY_OPERATOR(T)((T>=PLUSPLUS && T<=EXCLAMATION) ) //WRONG CHANGE THIS
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

