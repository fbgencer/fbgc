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



struct fbgc_object * new_fbgc_symbol_from_substr(struct fbgc_object * field_obj,const char * str1,const char * str2){
	
	struct fbgc_object * o = new_cfun_object_from_substr(field_obj,str1,str2);
	if(o!=NULL) return o;

	//first compare incoming str with the old identifiers
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) cast_fbgc_object_as_field(field_obj)->global_table;
	struct fbgc_object * ref_obj = new_fbgc_ref_object();

	
	int8_t cmp = 1;
	for(uint8_t i = 0; i<table->size; i++){
		if(strlen(cast_fbgc_object_as_str(table->symbols[i])->content) != str2-str1) continue;
		cmp = strncmp(str1,cast_fbgc_object_as_str(table->symbols[i])->content,str2-str1);
		if(cmp == 0) {
			//
			//cprintf(110,"Found in the symbol table!\n");
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


struct fbgc_object * new_cfun_object_from_substr(struct fbgc_object * field_obj,const char * str1,const char * str2){

	struct fbgc_ll_object * ll = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(field_obj)->modules );
	struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)ll->base.next;
	while(cm!= NULL && (struct fbgc_object * )cm != ll->tail){
		const struct fbgc_cfunction * cc = cm->module->functions[0];
		//cprintf(111,"Functions:\n");
		for (int i = 1; cc!=NULL; ++i){
			if(!memcmp(str1,cc->name,str2-str1)){
				//cprintf(010,"\n**Function [%s] is founded in module [%s]**\n",cc->name,cm->module->name);
				return new_fbgc_cfun_object(cc->function);
			} 
			//cprintf(101,"{%s}\n",cc->name);
			cc = cm->module->functions[i];
		}
		cm = (struct fbgc_cmodule_object * )cm->base.next;
	}

	return NULL;
}

struct fbgc_object * get_fbgc_symbol(struct fbgc_object * var);

/*
struct fbgc_object * load_module_in_symbol_table(struct fbgc_object * table_obj,struct fbgc_object * module){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	//first seek in our C modules
	//after search .fbgc modules!

	
}
*/


uint8_t is_object_referenced_in_symbol_table(struct fbgc_object * table_obj,struct fbgc_object * ref,struct fbgc_object * obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	//cprintf(111,"searching :"); print_fbgc_object(obj); printf("\n");
	for(uint8_t i = 0; i<table->size; i++ ){
		if(cast_fbgc_object_as_str(table->symbols[i])->base.next != NULL && cast_fbgc_object_as_str(table->symbols[i])->base.next == obj 
			&& cast_fbgc_object_as_str(table->symbols[i]) != cast_fbgc_object_as_str(cast_fbgc_object_as_ref(ref)->content)){	
			return 1;
		}

	}
	//cprintf(111,"Returning zero, object is not referenced before!\n");
	return 0;
}

void print_fbgc_symbol_table(struct fbgc_object * table_obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	struct fbgc_str_object * temp_str = NULL;
	cprintf(010,"[SYMBOL_TABLE]: ");
	for(int i = 0; i<table->size; i++){
		temp_str = cast_fbgc_object_as_str(table->symbols[i]);
		cprintf(110,"{%s : ",temp_str->content);
		if(temp_str->base.next != NULL){
			print_fbgc_object(temp_str->base.next);
			cprintf(110,"}");
		}
		else cprintf(110,"NULL}");
	}
	cprintf(110,"\n");

}

void free_fbgc_symbol_table(struct fbgc_object * table_obj){
	struct fbgc_symbol_table * table = (struct fbgc_symbol_table *) table_obj;
	//cprintf(011,"Free symbol table size :%d!\n",table->size);
	for(uint8_t i = 0; i<table->size; i++ ){
		if(cast_fbgc_object_as_str(table->symbols[i])->base.next != NULL){
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
	free(table);
}
