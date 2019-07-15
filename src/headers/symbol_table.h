#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif


struct fbgc_symbol{
	struct fbgc_object * obj;
	const char * name;
	  	
};

struct fbgc_symbol_table{
	struct fbgc_object base;
	struct fbgc_object ** symbols;
	size_t size;  	
};



#define cast_fbgc_object_as_symbol_table(x)((struct fbgc_symbol_table *)x)



struct fbgc_object * new_fbgc_symbol_table(void);
struct fbgc_object * new_fbgc_symbol(struct fbgc_object * ,const char * ,const char * );
struct fbgc_object * get_fbgc_symbol(struct fbgc_object * );

struct fbgc_object * new_fbgc_symbol_from_substr(struct fbgc_object * table_obj,const char * str1,const char * str2);
struct fbgc_object * new_cfun_object_from_substr(struct fbgc_object * table_obj,const char * str1,const char * str2);

uint8_t is_object_referenced_in_symbol_table(struct fbgc_object * table_obj,struct fbgc_object * ,struct fbgc_object * obj);
struct fbgc_object * load_module_in_symbol_table(struct fbgc_object *,struct fbgc_object * );

void print_fbgc_symbol_table(struct fbgc_object * );
void free_fbgc_symbol_table(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif