#ifndef OPERATOR_H
#define OPERATOR_H



struct fbgc_object * fbgc_binary_plus(struct fbgc_object *, struct fbgc_object *);
struct fbgc_object * fbgc_binary_minus(struct fbgc_object *, struct fbgc_object *);
struct fbgc_object * fbgc_binary_star(struct fbgc_object *, struct fbgc_object *);
struct fbgc_object * fbgc_binary_slash(struct fbgc_object *, struct fbgc_object *);

void fbgc_assignment_assign(struct fbgc_object *, struct fbgc_object *);


struct fbgc_object * (*fbgc_binary_op[])(struct fbgc_object *, struct fbgc_object *);
void (*fbgc_assignment_op[])(struct fbgc_object *, struct fbgc_object *);


// usage of this call definition is just calling binary function from its token
// when we write call_fbgc_binary_op(PLUS,x,y) it will call plus, if we call STAR it will star function
// DO NOT change operator arrangements, this call function depends on those arrangements.
// See tokens.h file

#define call_fbgc_binary_op(token,a,b)(fbgc_binary_op[token - PLUS](a,b))
#define call_fbgc_assignment_op(token,a,b)(fbgc_assignment_op[token - ASSIGN](a,b))

#endif
