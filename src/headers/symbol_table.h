#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H



struct fbgc_symbol_table{
	struct fbgc_object base;
	struct fbgc_object ** symbols;
	uint8_t size;  	
	//capacity is always size + 2
};

struct fbgc_object * new_fbgc_symbol_table(void);
struct fbgc_object * new_fbgc_symbol(struct fbgc_object * table,struct fbgc_object * symbol);
struct fbgc_object * get_fbgc_symbol(struct fbgc_object * var);

void free_fbgc_symbol_table(struct fbgc_object * table_obj);

#endif