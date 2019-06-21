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
        cprintf(111,"Lexer output array\n");
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main"); 
        cprintf(111,"\n");
    #endif
    
    int par = parser(&main_field);
    
    #ifdef INTERPRETER_DEBUG
        cprintf(111,"Parser output array\n");
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");   
        cprintf(111,"\n");
    #endif

    if(par) interpreter(&main_field); 
        
    fclose(input_file);    
}

static void compile_one_line(struct fbgc_object * main_field,char *line){
    
    if(regex_lexer(&main_field,line))      
        if(parser(&main_field))
            interpreter(&main_field);     
}

int main(int argc, char **argv){
	
   /* FILE *input_file = fopen("src/_fbgc_.txt","r");
    char line[1000] = {0};
    while (fgets(line, sizeof(line), input_file)){
        cprintf(111,"%s",line);     
    }    
    fclose(input_file);*/
    
	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

    
    initialize_fbgc_memory_block();
   
    struct fbgc_double_object *dbo = (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = -345623.789;  
    struct fbgc_object * db = (struct fbgc_object*) dbo; 

    struct fbgc_int_object * ino = (struct fbgc_int_object*) fbgc_malloc(size_fbgc_int_object);
    ino->base.type = INT;
    ino->base.next = NULL;
    ino->content = 0x00000000FB6C;     
    struct fbgc_object * db2 = (struct fbgc_object*) ino; 

    char *str1 = "abcdefghijklmno";
    char *str2 = str1+strlen(str1);

    struct fbgc_str_object *stro = fbgc_malloc(size_fbgc_str_object); 
    stro->base.type = STRING;
    stro->base.next =NULL;
    stro->content = (char *) fbgc_malloc(strlen(str1)+1);
    memcpy(stro->content,str1,(str2-str1));
    stro->content[str2-str1] = '\0';
    struct fbgc_object * sto = (struct fbgc_object*) stro;

    dbo = (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = -987564.123456;     
    struct fbgc_object * db3 = (struct fbgc_object*) dbo; 


    
    print_fbgc_object(db);   printf("\n");
    print_fbgc_object(db2); printf("\n");
    print_fbgc_object(db3); printf("\n");
    print_fbgc_object(sto); printf("\n");



    print_fbgc_memory_block();
    free_fbgc_memory_block();




    //struct fbgc_object * db = new_fbgc_double_object(3.1415973);
    //print_fbgc_object(db);
    //free_fbgc_object(db);

    //cprintf(111,"\nSize of double object %d\n",sizeof(struct fbgc_double_object));



/*
    int size = 20;
    void * h = (void *) malloc(size*1); 

    int *q = h;
    *q = 1234567891011121314;   
    int *q2 = h + sizeof(int);
    *q2 = 0x00000000000000FB6C;

    free(h);
    printf("\n");


    for(int i = 0; i<size; i++){
        int val = *( char *)(h+i); 
        cprintf(111,"[%d]:%0x\n",i,val);
        //else cprintf(111,"%d|",i);
    }    
    cprintf(111,"\n");
    */

    /*struct fbgc_double_object *dbo = (struct fbgc_double_object*) (h);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = -345623.789;  
    struct fbgc_object * db = (struct fbgc_object*) dbo; 

    dbo = (struct fbgc_double_object*) (h+24);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = 12345678.901234;     
    struct fbgc_object * db2 = (struct fbgc_object*) dbo; 

    dbo = (struct fbgc_double_object*) (h+48);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = -987564.123456;     
    struct fbgc_object * db3 = (struct fbgc_object*) dbo; */

/*    char *str1 = "abcdefghijklmno";
    char *str2 = str1+strlen(str1);

    struct fbgc_str_object *stro = (h); 
    stro->base.type = STRING;
    stro->base.next =NULL;
    stro->content = (char *) (h+sizeof(struct fbgc_str_object));
    memcpy(stro->content,str1,(str2-str1));
    stro->content[str2-str1] = '\0';
    struct fbgc_object * sto = (struct fbgc_object*) stro;

    
   // print_fbgc_object(db);   printf("\n");
    //print_fbgc_object(db2); printf("\n");
    //print_fbgc_object(db3); printf("\n");
    print_fbgc_object(sto); printf("[%d]\n",sizeof(struct fbgc_str_object));

*/



    //struct fbgc_double_object *dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    //dbo->base.type = DOUBLE;
    //dbo->base.next = NULL;
    //dbo->content = db_content; 
     //return (struct fbgc_object*) dbo;


	/*struct fbgc_object * main_field = new_fbgc_field_object();
	load_module_in_field_object(main_field,&fbgc_io_module);
	//load_module_in_field_object(main_field,&fbgc_math_module);

    //char *s = "print(1,2,3);";
    //compile_one_line(main_field,s);

    
    if(argc>1)
        compile_file(main_field, argv[1]);
    else
        cprintf(111,"Enter a file!\n");

 	free_fbgc_field_object(main_field);*/

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
