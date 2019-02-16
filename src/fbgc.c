#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"



char * fbgc_getline_from_file(char * s, int n, FILE *fp){
   	int c;
    char* cs = s;
    while(--n > 0 && (c = getc(fp)) != EOF){
    // put the input char into the current pointer position, then increment it
    // if a newline entered, break
    	switch((*cs++ = c)){
    		case '\n': goto end_of_getline;
    		case '\\': 
    			switch( c = getc(fp)){
					case 'a': *(cs-1) = '\a'; break;
					case 'b': *(cs-1) = '\b'; break;
					case 'e': *(cs-1) = '\e'; break;
					case 'f': *(cs-1) = '\f'; break;
					case 'n': *(cs-1) = '\n'; break;
					case 'r': *(cs-1) = '\r'; break;
					case 't': *(cs-1) = '\t'; break;
					case 'v': *(cs-1) = '\v'; break;
					//case '\'': *(cs) = '\''; break;
					case '\\': *(cs-1) = '\\'; break;     				
    				case EOF: goto end_of_getline;	
    				default:
    					*cs++ = c;
    						
    			} 
    		break;	
    	}          
    }
    end_of_getline:
    *cs = '\0';
    return (c == EOF && cs == s) ? NULL : s;
}

int main(){
	
	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	/*char str[] = "trying to move this:\\' here: <end of the line";
	for(int i = 0; i<strlen(str); printf("%d:%c\n",i,str[i]),i++);
  	memmove (str+20,str+21,44-21);
  	puts (str);*/

	struct fbgc_object * main_field = new_fbgc_field_object();
	//load_module_in_field_object(main_field,&fbgc_io_module);
	//load_module_in_field_object(main_field,&fbgc_math_module);
	
	//struct fbgc_object * ll = cast_fbgc_object_as_field(main_field)->modules;
	//struct fbgc_cmodule_object * cm = (struct fbgc_cmodule_object *)ll->next;
	//print_fbgc_cmodule(cm->module);

	char line[100] = {0};
	FILE *input_file = fopen("ex.fbgc","r");
    while (fbgc_getline_from_file(line, sizeof(line), input_file)){
    	cprintf(100,"%s\n",line);
    	if(line[0] == ':' && line[1] == '>') continue; 
    	//if(line[strlen(line)] == '\n') line[strlen(line)-1] = ';';
    	//if(line[strlen(line)] != ';') line[strlen(line)] = ';';
		regex_lexer(&main_field,line);   	
    }
	//print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");	
	//parser(&main_field);  print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");	
	//interpreter(&main_field); 
    	
    fclose(input_file);
 		
 	free_fbgc_field_object(main_field);

	cprintf(110,"\n[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}