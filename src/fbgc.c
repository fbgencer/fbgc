#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"


static void compile_file(struct fbgc_object * main_field,const char *file_name){
#define MAX_INPUT_BUFFER 100
    char line[MAX_INPUT_BUFFER] = {0};
    FILE *input_file = fopen(file_name,"r");
    while (fbgc_getline_from_file(line, sizeof(line), input_file)){
        if(line[0] == ':' && line[1] == '>') continue; 
        if(line[0] != '\0') regex_lexer(&main_field,line);      
    }
    #ifdef LEXER_DEBUG
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main"); 
    #endif
    
    int par = parser(&main_field);
    
    #ifdef INTERPRETER_DEBUG
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");   
    #endif

    if(par) interpreter(&main_field); 
        
    fclose(input_file);    
}

static void compile_one_line(struct fbgc_object * main_field,char *line){
    
    if(regex_lexer(&main_field,line))      
        if(parser(&main_field))
            interpreter(&main_field);     
}


int main(){
	
   /* FILE *input_file = fopen("src/_fbgc_.txt","r");
    char line[1000] = {0};
    while (fgets(line, sizeof(line), input_file)){
        cprintf(111,"%s",line);     
    }    
    fclose(input_file);*/
    
	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 


	struct fbgc_object * main_field = new_fbgc_field_object();
	load_module_in_field_object(main_field,&fbgc_io_module);
	//load_module_in_field_object(main_field,&fbgc_math_module);

    //char *s = "print(1,2,3);";
    //compile_one_line(main_field,s);

    compile_file(main_field,"ex.fbgc");


 	free_fbgc_field_object(main_field);

	cprintf(110,"\n[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}

/*
 
fun fbg(x){
    return x + 5;  
}
fun,fbg,arg:x,x,5,+,ret,endfun

y := fbg(x){    
    y = x+5;
}
y,fbg,x,=,y,x,5,+,=,end

*/