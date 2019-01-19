#ifndef TOKENS_H
#define TOKENS_H


typedef uint8_t fbgc_token;

typedef struct {
	fbgc_token tokencode;
	const char * name;
}token_struct;

#define UNKNOWN 0
#define NUMBER 1
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
#define ARRAY 13
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
#define ASSIGN 25
#define PLUS_ASSIGN 26
#define MINUS_ASSIGN 27
#define STAR_ASSIGN 28
#define SLASH_ASSIGN 29
#define CONST_ASSIGN 30
#define GREATER 31
#define LOWER 32
#define LO_EQ 33
#define GR_EQ 34
#define EQ_EQ 35
#define NOT_EQ 36
#define IS_EQ 37
#define IF_THEN 38
#define R_ARROW 39
#define L_ARROW 40
#define COL_LO 41
#define RW_ARROW 42
#define LW_ARROW 43
#define COLSTAR 44
#define COLSLASH 45
#define SLASHSLASH 46
#define COLON 47
#define RSHIFT 48
#define LSHIFT 49
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
#define AND 80
#define OR 81
#define NOT 82
#define SPACE 83
#define TAB 84
#define NEWLINE 85
#define RETURN 86
#define EXPRESSION 87
#define CLOSED_EXPRESSION 88
#define COMP_EXPRESSION 89
#define CLOSED_COMP_EXPRESSION 90
#define ASSIGNMENT 91




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
#define is_fbgc_NUMBER(T)(T>= NUMBER && T<=COMPLEX)
#define is_fbgc_INT(T) (T == INT)
#define is_fbgc_DOUBLE(T) (T == DOUBLE)
#define is_fbgc_STRING(T) (T == STRING)
#define is_fbgc_PARA(T)(T>=PARA && T<= RBRACK)
#define is_fbgc_BINARY_OPERATOR(T)(T>=PLUS && T<= LSHIFT)
#define is_fbgc_UNARY_OPERATOR(T)((T>=INC && T<=NOT_OP)|| T == PLUS || T == MINUS )
#define is_fbgc_VAR(T)(T == VAR )
#define is_fbgc_FUN(T)(T == FUN)
#define is_fbgc_WORD(T)(T == WORD)




#endif

