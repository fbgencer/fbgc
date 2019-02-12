#include "fbgc.h"


int main(){
	

	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 


    /*
		[1,2]
		1,2,2,vector
		[[1,2]]
		1,2,2,vector,1,vector,
		[1,2,3;7,8,9]
		1 2 3 row(3) 7 8 9 4 vector 
		
		*[1,2;3,4;5,6]
		1 2 row 3 4 row 5 6 6 vector
		Stack: 6,6,5,row,4,3,row,2,1
	
		*[1;5] sonuncu virgul mu kontrol et!
		1 row 5 2 vector

		[[10,20];[30,40]] = [[10,20;30,40]]
		10 20 2 vector row 30 40 2 vector 2 vector
		
		[[10,20;30,40]]
		10,20,row,30,40,4,vector,1,vector

		[1,[2,3];[4,5],6]
		 


		
    */



	struct fbgc_object * main_field = new_fbgc_field_object();


	char line[100] = {0};
	FILE *input_file = fopen("ex.fbgc","r");
    
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

