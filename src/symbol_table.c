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

void initialize_fbgc_symbol_table(){
	FBGC_LOGV(SYMBOL_TABLE,"Allocation new symbol table capacity :%u\n",INITIAL_SYMBOL_TABLE_SIZE);
   	fbgc_symbols = new_fbgc_tuple_object(INITIAL_SYMBOL_TABLE_SIZE);
}
struct fbgc_ll_base * new_fbgc_symbol_from_substr(const char * str1,const char * str2){

	assert(fbgc_symbols);

	//General symbol holder see symbol_table.h
	struct fbgc_object ** symbols = content_fbgc_tuple_object(fbgc_symbols);

	printf("fooo Size %d\n",size_fbgc_tuple_object(fbgc_symbols));
	
	for(size_t i = 0; i<size_fbgc_tuple_object(fbgc_symbols); i++){
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		// Find something proper, don't check the string size first but without checking the first match of n characters will return true 
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		if( strlen(content_fbgc_str_object(symbols[i])) == str2-str1 && !my_strncmp(content_fbgc_str_object(symbols[i]),str1,str2-str1)  ){
			return _new_fbgc_ll_identifier(i); 
		}
	}

	struct fbgc_object * temp_obj =  new_fbgc_str_object_from_substr(str1,str2);	
	push_back_fbgc_tuple_object(fbgc_symbols,temp_obj);
	return _new_fbgc_ll_identifier(size_fbgc_tuple_object(fbgc_symbols)-1);
}


void print_fbgc_symbol_table(struct fbgc_object * ll){


	cprintf(010,"[~~~~~~~~~~~~[SYMBOL TABLE]~~~~~~~~~~~~~]\n");
//	print_fbgc_ll_object(ll,"SYMBOLS:");
	cprintf(110,"\n");
	cprintf(010,"[~~~~~~~~~~~~~~~~~~~~~~~~~]\n");

}