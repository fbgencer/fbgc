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
#define STRING 6
#define IDENTIFIER 7
#define WORD 8
#define CFUN 9
#define FUN 10
#define SUBSCRIPT 11
#define REFERENCE 12
#define LIST 13
#define RAW_MATRIX 14
#define MATRIX 15
#define NUPLE 16
#define MONUPLE 17
#define TUPLE 18
#define BUILD_TUPLE 19
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
#define LOOP 84
#define WHILE 85
#define UNTIL 86
#define JUMP 87
#define BREAK 88
#define CONT 89
#define IF 90
#define ELSE 91
#define ELIF 92
#define END 93
#define START 94
#define LOAD 95
#define FUN_CALL 96
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
#define TOKEN_LIST_AS_STRINGS()\
"UNKNOWN",\
"INT",\
"INT16",\
"INT2",\
"DOUBLE",\
"COMPLEX",\
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
"LOOP",\
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
"FUN_CALL",\
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

