#include "fbgc.h"

#include "../cmodules/cmodules.h"

//Forward declarations
void print_logo();


struct fbgc_object * current_field = NULL;

struct fbgc_object __nil__ = {.type = NIL};
struct fbgc_object * __fbgc_nil = &__nil__;


static void compile_file(struct fbgc_object * main_field,const char *file_name){

	clock_t begin,end;
	double parser_time,interpreter_time;


	begin = clock();

	FILE *input_file = fopen(file_name,"r");
	if(input_file == NULL){
		cprintf(111,"file error\n");
	}
	 
   int par = parser(&main_field,input_file);

	fclose(input_file); 
	
	end = clock();
	parser_time = (double)(end - begin) / CLOCKS_PER_SEC; 

	 #ifdef INTERPRETER_DEBUG
		cprintf(111,"Parser output array\n");
		_print_fbgc_ll(cast_fbgc_object_as_field(main_field)->head,"Main");   
		cprintf(111,"\n");
	#endif

	//print_fbgc_memory_block();
	 
	begin = clock();
	if(par) 
		par = interpreter(&main_field); 
	end = clock();
	interpreter_time = (double)(end - begin) / CLOCKS_PER_SEC; 


	#ifdef TIME_DEBUG
	cprintf(110,"\n\n\n\n\n\n[=======================================================================]\n");   
	printf("Execution time [PARSER + LEXER] :%f\n",parser_time);    
	printf("Execution time [INTERPRETER] :%f\n",interpreter_time);  
	printf("Total exc. time %f\n",parser_time+interpreter_time);  
	#endif

	  
}




struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key){
		
	//load_key == 0, just return the module
	//load_key == 1, load all and return 
	//load_key == 2, load specific and return

	assert(current_field != NULL);

	const struct fbgc_cmodule * cm = NULL; 
	for(uint8_t i = 0; i<sizeof(__cmodules)/sizeof(__cmodules[0]); ++i){
		if(strcmp(module_name,__cmodules[i]->initializer->name) == 0){
			cm = __cmodules[i];
		}
	}

	if(cm == NULL){
		return NULL;
	}
	if(load_key != 0){
		//call function initializer
		cm->initializer->function(&current_field, 1); //need to send the address of current field

	 	const struct fbgc_cfunction * cc = cm->functions[0];
	 	for(int i = 1; cc!= NULL; i++){

			if(load_key == 2 && my_strcmp(fun_name,cc->name)){
				//if no match, 
				cc = cm->functions[i];
				continue;
			}
			add_variable_in_field_object(current_field,cc->name,new_fbgc_cfun_object(cc->function));
			if(load_key == 2) break;

			cc = cm->functions[i];
		}
	}

	return new_fbgc_cmodule_object(cm);

}

struct fbgc_object * fbgc_load_file(char * file_name,const char * fun_name, uint8_t load_key){

	FILE * input_file = fopen(file_name,"r");
	if(input_file == NULL){
		return NULL; 
	}

	struct fbgc_object * prev_field = current_field;	

	struct fbgc_object * file_field = new_fbgc_field_object();
	parser(&file_field,input_file);
	interpreter(&file_field);
	current_field = file_field;

	if(load_key != 0){
		//now merge two field objects, new field and prev field
		//prev_field_local_array_object
		struct fbgc_object * prev_ao = cast_fbgc_object_as_field(prev_field)->locals;
		//current field local array object
		struct fbgc_object * current_ao = cast_fbgc_object_as_field(current_field)->locals;

		printf("\n");

		unsigned int last_j = 0;

		char atleast_matched = 0;
		

		for(unsigned int i = 0;  i<size_fbgc_array_object(current_ao); ++i){	
			struct fbgc_identifier * ctemp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(current_ao,i);
			struct fbgc_identifier * ptemp_id;
			char found = 0;

			//cprintf(111,"ctemp_id name:[%s]fun:[%s]\n",content_fbgc_cstr_object(ctemp_id->name),fun_name);
			//cprintf(100,"result %d\n",my_strcmp(fun_name,content_fbgc_cstr_object(ctemp_id->name)));
			if(load_key == 2 && my_strcmp(fun_name,content_fbgc_cstr_object(ctemp_id->name)) ){
				//if no match
				continue;
			}

			//so matched with the given function name
			atleast_matched = 1;

			for(unsigned int j = last_j; j<size_fbgc_array_object(prev_ao); ++j){
				ptemp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(prev_ao,j);
				//cprintf(111,"ptemp_id name :"); print_fbgc_object(ptemp_id->name);
				//cprintf(111," | ctemp_id name :"); print_fbgc_object(ctemp_id->name);
				
				if(!my_strcmp(content_fbgc_cstr_object(ptemp_id->name),content_fbgc_cstr_object(ctemp_id->name))){
					//change the content of ptemp_id object with the second one
					found = 1;
					last_j = ++j;
					//cprintf(100," (Equal)\n");
					break;
				}
			}

			if(found){
				//cprintf(101,"Found!\n");
				ptemp_id->content = ctemp_id->content;
			}else{			
				//cprintf(101,"creating new local\n");
				prev_ao = push_back_fbgc_array_object(prev_ao,ctemp_id);	
			}
			if(load_key == 2) break;
		}
		cast_fbgc_object_as_field(prev_field)->locals = prev_ao;
		

		if(!atleast_matched){
			cprintf(100,"Error '%s' could not be found in '%s'\n",fun_name,file_name);
		}
		assert(atleast_matched);
	}

