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
#define FUN 11
#define REFERENCE 12
#define LIST 13
#define RAW_MATRIX 14
#define MATRIX 15
#define NUPLE 16
#define MONUPLE 17
#define RAW_TUPLE 18
#define TUPLE 19
#define LINKED_LIST 20
#define LINKED_LIST_TAIL 21
#define STACK 22
#define OP 23
#define PLUS 24
#define MINUS 25
#define STAR 26
#define SLASH 27
#define MOD 28
#define STARSTAR 29
#define CARET 30
#define OR_OP 31
#define AND_OP 32
#define GREATER 33
#define LOWER 34
#define LO_EQ 35
#define GR_EQ 36
#define EQ_EQ 37
#define NOT_EQ 38
#define IS_EQ 39
#define IF_THEN 40
#define R_ARROW 41
#define L_ARROW 42
#define COL_LO 43
#define RW_ARROW 44
#define LW_ARROW 45
#define COLSTAR 46
#define COLSLASH 47
#define SLASHSLASH 48
#define COLON 49
#define RSHIFT 50
#define LSHIFT 51
#define ASSIGN 52
#define PLUS_ASSIGN 53
#define MINUS_ASSIGN 54
#define STAR_ASSIGN 55
#define SLASH_ASSIGN 56
#define CONST_ASSIGN 57
#define INC 58
#define DECR 59
#define UPLUS 60
#define UMINUS 61
#define NOT_OP 62
#define SEMICOLON 63
#define TWO_DOT 64
#define THREE_DOT 65
#define TWO_COLON 66
#define TILDE 67
#define COMMA 68
#define DOT 69
#define QUESTION_MARK 70
#define DOLLAR 71
#define PARA 72
#define LPARA 73
#define RPARA 74
#define LBRACE 75
#define RBRACE 76
#define LBRACK 77
#define RBRACK 78
#define FOR 79
#define WHILE 80
#define UNTIL 81
#define JUMP 82
#define BREAK 83
#define CONT 84
#define IF 85
#define ELSE 86
#define ELIF 87
#define IF_BEGIN 88
#define ELSE_BEGIN 89
#define BEGIN 90
#define END 91
#define START 92
#define FINISH 93
#define LOAD 94
#define AND 95
#define OR 96
#define NOT 97
#define SPACE 98
#define TAB 99
#define NEWLINE 100
#define RETURN 101
#define EXPRESSION 102
#define ASSIGNMENT 103
#define STATEMENT 104
#define ASSIGNMENT_EXPRESSION 105
#define UNARY_EXPRESSION 106
#define BINARY_EXPRESSION 107
#define BALANCED_EXPRESSION_LIST 108
#define UNBALANCED_EXPRESSION_LIST 109




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

#define is_fbgc_MATRIX(x)(x == MATRIX)
#define is_fbgc_TUPLE(x)(x == NUPLE || x == MONUPLE  || x == RAW_TUPLE ||x == TUPLE)

#define is_fbgc_FUNCTIONABLE(x)(x == IF || x == ELIF || x == ELSE || x == WHILE)

#define is_fbgc_START(x)(x == START || x == BEGIN || x == IF_BEGIN)
#define is_fbgc_LIST(x) (x == LIST)
#define is_fbgc_EXPRESSION(x)(is_fbgc_ATOM(x) || x == IDENTIFIER || x == REFERENCE || x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_TUPLE(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x))

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x))



#ifdef  __cplusplus
}
#endif








#endif

