#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

/*
	Shunting-Yard algorithm

*/

uint8_t operator_precedence(fbgc_token T);

uint8_t parser(struct fbgc_object ** field,FILE * fp);

#ifdef  __cplusplus
}
#endif

#endif