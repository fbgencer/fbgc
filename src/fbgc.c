#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"


static void compile_file(struct fbgc_object * main_field,const char *file_name){
#define MAX_INPUT_BUFFER 1000
    
    clock_t begin,end;
    double lexer_time,parser_time,interpreter_time;


    begin = clock();

    char line[MAX_INPUT_BUFFER] = {0};
    FILE *input_file = fopen(file_name,"r");
    while (fbgc_getline_from_file(line, sizeof(line), input_file)){
        if(line[0] == ':' && line[1] == '>') continue; 
        if(line[0] != '\0') regex_lexer(&main_field,line);      
    }
    fclose(input_file); 
    end = clock();
    lexer_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    #ifdef LEXER_DEBUG
        cprintf(111,"Lexer output array\n");
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main"); 
        cprintf(111,"\n");
    #endif

    begin = clock();

     int par = parser(&main_field);
    
    end = clock();
    parser_time = (double)(end - begin) / CLOCKS_PER_SEC; 

      #ifdef PARSER_DEBUG
        cprintf(111,"Parser output array\n");
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");   
        cprintf(111,"\n");
    #endif

    //print_fbgc_memory_block();
     
    begin = clock();
     //if(par) 
    interpreter(&main_field); 
    end = clock();
    interpreter_time = (double)(end - begin) / CLOCKS_PER_SEC; 

    cprintf(110,"\n\n\n\n\n\n[=======================================================================]\n"); 
    printf("Execution time [LEXER] :%f\n",lexer_time);    
    printf("Execution time [PARSER] :%f\n",parser_time);    
    printf("Execution time [INTERPRETER] :%f\n",interpreter_time);  
    printf("Total ex time %f\n",lexer_time+parser_time+interpreter_time);  
        
      
}

static void compile_one_line(struct fbgc_object * main_field,char *line){
    
    if(regex_lexer(&main_field,line))      
        if(parser(&main_field))
            interpreter(&main_field);     
}

/*
asagidaki sekilde garbage objesi 42lik yer tutuyor ama
bu 42lik yeri allocate etmek için 42-gb_size kadar bir allocation lazım
bazı durumlarda bu mümkün olmayabilir
bunun icin gereken nedir ?

*/


/*
    15.07.19 tuple & memory example code
    struct fbgc_tuple_object *to =  (struct fbgc_tuple_object*) fbgc_malloc(sizeof(struct fbgc_tuple_object));
    to->base.type = TUPLE;
    to->base.next = NULL;
    to->size = 5;
    print_fbgc_memory_block();
    fbgc_malloc(sizeof(struct fbgc_object*)*to->size);
    //cprintf(100,"TUPLE opened size:%d\n",size);
    print_fbgc_memory_block();

 //   struct fbgc_object *tp = new_fbgc_tuple_object(5); 
    struct fbgc_object * ino = new_fbgc_int_object(0x00000000ABCD);
    set_in_fbgc_tuple_object(to,ino,3);
    print_fbgc_memory_block();
    
*/



int main(int argc, char **argv){
    
cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

//******************************************************************
    if(argc>1)
    {
        initialize_fbgc_memory_block();
        initialize_fbgc_symbol_table();


        /*struct fbgc_object * to = new_fbgc_tuple_object(4);
        to = push_back_fbgc_tuple_object(to,new_fbgc_int_object(10));
        to = push_back_fbgc_tuple_object(to,new_fbgc_int_object(100));
        to = push_back_fbgc_tuple_object(to,new_fbgc_int_object(1000));
        to = push_back_fbgc_tuple_object(to,new_fbgc_int_object(98765));

        struct fbgc_object ** contents = tuple_object_content(to);
        struct fbgc_object * cto =  new_fbgc_const_tuple_object_from_tuple_content(contents+2,2);

        print_fbgc_tuple_object(cto);*/

        struct fbgc_object * main_field = new_fbgc_field_object();
        load_module_in_field_object(main_field,&fbgc_math_module);
        load_module_in_field_object(main_field,&fbgc_io_module);
        compile_file(main_field, argv[1]);
        //print_fbgc_memory_block();

        /*char buffer [50];
        int n, a=5, b=3;
        n = sprintf(buffer, "%%%d0d",5);
        printf(buffer,3);
    */
        free_fbgc_memory_block();    
    }
    else{
        cprintf(111,"Enter a file!\n");
    }
//******************************************************************

	cprintf(110,"\n[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}


