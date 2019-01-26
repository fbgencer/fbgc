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
#define REFERENCE 12
#define MATRIX 13
#define LIST 14
#define LINKED_LIST 15
#define LINKED_LIST_TAIL 16
#define STACK 17
#define OP 18
#define PLUS 19
#define MINUS 20
#define STAR 21
#define SLASH 22
#define MOD 23
#define STARSTAR 24
#define CARET 25
#define OR_OP 26
#define AND_OP 27
#define GREATER 28
#define LOWER 29
#define LO_EQ 30
#define GR_EQ 31
#define EQ_EQ 32
#define NOT_EQ 33
#define IS_EQ 34
#define IF_THEN 35
#define R_ARROW 36
#define L_ARROW 37
#define COL_LO 38
#define RW_ARROW 39
#define LW_ARROW 40
#define COLSTAR 41
#define COLSLASH 42
#define SLASHSLASH 43
#define COLON 44
#define RSHIFT 45
#define LSHIFT 46
#define ASSIGN 47
#define PLUS_ASSIGN 48
#define MINUS_ASSIGN 49
#define STAR_ASSIGN 50
#define SLASH_ASSIGN 51
#define CONST_ASSIGN 52
#define INC 53
#define DECR 54
#define UPLUS 55
#define UMINUS 56
#define NOT_OP 57
#define SEMICOLON 58
#define TWO_DOT 59
#define THREE_DOT 60
#define TWO_COLON 61
#define TILDE 62
#define COMMA 63
#define DOT 64
#define QUESTION_MARK 65
#define DOLLAR 66
#define PARA 67
#define LPARA 68
#define RPARA 69
#define LBRACE 70
#define RBRACE 71
#define LBRACK 72
#define RBRACK 73
#define FOR 74
#define WHILE 75
#define UNTIL 76
#define JUMP 77
#define BREAK 78
#define CONT 79
#define IF 80
#define ELSE 81
#define ELIF 82
#define IF_BEGIN 83
#define ELSE_BEGIN 84
#define BEGIN 85
#define END 86
#define START 87
#define FINISH 88
#define LOAD 89
#define AND 90
#define OR 91
#define NOT 92
#define SPACE 93
#define TAB 94
#define NEWLINE 95
#define RETURN 96
#define EXPRESSION 97
#define ASSIGNMENT 98
#define STATEMENT 99
#define ASSIGNMENT_EXPRESSION 100
#define UNARY_EXPRESSION 101
#define BINARY_EXPRESSION 102
#define BALANCED_IDENTIFIER_LIST 103
#define UNBALANCED_IDENTIFIER_LIST 104
#define BALANCED_EXPRESSION_LIST 105
#define UNBALANCED_EXPRESSION_LIST 106
#define VOID_EXPRESSION 107
#define PARA_VOID_EXPRESSION 108
#define PARA_EXPRESSION 109
#define PARA_ASSIGNMENT_EXPRESSION 110
#define PARA_BALANCED_EXPR_LIST 111
#define PARA_IDENTIFIER_LIST 112


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

