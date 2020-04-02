#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
	Shunting-Yard algorithm
*/


struct parser_packet{
	struct fbgc_ll * head;
	struct fbgc_ll_base * iter;
	struct fbgc_ll_base * iter_prev;
	struct fbgc_ll_base * op;
	fbgc_grammar gm;
	struct fbgc_ll_base * scope_list;	
	uint8_t error_code;
	uint8_t gm_error;
};

uint8_t operator_precedence(fbgc_token T);

uint8_t parser(struct fbgc_object ** field,FILE * fp);

#ifdef  __cplusplus
}
#endif

#endif