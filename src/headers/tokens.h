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
#define REFERENCE 13
#define LIST 14
#define RAW_MATRIX 15
#define MATRIX 16
#define NUPLE 17
#define MONUPLE 18
#define RAW_TUPLE 19
#define TUPLE 20
#define LINKED_LIST 21
#define LINKED_LIST_TAIL 22
#define STACK 23
#define OP 24
#define PLUS 25
#define MINUS 26
#define STAR 27
#define SLASH 28
#define MOD 29
#define STARSTAR 30
#define CARET 31
#define OR_OP 32
#define AND_OP 33
#define GREATER 34
#define LOWER 35
#define LO_EQ 36
#define GR_EQ 37
#define EQ_EQ 38
#define NOT_EQ 39
#define IS_EQ 40
#define IF_THEN 41
#define R_ARROW 42
#define L_ARROW 43
#define COL_LO 44
#define RW_ARROW 45
#define LW_ARROW 46
#define COLSTAR 47
#define COLSLASH 48
#define SLASHSLASH 49
#define COLON 50
#define RSHIFT 51
#define LSHIFT 52
#define ASSIGN 53
#define PLUS_ASSIGN 54
#define MINUS_ASSIGN 55
#define STAR_ASSIGN 56
#define SLASH_ASSIGN 57
#define CONST_ASSIGN 58
#define INC 59
#define DECR 60
#define UPLUS 61
#define UMINUS 62
#define NOT_OP 63
#define SEMICOLON 64
#define TWO_DOT 65
#define THREE_DOT 66
#define TWO_COLON 67
#define TILDE 68
#define COMMA 69
#define DOT 70
#define QUESTION_MARK 71
#define DOLLAR 72
#define PARA 73
#define LPARA 74
#define RPARA 75
#define LBRACE 76
#define RBRACE 77
#define LBRACK 78
#define RBRACK 79
#define FOR 80
#define WHILE 81
#define UNTIL 82
#define JUMP 83
#define BREAK 84
#define CONT 85
#define IF 86
#define ELSE 87
#define ELIF 88
#define IF_BEGIN 89
#define ELSE_BEGIN 90
#define BEGIN 91
#define END 92
#define START 93
#define FINISH 94
#define LOAD 95
#define AND 96
#define OR 97
#define NOT 98
#define SPACE 99
#define TAB 100
#define NEWLINE 101
#define RETURN 102
#define ROW 103
#define EXPRESSION 104
#define ASSIGNMENT 105
#define STATEMENT 106
#define ASSIGNMENT_EXPRESSION 107
#define UNARY_EXPRESSION 108
#define BINARY_EXPRESSION 109
#define BALANCED_EXPRESSION_LIST 110
#define UNBALANCED_EXPRESSION_LIST 111




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
#define is_fbgc_EXPRESSION(x)(is_fbgc_ATOM(x) || x == IDENTIFIER || x == REFERENCE || x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_TUPLE(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x))

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x))



#ifdef  __cplusplus
}
#endif








#endif

