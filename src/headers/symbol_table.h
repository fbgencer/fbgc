#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define INITIAL_SYMBOL_TABLE_SIZE 16


/*
Symbols are just c-string objects.
It is somehow troublesome using c-string as symbols but it decreases the number of the objects we used.
C-string object has three variable in its struct. Base object has type and next pointer
type is used for recognition in memory, next pointer holds the actual object address
where the string content is the name of the variable
Consider the following line
	xyz_10 = 10

	//here xyz_10 is our symbol

	---------------
	|type:C-STRING|
	---------------          
	|next: -----\\|----> shows next symbol
	---------------			 
	|cont:'xyz_10'|          
	---------------          
							 
							 						 
*/


#define get_fbgc_symbol_name(x)(cast_fbgc_object_as_cstr(x)->content)
#define get_fbgc_symbol_obj(x)(x->next)


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