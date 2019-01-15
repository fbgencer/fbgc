#ifndef PARSER_H
#define PARSER_H

/*
	Shunting-Yard algorithm

*/





uint8_t operator_precedence(fbgc_token T);

struct
fbgc_object *  parser(struct fbgc_object * head,struct fbgc_object * tail);

struct
fbgc_object * op_stack_push(struct fbgc_object * head,struct fbgc_object * obj);
struct
fbgc_object * op_stack_pop(struct fbgc_object * head);


#endif