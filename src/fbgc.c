#include "fbgc.h"
#include "../cmodules/fbgc_io.h"
#include "../cmodules/fbgc_math.h"


static void compile_file(struct fbgc_object * main_field,const char *file_name){
#define MAX_INPUT_BUFFER 1000
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

    //print_fbgc_memory_block();
        
    int par = parser(&main_field);
    
    #ifdef INTERPRETER_DEBUG
        cprintf(111,"Parser output array\n");
        print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");   
        cprintf(111,"\n");
    #endif

    //print_fbgc_memory_block();

    if(par) interpreter(&main_field); 
        
    fclose(input_file);    
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
    set_object_in_fbgc_tuple_object(to,ino,3);
    print_fbgc_memory_block();
    
*/


int main(int argc, char **argv){
    
cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 


//******************************************************************


    
    /*initialize_fbgc_memory_block();
    struct fbgc_object * ino = new_fbgc_int_object(0x00000000FB6C);
    struct fbgc_object * dbo = new_fbgc_double_object(3.14159);
    struct fbgc_object * tpo = new_fbgc_tuple_object(7);
    tpo = push_back_fbgc_tuple_object(tpo,ino);

    print_fbgc_memory_block();

    tpo = push_back_fbgc_tuple_object(tpo,dbo);
    
    
    print_fbgc_memory_block();

    struct fbgc_object * ino1 = new_fbgc_int_object(0x00000000ABCD);
    struct fbgc_object * ino2 = new_fbgc_int_object(0x00000000ABABABDC);

   tpo = push_back_fbgc_tuple_object(tpo,ino1);
   tpo = push_back_fbgc_tuple_object(tpo,ino2);   

    print_fbgc_memory_block();

    free_fbgc_memory_block();*/
    
    




 //   cprintf(111,"x:%d\n",5*sizeof(struct fbgc_object*) + sizeof(struct fbgc_tuple_object));

 /*   char *str1 = "abc";
    struct fbgc_str2_object *stro = fbgc_malloc(size_fbgc_str_object + strlen(str1) + 1); 
    stro->base.type = STRING;
    stro->len = strlen(str1);
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    //set_mark_bit_one(stro->base.type);
    print_fbgc_memory_block(); 


    dbo = (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->content = -987564.123456;
    set_mark_bit_one(dbo->base.type);
    print_fbgc_memory_block();

    str1 = "aaa|bbb|ccc|ddd|eee|fff|aaa|bbb|ccc|ddd|eee|fff";
    stro = fbgc_malloc(size_fbgc_str_object + strlen(str1) + 1); 
    stro->base.type = STRING;
    stro->len = strlen(str1);
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    //set_mark_bit_one(stro->base.type);
    print_fbgc_memory_block(); 

    dbo = (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->content = -987564.123456;
    set_mark_bit_one(dbo->base.type);
    print_fbgc_memory_block();


    str1 = "123|4";
    stro = fbgc_malloc(size_fbgc_str_object + strlen(str1) + 1); 
    stro->base.type = STRING;
    stro->len = strlen(str1);
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    set_mark_bit_one(stro->base.type);
    print_fbgc_memory_block(); 

    str1 = "aaaaaaaaaaaaaaa";
    stro = fbgc_malloc(size_fbgc_str_object + strlen(str1) + 1); 
    stro->base.type = STRING;
    stro->len = strlen(str1);
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    set_mark_bit_one(stro->base.type);
    print_fbgc_memory_block(); 
*/

  /*  str1 = "a";
    stro = fbgc_malloc(size_fbgc_str_object + strlen(str1) + 1); 
    stro->base.type = STRING;
    stro->len = strlen(str1);
    stro->content = ( (char *) stro ) + size_fbgc_str_object;
    memcpy(&stro->content,str1,stro->len);
    *(&stro->content+stro->len) = '\0';
    //set_mark_bit_one(stro->base.type);
    print_fbgc_memory_block(); */

 /*   dbo = (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->content = -987564.123456;    
    print_fbgc_memory_block();

    struct fbgc_int_object * ino = (struct fbgc_int_object*) fbgc_malloc(size_fbgc_int_object);
    ino->base.type = INT;
    ino->content = 0x00000000ABCD;     
    set_mark_bit_one(ino->base.type);  
    print_fbgc_memory_block();

*/



    /*
    ino = (struct fbgc_int_object*) fbgc_malloc(size_fbgc_int_object);
    ino->base.type = INT;
    ino->base.next = NULL;
    ino->content = 0x00000000FB6C;     
    print_fbgc_memory_block();
    */

/*

    dbo = (struct fbgc_double_object*) fbgc_malloc(size_fbgc_double_object);
    dbo->base.type = DOUBLE;
    dbo->base.next = NULL;
    dbo->content = -987564.123456;     
    struct fbgc_object * db3 = (struct fbgc_object*) dbo; 
    print_fbgc_memory_block();*/
    

    
  //  print_fbgc_object(db);   printf("\n");
  //  print_fbgc_object(db2); printf("\n");
   // print_fbgc_object(db3); printf("\n");
  //  print_fbgc_object(sto); printf("\n");






//******************************************************************


    //struct fbgc_double_object *dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    //dbo->base.type = DOUBLE;
    //dbo->base.next = NULL;
    //dbo->content = db_content; 
     //return (struct fbgc_object*) dbo;

	//load_module_in_field_object(main_field,&fbgc_io_module);
	//load_module_in_field_object(main_field,&fbgc_math_module);

   // char *s = "abcdef=1;abcdef=10.2;";
   // compile_one_line(main_field,s);

    //print_fbgc_memory_block();

    if(argc>1){

        initialize_fbgc_memory_block();

        struct fbgc_object * main_field = new_fbgc_field_object();

        compile_file(main_field, argv[1]);
        //print_fbgc_memory_block();
        free_fbgc_memory_block();    
    }
    else
        cprintf(111,"Enter a file!\n");

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
