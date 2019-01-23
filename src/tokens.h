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
#define LINKED_LIST_TAIL 15
#define STACK 16
#define OP 17
#define PLUS 18
#define MINUS 19
#define STAR 20
#define SLASH 21
#define MOD 22
#define STARSTAR 23
#define CARET 24
#define OR_OP 25
#define AND_OP 26
#define GREATER 27
#define LOWER 28
#define LO_EQ 29
#define GR_EQ 30
#define EQ_EQ 31
#define NOT_EQ 32
#define IS_EQ 33
#define IF_THEN 34
#define R_ARROW 35
#define L_ARROW 36
#define COL_LO 37
#define RW_ARROW 38
#define LW_ARROW 39
#define COLSTAR 40
#define COLSLASH 41
#define SLASHSLASH 42
#define COLON 43
#define RSHIFT 44
#define LSHIFT 45
#define ASSIGN 46
#define PLUS_ASSIGN 47
#define MINUS_ASSIGN 48
#define STAR_ASSIGN 49
#define SLASH_ASSIGN 50
#define CONST_ASSIGN 51
#define INC 52
#define DECR 53
#define UPLUS 54
#define UMINUS 55
#define NOT_OP 56
#define SEMICOLON 57
#define TWO_DOT 58
#define THREE_DOT 59
#define TWO_COLON 60
#define TILDE 61
#define COMMA 62
#define DOT 63
#define QUESTION_MARK 64
#define DOLLAR 65
#define PARA 66
#define LPARA 67
#define RPARA 68
#define LBRACE 69
#define RBRACE 70
#define LBRACK 71
#define RBRACK 72
#define FOR 73
#define WHILE 74
#define UNTIL 75
#define JUMP 76
#define BREAK 77
#define CONT 78
#define IF 79
#define ELSE 80
#define ELIF 81
#define IF_BEGIN 82
#define ELSE_BEGIN 83
#define BEGIN 84
#define END 85
#define START 86
#define FINISH 87
#define AND 88
#define OR 89
#define NOT 90
#define SPACE 91
#define TAB 92
#define NEWLINE 93
#define RETURN 94
#define EXPRESSION 95
#define ASSIGNMENT 96
#define STATEMENT 97
#define ASSIGNMENT_EXPRESSION 98
#define UNARY_EXPRESSION 99
#define BINARY_EXPRESSION 100
#define BALANCED_IDENTIFIER_LIST 101
#define UNBALANCED_IDENTIFIER_LIST 102
#define BALANCED_EXPRESSION_LIST 103
#define UNBALANCED_EXPRESSION_LIST 104
#define VOID_EXPRESSION 105
#define PARA_VOID_EXPRESSION 106
#define PARA_EXPRESSION 107
#define PARA_ASSIGNMENT_EXPRESSION 108
#define PARA_BALANCED_EXPR_LIST 109
#define PARA_IDENTIFIER_LIST 110





extern const char * object_name_array[];

static const token_struct operator_token_array[];
static const token_struct reserved_words_token_array[];


fbgc_token get_operator_code(const char *str);
fbgc_token get_operator_code_from_substr(const char *str1,const char *str2);
fbgc_token get_reserved_word_code(const char *str);
fbgc_token get_reserved_word_code_from_substr(const char *str1,const char *str2);


const char * get_token_as_str(fbgc_token tok);


#define is_fbgc_OPERATOR(T)(T>=OP && T<= RBRACK)
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

#define is_fbgc_FUNCTIONABLE(x)(x == IF || x == ELIF || x == ELSE)

#define is_fbgc_START(x)(x == START || x == BEGIN || x == IF_BEGIN)
#define is_fbgc_LIST(x) (x == LIST)
#define is_fbgc_PARA_EXPRESSION(x)(x == PARA_EXPRESSION)
#define is_fbgc_EXPRESSION(x)(x == ATOM || x == EXPRESSION || x == UNARY_EXPRESSION ||x==BINARY_EXPRESSION||\
is_fbgc_PARA_EXPRESSION(x) || is_fbgc_MATRIX(x) || is_fbgc_LIST(x))

#define is_fbgc_ASSIGNMENT(x)(x==ASSIGNMENT_EXPRESSION || x == PARA_ASSIGNMENT_EXPRESSION)
#define is_fbgc_STATEMENT(x)(x == STATEMENT || is_fbgc_EXPRESSION(x) || is_fbgc_ASSIGNMENT(x))



#endif

