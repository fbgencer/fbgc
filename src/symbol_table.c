// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"

struct general_symbol_container general_symbols = {.sym = NULL};

struct fbgc_object * initialize_fbgc_symbol_table(){
    //struct fbgc_tuple_object * table = (struct fbgc_tuple_object*) new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
   	fbgc_symbols = new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);

   	set_gc_black(fbgc_symbols);
	gc_mark_fbgc_object(fbgc_symbols);
	
    return fbgc_symbols;
}
struct fbgc_ll_base * new_fbgc_symbol_from_substr(const char * str1,const char * str2){
	
	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"Symbol table from substring\n");
	#endif


	//General symbol holder see symbol_table.h
	struct fbgc_object ** symbols = content_fbgc_tuple_object(fbgc_symbols);
	
	#ifdef SYMBOL_TABLE_DEBUG
		cprintf(111,"Trying to find str in old symbols\n");
	#endif

	for(size_t i = 0; i<size_fbgc_tuple_object(fbgc_symbols); i++){
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		// Find something proper, don't check the string size first but without checking the first match of n characters will return true 
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		if( strlen(content_fbgc_str_object(symbols[i])) == str2-str1 && !my_strncmp(content_fbgc_str_object(symbols[i]),str1,str2-str1)  ){

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

	struct fbgc_object * temp_obj =  new_fbgc_str_object_from_substr(str1,str2);

	#ifdef SYMBOL_TABLE_DEBUG
	cprintf(100,"OLD symbol table : ");
	//:>print_fbgc_object(cast_fbgc_object_as_field(field_obj)->symbols);
	print_fbgc_object(fbgc_symbols);
	cprintf(111,"\n");
	cprintf(110,"New symbol from s1:%s to s2:%s\n",str1,str2);
	cprintf(110,"Symbol has been created as cstring : ");
	print_fbgc_object(temp_obj);
	cprintf(111,"\n");
	#endif

	
	fbgc_symbols = push_back_fbgc_tuple_object(fbgc_symbols,temp_obj);

	//:>cast_fbgc_object_as_field(field_obj)->symbols = table;
	//fbgc_symbols = table;

	//table pointer may not be the same anymore
	symbols = content_fbgc_tuple_object(fbgc_symbols);

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