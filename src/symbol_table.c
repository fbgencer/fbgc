#include "fbgc.h"



struct fbgc_object * initialize_fbgc_symbol_table(){
    //struct fbgc_tuple_object * table = (struct fbgc_tuple_object*) new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
   	fbgc_symbols = new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
    return NULL;
}
struct fbgc_ll_base * new_fbgc_symbol_from_substr(const char * str1,const char * str2){
	
	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Symbol table from substring\n");
	#endif


	//General symbol holder see symbol_table.h
	struct fbgc_object ** symbols = tuple_object_content(fbgc_symbols);


	//############################
	//Oh. this part just fucks the time for the variable searching. Clearly, without hash table adding new variable causes lots of times!
	//############################
    //clock_t begin,end;
   // double search_time;
	//begin = clock();
	
	#ifdef SYMBOL_TABLE_DEBUG
		cprintf(111,"Trying to find str in old symbols\n");
	#endif

	for(size_t i = 0; i<size_fbgc_tuple_object(fbgc_symbols); i++){
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		// Find something proper, don't check the string size first but without checking the first match of n characters will return true 
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		if( strlen(content_fbgc_cstr_object(symbols[i])) == str2-str1 && !my_strncmp(content_fbgc_cstr_object(symbols[i]),str1,str2-str1)  ){

			#ifdef SYMBOL_TABLE_DEBUG
				cprintf(110,"Found!\n");
			#endif
			return _new_fbgc_ll_identifier(i); 
		}
	}


	#ifdef SYMBOL_TABLE_DEBUG
		cprintf(111,"Couldn't found, creating symbol!\n");
	#endif	
	//end = clock();
	//search_time = (double)(end - begin) / CLOCKS_PER_SEC; 
	//printf("Time :%f\n",search_time );

	struct fbgc_object * temp_obj =  new_fbgc_cstr_object_from_substr(str1,str2);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"OLD symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	cprintf(110,"New symbol from s1:%s to s2:%s\n",str1,str2);
	cprintf(110,"Symbol has been created as cstring : ");
	print_fbgc_cstr_object(temp_obj);
	cprintf(111,"\n");
	#endif

	
	fbgc_symbols = push_back_fbgc_tuple_object(fbgc_symbols,temp_obj);

	//:>cast_fbgc_object_as_field(field_obj)->symbols = table;
	//fbgc_symbols = table;

	//table pointer may not be the same anymore
	symbols = tuple_object_content(fbgc_symbols);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Succesfully added in the symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	#endif	

	return _new_fbgc_ll_identifier(size_fbgc_tuple_object(fbgc_symbols)-1);
}


void print_fbgc_symbol_table(struct fbgc_object * ll){


	cprintf(010,"[~~~~~~~~~~~~[SYMBOL TABLE]~~~~~~~~~~~~~]\n");
//	print_fbgc_ll_object(ll,"SYMBOLS:");
	cprintf(110,"\n");
	cprintf(010,"[~~~~~~~~~~~~~~~~~~~~~~~~~]\n");

}
