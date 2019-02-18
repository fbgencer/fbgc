#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"


static void compile_file(struct fbgc_object * main_field,const char *file_name){
#define MAX_INPUT_BUFFER 100
    char line[MAX_INPUT_BUFFER] = {0};
    FILE *input_file = fopen(file_name,"r");
    while (fbgc_getline_from_file(line, sizeof(line), input_file)){
        if(line[0] == ':' && line[1] == '>') continue; 
        regex_lexer(&main_field,line);      
    }
    //print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main"); 
    parser(&main_field);
   // print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");   
    interpreter(&main_field); 
        
    fclose(input_file);    
}


int main(){
	
	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	struct fbgc_object * main_field = new_fbgc_field_object();
	load_module_in_field_object(main_field,&fbgc_io_module);
	//load_module_in_field_object(main_field,&fbgc_math_module);

   compile_file(main_field,"ex.fbgc");
 	


 	free_fbgc_field_object(main_field);

	cprintf(110,"\n[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}