#include "fbgc.h"

#include "../cmodules/cmodules.h"

struct fbgc_object * current_field = NULL;
const struct fbgc_object __nil__ = {.type = NIL, .next=NULL};
const struct fbgc_object * const __fbgc_nil = &__nil__;


static void compile_file(struct fbgc_object * main_field,const char *file_name){

	clock_t begin,end;
	double parser_time,interpreter_time;

	#ifdef LEXER_DEBUG
		cprintf(111,"Lexer output array\n");
		print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main"); 
		cprintf(111,"\n");
	#endif

	begin = clock();

	FILE *input_file = fopen("ex.fbgc","r");
	if(input_file == NULL){
		cprintf(111,"file error\n");
	}
	 
   int par = parser(&main_field,input_file);

	fclose(input_file); 
	
	end = clock();
	parser_time = (double)(end - begin) / CLOCKS_PER_SEC; 

	  #ifdef INTERPRETER_DEBUG
		cprintf(111,"Parser output array\n");
		print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main");   
		cprintf(111,"\n");
	#endif

	//print_fbgc_memory_block();
	 
	begin = clock();
	if(par) 
		interpreter(&main_field); 
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



int main(int argc, char **argv){




//******************************************************************
	initialize_fbgc_memory_block();
	initialize_fbgc_symbol_table();
	struct fbgc_object * main_field = new_fbgc_field_object();
	current_field = main_field;
	 //load_module_in_field_object(main_field,&fbgc_math_module);

	//load_module_in_field_object(main_field,&fbgc_file_module);
	

	//compile_file(main_field, "ex.fbgc");

	/*if(argc > 1)
	{   
		if(!strcmp(argv[1],"-s")){
			compile_one_line(main_field,argv[2]);
		}
		else{
		   compile_file(main_field, argv[1]);
		}
		//print_fbgc_memory_block();
		
	}
	else{
	   realtime_fbgc(main_field);
	}*/

	free_fbgc_memory_block();
//******************************************************************

	return 0;
}



