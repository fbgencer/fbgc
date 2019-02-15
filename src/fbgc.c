#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"

int main(){
	

	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 


	struct fbgc_object * main_field = new_fbgc_field_object();
	load_module_in_field_object(main_field,&fbgc_io_module);
	load_module_in_field_object(main_field,&fbgc_math_module);
	//struct fbgc_object * ll = cast_fbgc_object_as_field(main_field)->modules;
	//struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)ll->next;
	//print_fbgc_cmodule(cm->module);

	char line[100] = {0};
	FILE *input_file = fopen("ex.fbgc","r");
    	
	//strcpy(line,"x  =123; y = (10,20,30); print(x,'\n',y,'\n',x+y,'\n');");
    	
    while (fgets(line, sizeof(line), input_file)){
    	if(line[0] == ':' && line[1] == '>') continue; 
    	//if(line[strlen(line)] == '\n') line[strlen(line)-1] = ';';
    	//if(line[strlen(line)] != ';') line[strlen(line)] = ';';
		regex_lexer(&main_field,line);   	
    }
	    print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");	
		parser(&main_field);  print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");	
		interpreter(&main_field); 
    
	
	
    fclose(input_file);
 	
 	free_fbgc_field_object(main_field);

	cprintf(110,"[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}

/*



*/

