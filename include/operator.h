#ifndef OPERATOR_H
#define OPERATOR_H

#ifdef __cplusplus
extern "C" {
#endif

extern struct fbgc_object * (*fbgc_operator[8]) (struct fbgc_object *, struct fbgc_object *, fbgc_token );

extern struct fbgc_object * (*fbgc_operator2[1]) (struct fbgc_object *, struct fbgc_object *, fbgc_token);

// usage of this call definition is just calling binary function from its token
// when we write call_fbgc_binary_op(PLUS,x,y) it will call plus, if we call STAR it will star function
// DO NOT change operator arrangements, this call function depends on those arrangements.
// See tokens.h file
//#define call_fbgc_binary_op(token,a,b,main_tok)(fbgc_binary_op[token - STARSTAR](a,b,main_tok))


#define is_fbgc_binary_op_null(token)(fbgc_operator[token - LOGIC] == NULL)

#define call_fbgc_operator(main_tok,a,b,op_tok)(fbgc_operator[main_tok - LOGIC](a,b,op_tok))
#define call_fbgc_operator2(main_tok,a,b,op_tok)(fbgc_operator2[0](a,b,op_tok))

struct fbgc_object * safe_call_fbgc_operator(struct fbgc_object * , struct fbgc_object * , fbgc_token, fbgc_token);


#ifdef  __cplusplus
}
#endif


#endif


/*
case R_SHIFT:
{
}
case L_SHIFT:
{
}
case STARSTAR:
{
}
case SLASHSLASH:
{
}
case PLUS:
{
}
case MINUS:
{
}
case STAR:
{
}
case SLASH:
{
}
case CARET:
{
}
case PERCENT:
{
}
case LO_EQ:
{
}
case GR_EQ:
{
}
case EQ_EQ:
{
}
case NOT_EQ:
{
}
case LOWER:
{
}
case GREATER:
{
}
case PIPE:
{
}
case AMPERSAND:
{
}
*/
