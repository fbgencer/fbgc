#include "fbgc.h"


struct fbgc_object * new_fbgc_symbol_table(){
    struct fbgc_tuple_object * table = (struct fbgc_tuple_object*) new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
    //table->next = 
    return table;
}
struct fbgc_object * new_fbgc_symbol_from_substr(struct fbgc_object * field_obj,const char * str1,const char * str2){
	
	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Symbol table new symbol from substr..\n");
	#endif
	//############
	//This part is for the recognization of the c module functions
	//	struct fbgc_object * o = new_cfun_object_from_substr(field_obj,str1,str2);
	//if(o) return o;
	//first compare incoming str with the old identifiers
	struct fbgc_object * table = (struct fbgc_object *) cast_fbgc_object_as_field(field_obj)->symbols;
	struct fbgc_object * ref_obj = new_fbgc_ref_object(-1);

	struct fbgc_object ** symbols = tuple_object_content(table);


	//############################
	//Oh. this part just fucks the time for the variable searching. Clearly, without hash table adding new variable causes lots of times!
	//############################
	int8_t cmp = 1;
	for(size_t i = 0; i<size_fbgc_tuple_object(table); i++){
		if( length_fbgc_cstr_object(symbols[i]) != str2-str1) continue;
		cmp = strncmp(str1,content_fbgc_cstr_object(symbols[i]),str2-str1);
		if(cmp == 0) {
			//
			//cprintf(110,"Found in the symbol table!\n");
			return (struct fbgc_object *) new_fbgc_ref_object(i);
		}
	}

	struct fbgc_object * temp_obj =  new_fbgc_cstr_object_from_substr(str1,str2);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"OLD the symbol table : ");
	print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	cprintf(111,"\n");
	#endif


	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(110,"New symbol from s1:%s to s2:%s\n",str1,str2);
	cprintf(110,"Symbol has been created as cstring : ");
	print_fbgc_cstr_object(temp_obj);
	cprintf(111,"\n");

	if(!strncmp("fbg9",str1,4))	cprintf(111,"================[FBG9]=====================\n");
	#endif

	if(!strncmp("fbg9",str1,4)){
		cprintf(111,"================[pushback oncesi table]=====================\n");
		print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);		
		cprintf(111,"\n");
	}
	
	table = push_back_fbgc_tuple_object(table,temp_obj);
	
	if(!strncmp("fbg9",str1,4)){
		cprintf(111,"================[pushback sonrasi table]=====================\n");
		print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
		cprintf(111,"\n\n");	

		print_fbgc_memory_block();	
	}	
	cast_fbgc_object_as_field(field_obj)->symbols = table;

	if(!strncmp("fbg9",str1,4)){
		cprintf(111,"================[pushback sonrasi assignment ile table]=====================\n");
		print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
		cprintf(111,"\n\n");		
	}	
	//table pointer may not be the same anymore
	symbols = tuple_object_content(table);
	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Succesfully added in the symbol table : ");
	print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	cprintf(111,"\n");
	#endif	
	return new_fbgc_ref_object(size_fbgc_tuple_object(table)-1);
	

}

struct fbgc_object * new_cfun_object_from_substr(struct fbgc_object * field_obj,const char * str1,const char * str2){

	/*struct fbgc_ll_object * ll = cast_fbgc_object_as_ll( cast_fbgc_object_as_field(field_obj)->modules );
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

	return NULL;*/
}



void print_fbgc_symbol_table(struct fbgc_object * ll){


	cprintf(010,"[~~~~~~~~~~~~[SYMBOL TABLE]~~~~~~~~~~~~~]\n");
	print_fbgc_ll_object(ll,"SYMBOLS:");
	cprintf(110,"\n");
	cprintf(010,"[~~~~~~~~~~~~~~~~~~~~~~~~~]\n");

}

void free_fbgc_symbol_table(struct fbgc_object * table_obj){
	/*
	#define table cast_fbgc_object_as_symbol_table(table_obj)
	
	cprintf(100,"=====================================CLEAN SYMBOLS==================================================\n");

	//##################################################################
	//This part work awful and it needs a new algorithm because 
	//when we free a symbol and its object, another symbol may points freed object and this will
	//create problem. However, this is the end of the main program, 
	//it does not effect runtime environment for fbgc so i left an open issue.
	//This issue must be solved.
	//##################################################################

	for(uint8_t i = 0; i<table->size; i++ ){
		struct fbgc_object * temp = cast_fbgc_object_as_str(table->symbols[i])->base.next;
		
		if(temp != NULL){
			
			//now ownership is broken for temp's object
			/*temp->type &= 0x7F;
			cast_fbgc_object_as_str(table->symbols[i])->base.next = NULL;
			if(get_fbgc_object_type(temp) == TUPLE){
				for(int i = 0; i<cast_fbgc_object_as_tuple(temp)->size; i++){
					cast_fbgc_object_as_tuple(temp)->contents[i]->type &= 0x7F;
				}
			}

			cprintf(100,"Broken objest previous mark bit :%d, type:%s\n",(temp->type & 0x80) == 0x80,object_type_as_str(temp) );
			claim_ownership_in_symbol_table(table_obj);
			cprintf(100,"Broken objest mark bit :%d\n",(temp->type & 0x80) == 0x80 );*/
	/*		for(uint8_t q = i+1; q <(table->size) && cast_fbgc_object_as_str(table->symbols[q])->base.next != NULL; q++){
				if( cast_fbgc_object_as_str(table->symbols[q])->base.next == temp){
					
					cast_fbgc_object_as_str(table->symbols[q])->base.next = NULL;
					//cprintf(100,"Variables shows same object! var[%d]\n",q);
				}

			}
			free_fbgc_object(temp);
		}

			//cprintf(111,"call free ::::: %s\n",object_name_array[get_fbgc_object_type(table->symbols[i])]);
		free_fbgc_object( (struct fbgc_object *) table->symbols[i] );
	}

	free(table->symbols);
	free(table);
	#undef table*/
}
