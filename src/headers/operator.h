#ifndef OPERATOR_H
#define OPERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

/*
STARSTAR $ **
PLUSPLUS $ ++
MINUSMINUS $ --
SLASHSLASH $ //
LO_EQ $ <=
GR_EQ $ >=
EQ_EQ $ ==
NOT_EQ $ !=
R_SHIFT $ >>
L_SHIFT $ <<
CARET $ ^
PERCENT $ %
LOWER $ <
GREATER $ >
PIPE $ |
AMPERSAND $ &
SLASH $ /
STAR $ *
MINUS $ -
PLUS $ +
*/

#define BINARY_FUNCTION_NO 18

/*
struct fbgc_object * fbgc_binary_starstar(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_slashslash(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_loeq(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_greq(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_eqeq(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_noteq(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_rshift(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_lshift(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_caret(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_percent(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_lower(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_greater(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_pipe(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_ampersand(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_slash(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_star(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_minus(struct fbgc_object *, struct fbgc_object *, fbgc_token);
struct fbgc_object * fbgc_binary_plus(struct fbgc_object *, struct fbgc_object *, fbgc_token);*/

struct fbgc_object * is_fbgc_object_true(struct fbgc_object * a);

//extern struct fbgc_object * (*fbgc_binary_op[BINARY_FUNCTION_NO])(struct fbgc_object *, struct fbgc_object *, fbgc_token );

extern struct fbgc_object * (*fbgc_binary_op[6]) (struct fbgc_object *, struct fbgc_object *, fbgc_token );

// usage of this call definition is just calling binary function from its token
// when we write call_fbgc_binary_op(PLUS,x,y) it will call plus, if we call STAR it will star function
// DO NOT change operator arrangements, this call function depends on those arrangements.
// See tokens.h file
//#define call_fbgc_binary_op(token,a,b,main_tok)(fbgc_binary_op[token - STARSTAR](a,b,main_tok))


#define is_fbgc_binary_op_null(token)(fbgc_binary_op[token - INT] == NULL)

#define call_fbgc_binary_op__new(main_tok,a,b,op_tok)(fbgc_binary_op[main_tok - INT](a,b,op_tok))


#ifdef  __cplusplus
}
#endif


#endif
