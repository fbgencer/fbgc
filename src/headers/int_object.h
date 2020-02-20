#ifndef FBGC_INT_OBJECTH
#define FBGC_INT_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_int_object{
    struct fbgc_object base;
    int content;
};

#define cast_fbgc_object_as_int(x)(((struct fbgc_int_object*) x))
#define sizeof_fbgc_int_object(x)(sizeof(struct fbgc_int_object))


struct fbgc_object * new_fbgc_int_object(int);
struct fbgc_object * derive_from_new_int_object(fbgc_token type, int int_content);
struct fbgc_object * new_fbgc_int_object_from_str(const char *);
struct fbgc_object * new_fbgc_int_object_from_substr(const char *,const char *,fbgc_token );

struct fbgc_object * operator_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

struct fbgc_object * operator_fbgc_int_object2(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

void print_fbgc_int_object(struct fbgc_object *);
void free_fbgc_int_object(struct fbgc_object * );




//
// struct fbgc_object * inline operator_R_SHIFT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_L_SHIFT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_STARSTAR_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_SLASHSLASH_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_PLUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_MINUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_STAR_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_SLASH_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_CARET_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_PERCENT_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_LO_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_GR_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_EQ_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_NOT_EQ_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_LOWER_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_GREATER_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_PIPE_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_AMPERSAND_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_EXCLAMATION_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_TILDE_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_UPLUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// struct fbgc_object * inline operator_UMINUS_fbgc_int_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
// //


struct fbgc_object * operator_R_SHIFT_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_L_SHIFT_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_STARSTAR_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_SLASHSLASH_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_PLUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_MINUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_STAR_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_SLASH_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_CARET_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_PERCENT_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_LO_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_GR_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_EQ_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_NOT_EQ_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_LOWER_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_GREATER_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_PIPE_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_AMPERSAND_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_EXCLAMATION_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_TILDE_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_UPLUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
struct fbgc_object * operator_UMINUS_fbgc_object(struct fbgc_object * a,struct fbgc_object * b, fbgc_token type);
//


extern struct fbgc_object * (*fbgc_INT_operators[22]) (struct fbgc_object *, struct fbgc_object *, fbgc_token);





#ifdef  __cplusplus
}
#endif

#endif