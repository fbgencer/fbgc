#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

struct symbol_table{
	struct fbgc_object base;
	char ** symbol_names;
	fbgc_token c; 	
};

#endif