	current_field = prev_field;
	return file_field;
}


void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
}

#ifndef MODULE_TEST
int main(int argc, char **argv){

	#ifdef FBGC_LOGO
		print_logo();
	#endif   



//******************************************************************
	initialize_fbgc_memory_block();
	initialize_fbgc_symbol_table();
	struct fbgc_object * main_field = new_fbgc_field_object();
	current_field = main_field;
	//load_module_in_field_object(main_field,&fbgc_math_module);
	//load_module_in_field_object(main_field,&fbgc_file_module);
	

	clock_t begin1,begin2;
	double loop_time = 0, struct_time = 0;

	struct fbgc_object * q = new_fbgc_int_object(88);
	
	

	/*struct fbgc_object_property_holder * p = get_fbgc_object_property_holder(q);
	uint32_t bits = p->bits;
	uint32_t bit_conf = _BIT_TO_INT & bits;

	printBits(sizeof(bits),&bits);printf("<<\n");
	printBits(sizeof(bit_conf),&bit_conf);printf("<<\n");

	int8_t w = _find_property(p->bits,_BIT_DESTRUCTOR);
	printf("w = %d\n",w );

	//return p->properties[2].print(self);*/

	long boom = 100000000;

	//#define FOO

	#ifdef FOO
	begin2 = clock();	
	for(int i = 0; i<boom; ++i)
		myprint_fbgc_object(q);
	struct_time = (double)(clock() - begin2) / CLOCKS_PER_SEC;
	#else	

	begin1 = clock();
	for(int i = 0; i< boom; ++i)
		print_fbgc_object(q);

	loop_time = (double)(clock() - begin1) / CLOCKS_PER_SEC; 
	#endif

	printf("Loop time :%g | Struct time :%g\n",loop_time,struct_time );

	/*printf("bit:%04x\n",fbgc_int_object_property_holder.bits);
	printf("Counting bits...\n");

	uint8_t where = _LOC_DESTRUCTOR;
	int8_t count = 0;
	uint32_t bit = fbgc_int_object_property_holder.bits;

	count = _find_property(fbgc_int_object_property_holder.bits,where);

	//printf("is prop %d, %d %s\n",(0x0001<<where)? "yes":"no" );
	printf("Count:%d\n",count);*/
	
	/*

	fbgc_int_object_property_holder.properties[1].print(q);

	printf("\n%p:holder start\n",&fbgc_int_object_property_holder);
	printf("%p:bits start\n",&fbgc_int_object_property_holder.bits );
	printf("%p:properties0\n",&fbgc_int_object_property_holder.properties[0]);
	printf("%p:properties1\n",&fbgc_int_object_property_holder.properties[1]);
	printf("%p:properties1\n",&fbgc_int_object_property_holder.properties[2]);
	printf("%p:properties1\n",&fbgc_int_object_property_holder.properties[3]);
	printf("Sizeof %lu\n",sizeof(fbgc_int_object_property_holder));*/
	
	//regex_lexer(&main_field,"1+2+3");
	//compile_file(main_field, "ex.fbgc");""
	/*
	if(argc > 1)
	{   
		if(!strcmp(argv[1],"-s")){
			//compile_one_line(main_field,argv[2]);
		}
		else{
		   compile_file(main_field, argv[1]); 
		}
		//print_fbgc_memory_block();
		
	}
	else{
	   //realtime_fbgc(main_field);
	}*/

	free_fbgc_memory_block();
//******************************************************************

	return 0;
}


void print_logo(){
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen ( "./src/_fbgc_.txt" , "r" );
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	printf("%s\n",buffer);

	// terminate
	fclose (pFile);
	free (buffer);

}

#endif

