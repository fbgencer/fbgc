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
struct fbgc_object * new_fbgc_symbol(struct fbgc_object * ,const char * ,const char * );
struct fbgc_object * get_fbgc_symbol(struct fbgc_object * );

struct fbgc_object * new_fbgc_symbol_from_substr(const char * str1,const char * str2);
struct fbgc_object * new_cfun_object_from_substr(struct fbgc_object * table_obj,const char * str1,const char * str2);

struct fbgc_object * load_module_in_symbol_table(struct fbgc_object *,struct fbgc_object * );

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