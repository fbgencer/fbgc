#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H



struct fbgc_symbol_table{
	struct fbgc_object base;
	struct fbgc_object ** symbols;
	uint8_t size;  	
	//capacity is always size + 2
};


struct fbgc_object * new_fbgc_symbol_table(void);
struct fbgc_object * new_fbgc_symbol(struct fbgc_object * table,const char * str1,const char * str2);
struct fbgc_object * get_fbgc_symbol(struct fbgc_object * var);

uint8_t is_variable_in_sym_table_exist(struct fbgc_object * table,struct fbgc_object * var);


void print_fbgc_symbol_table(struct fbgc_object * table_obj);
void free_fbgc_symbol_table(struct fbgc_object * table_obj);

#endif