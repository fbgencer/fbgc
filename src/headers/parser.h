#ifndef PARSER_H
#define PARSER_H

/*
	Shunting-Yard algorithm

*/





uint8_t operator_precedence(fbgc_token T);

uint8_t parser(struct fbgc_object ** field);


#endif