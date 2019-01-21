#ifndef TOKENS_H
#define TOKENS_H


typedef uint8_t fbgc_token;

typedef struct {
	fbgc_token tokencode;
	const char * name;
}token_struct;

#define UNKNOWN 0
#define ATOM 1
#define INT 2
#define HEX 3
#define BIN 4
#define DOUBLE 5
#define COMPLEX 6
#define STRING 7
#define IDENTIFIER 8
#define WORD 9
#define VAR 10
#define FUN 11
#define MATRIX 12
#define LIST 13
#define LINKED_LIST 14
#define OP 15
#define PLUS 16
#define MINUS 17
#define STAR 18
#define SLASH 19
#define MOD 20
#define STARSTAR 21
#define CARET 22
#define OR_OP 23
#define AND_OP 24
#define GREATER 25
#define LOWER 26
#define LO_EQ 27
#define GR_EQ 28
#define EQ_EQ 29
#define NOT_EQ 30
#define IS_EQ 31
#define IF_THEN 32
#define R_ARROW 33
#define L_ARROW 34
#define COL_LO 35
#define RW_ARROW 36
#define LW_ARROW 37
#define COLSTAR 38
#define COLSLASH 39
#define SLASHSLASH 40
#define COLON 41
#define RSHIFT 42
#define LSHIFT 43
#define ASSIGN 44
#define PLUS_ASSIGN 45
#define MINUS_ASSIGN 46
#define STAR_ASSIGN 47
#define SLASH_ASSIGN 48
#define CONST_ASSIGN 49
#define INC 50
#define DECR 51
#define UPLUS 52
#define UMINUS 53
#define NOT_OP 54
#define SEMICOLON 55
#define TWO_DOT 56
#define THREE_DOT 57
#define TWO_COLON 58
#define TILDE 59
#define COMMA 60
#define DOT 61
#define QUESTION_MARK 62
#define DOLLAR 63
#define PARA 64
#define LPARA 65
#define RPARA 66
#define LBRACE 67
#define RBRACE 68
#define LBRACK 69
#define RBRACK 70
#define FOR 71
#define WHILE 72
#define UNTIL 73
#define JUMP 74
#define BREAK 75
#define CONT 76
#define IF 77
#define ELSE 78
#define ELIF 79
#define END 80
#define AND 81
#define OR 82
#define NOT 83
#define SPACE 84
#define TAB 85
#define NEWLINE 86
#define RETURN 87
#define EXPRESSION 88
#define ASSIGNMENT 89
#define STATEMENT 90
#define ASSIGNMENT_EXPRESSION 91
#define UNARY_EXPRESSION 92
#define BINARY_EXPRESSION 93
#define BALANCED_IDENTIFIER_LIST 94
#define UNBALANCED_IDENTIFIER_LIST 95
#define BALANCED_EXPR_LIST 96
#define UNBALANCED_EXPR_LIST 97
#define PARA_EMPTY 98
#define PARA_EXPRESSION 99
#define PARA_ASSIGNMENT_EXPRESSION 100
#define PARA_BALANCED_EXPR_LIST 101
#define PARA_IDENTIFIER_LIST 102




extern const char * object_name_array[];

static const token_struct operator_token_array[];
static const token_struct reserved_words_token_array[];


fbgc_token get_operator_code(const char *str);
fbgc_token get_operator_code_from_substr(const char *str1,const char *str2);
fbgc_token get_reserved_word_code(const char *str);
fbgc_token get_reserved_word_code_from_substr(const char *str1,const char *str2);


const char * get_token_as_str(fbgc_token tok);


#define is_fbgc_OPERATOR(T)(T>=OP && T< PARA)
#define is_fbgc_IDENTIFIER(T)(T>=IDENTIFIER && T<=FUN)
#define is_fbgc_ATOM(T)(T>= ATOM && T<=COMPLEX)
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

#define is_fbgc_START(x)(x == LINKED_LIST)
#define is_fbgc_LIST(x) (x == LIST)
#define is_fbgc_PARA_EXPRESSION(x)(x == PARA_EXPRESSION)
#define is_fbgc_EXPRESSION(x)(x == ATOM || x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_PARA_EXPRESSION(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x))

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION || x == PARA_ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x))



#endif

