#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define INITIAL_SYMBOL_TABLE_SIZE 16


struct general_symbol_container{
	struct fbgc_object * sym;
};

//XXXXXXXXXXXXXXXXXXXX
// General symbol holder
// Why making extern gives an error ? Investigate..
//XXXXXXXXXXXXXXXXXXXX
struct general_symbol_container general_symbols;

#define fbgc_symbols (general_symbols.sym)


struct fbgc_identifier{
	struct fbgc_object * name; // cstring name object  - key
	struct fbgc_object * content; // associated object	- value
};


struct fbgc_object * initialize_fbgc_symbol_table();
struct fbgc_ll_base * new_fbgc_symbol_from_substr(const char * str1,const char * str2);

void print_fbgc_symbol_table(struct fbgc_object * );
void free_fbgc_symbol_table(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif




/*



void print_fbgc_name_object(struct fbgc_object * obj){
   	
   	struct fbgc_name_object * nameo = cast_fbgc_object_as_name(obj);
    
    cprintf(011,"[");
    print_fbgc_object(nameo->name);
    if(nameo->content == NULL) cprintf(100,"NULL");
    else print_fbgc_object(nameo->content); 
}

*/