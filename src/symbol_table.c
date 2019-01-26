#include "fbgc.h"


struct fbgc_object * new_fbgc_symbol_table(void){
	struct fbgc_symbol_table * table =  (struct fbgc_symbol_table*) malloc(sizeof(struct fbgc_symbol_table));
    table->base.type = LIST; 
    table->base.next = NULL;
    table->size = 0;

    table->symbols = (struct fbgc_str_object **) malloc(sizeof(struct fbgc_str_object **) * 2);
    table->symbols[0] = NULL;
	table->symbols[1] = NULL;
    return (struct fbgc_object*) table;
}
struct fbgc_object * new_fbgc_symbol_from_substr(struct fbgc_object * table_obj,const char * str1,const char * str2){
	//first compare incoming str with the old identifiers
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	struct fbgc_object * ref_obj = new_fbgc_ref_object();



	int8_t cmp = 1;
	for(uint8_t i = 0; i<table->size; i++){
		cmp = memcmp(str1,cast_fbgc_object_as_str(table->symbols[i])->content,str2-str1);
		if(cmp == 0) {
			//
			cprintf(110,"Found in the symbol table!\n");
			ref_obj = assign_fbgc_ref_object(ref_obj,&(cast_fbgc_object_as_str(table->symbols[i])->base));
			break;
		}
	}

	if(cmp != 0){
		struct fbgc_object * temp_obj =  new_fbgc_str_object_from_substr(str1,str2);
		temp_obj->type = WORD;
		//print_fbgc_str_object(temp_obj);
		if(table->size > 0 && table->size % 2 == 0){
			table->symbols = (struct fbgc_str_object **) realloc(table->symbols,sizeof(struct fbgc_str_object **)*(table->size+2));
		}
		table->symbols[table->size] = temp_obj;
		ref_obj = assign_fbgc_ref_object(ref_obj,&(cast_fbgc_object_as_str(table->symbols[table->size])->base));
		table->size++;
	}
	return (struct fbgc_object *) ref_obj;
}

struct fbgc_object * get_fbgc_symbol(struct fbgc_object * var);


uint8_t is_variable_in_sym_table_exist(struct fbgc_object * table,struct fbgc_object * var){

}

void print_fbgc_symbol_table(struct fbgc_object * table_obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	struct fbgc_str_object * temp_str = NULL;
	cprintf(010,"\n+++++++++++++++++SYMBOL_TABLE++++++++++++++++\n");
	for(int i = 0; i<table->size; i++){
		temp_str = cast_fbgc_object_as_str(table->symbols[i]);
		cprintf(110,"{%s : ",temp_str->content);
		if(temp_str->base.next != NULL){
			print_fbgc_object(temp_str->base.next);
		}
		else cprintf(110,"NULL}\n");
	}


	cprintf(010,"++++++++++++++++++++++++++++++++++++++++++++++\n\n");
}


void free_fbgc_symbol_table(struct fbgc_object * table_obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;

	for(uint8_t i = 0; i<table->size; i++ ){
		if(cast_fbgc_object_as_str(table->symbols[i])->base.next != NULL){
			free_fbgc_object(cast_fbgc_object_as_str(table->symbols[i])->base.next);
		}
		free_fbgc_str_object( (struct fbgc_object *) table->symbols[i] );
	}

	free(table->symbols);
	table->size = 0;
	free(table);
}
