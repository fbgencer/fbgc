#include "fbgc.h"


int main(){
	

	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	/*struct fbgc_object * sym = new_fbgc_symbol_table();
	
	struct fbgc_object * ref = new_fbgc_ref_object();
	struct fbgc_object * ref2 = new_fbgc_ref_object();

	struct fbgc_object * ino = new_fbgc_int_object(88);

	
	const char * ss = "x fbg ahah";
	const char * s1 = ss+2;
	const char * s2 = s1+3;

	sym = new_fbgc_symbol_from_substr(sym,ref,s1,s2);
	sym = new_fbgc_symbol_from_substr(sym,ref2,s1,s2);
	
	cast_fbgc_object_as_str((cast_fbgc_object_as_ref(ref)->content))->base.next = ino;
	print_fbgc_object(ref);
	print_fbgc_object(ref2);

	free_fbgc_ref_object(ref2);
	free_fbgc_ref_object(ref);
	free_fbgc_symbol_table(sym);*/

	/*struct fbgc_object * head = new_fbgc_ll_object(LINKED_LIST);
	char line[100];
	FILE *input_file = fopen("ex.fbgc","r");
    
    while (fgets(line, sizeof(line), input_file)){
    	if(line[0] == ':' && line[1] == '>') continue; 
		if(!regex_lexer(&head,line)) return 0;
    }
    fclose(input_file);
    print_fbgc_ll_object(head,"Main");
	//head = parser(head); print_fbgc_ll_object(head,"Main");
	free_fbgc_ll_object(head);*/

	struct fbgc_object * main_field = new_fbgc_field_object();


	char line[100];
	FILE *input_file = fopen("ex.fbgc","r");
    
    while (fgets(line, sizeof(line), input_file)){
    	if(line[0] == ':' && line[1] == '>') continue; 
    	line[strlen(line)-1] = '\0';
		regex_lexer(&main_field,line);

	    print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");
		//parser(&main_field);  print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");

				
    }
    fclose(input_file);
 	
 	free_fbgc_field_object(main_field);
	cprintf(110,"[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}

/*



*/

