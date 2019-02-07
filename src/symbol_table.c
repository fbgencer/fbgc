#include "fbgc.h"


struct fbgc_object * new_fbgc_symbol_table(void){
	struct fbgc_symbol_table * table =  (struct fbgc_symbol_table*) malloc(sizeof(struct fbgc_symbol_table));
    table->base.type = LIST; 
    table->base.next = NULL;
    table->size = 0;

    table->symbols = (struct fbgc_object **) malloc(sizeof(struct fbgc_str_object **) * 2);
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
			ref_obj = initialize_fbgc_ref_object(ref_obj,&(cast_fbgc_object_as_str(table->symbols[i])->base));
			return (struct fbgc_object *) ref_obj;
		}
	}
	//cprintf(110,"New symbol!!\n");
	struct fbgc_object * temp_obj =  new_fbgc_str_object_from_substr(str1,str2);
	temp_obj->type = WORD;
	//print_fbgc_str_object(temp_obj);
	if(table->size > 0 && table->size % 2 == 0){
		table->symbols = (struct fbgc_object **) realloc(table->symbols,sizeof(struct fbgc_str_object **)*(table->size+2));
	}
	table->symbols[table->size] = temp_obj;
	ref_obj = initialize_fbgc_ref_object(ref_obj,&(cast_fbgc_object_as_str(table->symbols[table->size])->base));
	table->size++;
	
	//cprintf(100,"Succesfully added in the symbol table\n");
	//print_fbgc_symbol_table(table_obj);

	return (struct fbgc_object *) ref_obj;
}

struct fbgc_object * get_fbgc_symbol(struct fbgc_object * var);

/*
struct fbgc_object * load_module_in_symbol_table(struct fbgc_object * table_obj,struct fbgc_object * module){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	//first seek in our C modules
	//after search .fbgc modules!

	
}
*/

void print_fbgc_symbol_table(struct fbgc_object * table_obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	struct fbgc_str_object * temp_str = NULL;
	cprintf(010,"[SYMBOL_TABLE]: ");
	for(int i = 0; i<table->size; i++){
		temp_str = cast_fbgc_object_as_str(table->symbols[i]);
		cprintf(110,"{%s : ",temp_str->content);
		if(temp_str->base.next != NULL){
			print_fbgc_object(temp_str->base.next);
		}
		else cprintf(110,"NULL");
	}
	cprintf(110,"}\n");

}

void free_fbgc_symbol_table(struct fbgc_object * table_obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	cprintf(011,"Free symbol table size :%d!\n",table->size);
	for(uint8_t i = 0; i<table->size; i++ ){
		if(cast_fbgc_object_as_str(table->symbols[i])->base.next != NULL){
			cprintf(011,"Base.next != NULL\n");
			//actual object that is held by identifier
			struct fbgc_object * temp = cast_fbgc_object_as_str(table->symbols[i])->base.next;

			for(uint8_t q = i+1; q <(table->size) && cast_fbgc_object_as_str(table->symbols[q])->base.next != NULL; q++){
				if( cast_fbgc_object_as_str(table->symbols[q])->base.next == temp){
					cast_fbgc_object_as_str(table->symbols[q])->base.next = NULL;
					//cprintf(100,"Variables shows same object! var[%d]\n",q);
				}

			}
			free_fbgc_object(temp);
		}

		free_fbgc_object( (struct fbgc_object *) table->symbols[i] );
	}

	free(table->symbols);
	table->size = 0;
	free(table);
}
