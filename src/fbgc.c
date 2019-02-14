#include "fbgc.h"


int main(){
	

	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	/*struct fbgc_object *lo = new_fbgc_double_object(31.2);
	
	//	cprintf(101,"%s",fbgc_io_module.functions[1]->name);
	struct fbgc_object *funo = new_fbgc_cfun_object(fbgc_io_module.functions[1]->function);

	print_fbgc_object(funo);
	cfun_object_call(funo,lo);


	free_fbgc_object(lo);
	free_fbgc_object(funo);*/

	struct fbgc_object * main_field = new_fbgc_field_object();


	char line[100] = {0};
	FILE *input_file = fopen("ex.fbgc","r");
    	
	strcpy(line,"print('fbg:',3.313,'\n','But he know what he is going to do');");
    	
   // while (fgets(line, sizeof(line), input_file)){
    	printf("%s",line);
    	//if(line[0] == ':' && line[1] == '>') continue; 
    	//if(line[strlen(line)] == '\n') line[strlen(line)-1] = ';';
    	//if(line[strlen(line)] != ';') line[strlen(line)] = ';';
		regex_lexer(&main_field,line);   	
   // }
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

