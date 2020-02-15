#include "fbgc.h"

#include "../cmodules/cmodules.h"



static void compile_file(struct fbgc_object * main_field,const char *file_name){
#define MAX_INPUT_BUFFER 1000
	
	clock_t begin,end;
	double lexer_time,parser_time,interpreter_time;


	char line[MAX_INPUT_BUFFER] = {0};
/*	FILE *input_file = fopen(file_name,"r");
	 
	begin = clock();
	while (fbgc_getline_from_file(line, sizeof(line), input_file)){
		cprintf(111,"line[%s]\n",line);
		//if(line[0] == '#') continue; //past the comment fast
		//if(line[0] != '\0') regex_lexer(&main_field,line);
	}
	end = clock();
	fclose(input_file); 
	lexer_time = (double)(end - begin) / CLOCKS_PER_SEC; 

*/
	#ifdef LEXER_DEBUG
		cprintf(111,"Lexer output array\n");
		print_fbgc_ll_object(cast_fbgc_object_as_field(main_field)->head,"Main"); 
		cprintf(111,"\n");
	#endif

	begin = clock();

	FILE *input_file = fopen("ex.fbgc","r");
	if(input_file == NULL){
		cprintf(111,"file error\n");
		return 0;
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
	printf("Execution time [LEXER] :%f\n",lexer_time);    
	printf("Execution time [PARSER] :%f\n",parser_time);    
	printf("Execution time [INTERPRETER] :%f\n",interpreter_time);  
	printf("Total ex time %f\n",lexer_time+parser_time+interpreter_time);  
	#endif

	  
}

static void compile_one_line(struct fbgc_object * main_field,char *line){
	
	//regex_lexer(&main_field,line);      
	//parser(&main_field);
	//interpreter(&main_field);     
}


void realtime_fbgc(struct fbgc_object * main_field){
}

struct fbgc_object * main_field;




struct fbgc_object * fbgc_load_module(const char * module_name,const char * fun_name, uint8_t load_key){
		
	//load_key == 0, just return the module
	//load_key == 1, load all and return 
	//load_key == 2, load specific and return

	add_variable_in_field_object(main_field,"pi",new_fbgc_double_object(3.14159));

	struct fbgc_cmodule * cm = NULL; 


	for(uint8_t i = 0; i<sizeof(__cmodules)/sizeof(__cmodules[0]); ++i){
		if(strcmp(module_name,__cmodules[i]->initializer->name) == 0){
			cm = __cmodules[i];
		}
	}

	if(cm == NULL){
		cprintf(100,"Undefined module!\n");
		return NULL;
	}

	if(load_key != 0){
		

	 	const struct fbgc_cfunction * cc = cm->functions[0];
	 	for(int i = 1; cc!= NULL; i++){

			if(load_key == 2 && my_strcmp(fun_name,cc->name)){
				//if no match, 
				cc = cm->functions[i];
				continue;
			}

			add_variable_in_field_object(main_field,cc->name,new_fbgc_cfun_object(cc->function));
			// struct fbgc_object *rhs = new_fbgc_cfun_object(cc->function);
			// const char * str1 = cc->name;
			// struct fbgc_object * iter = new_fbgc_symbol_from_substr(str1,str1 + strlen(str1));
			// 	//this location is from symbols, we need to find location in fields
			// struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,cast_fbgc_object_as_id_opcode(iter)->loc);


			// struct fbgc_object * local_array = cast_fbgc_object_as_field(main_field)->locals;
			// struct fbgc_identifier * temp_id; 
			// int where = -1;

			// for(int i = 0; i<size_fbgc_array_object(local_array); i++){
			// 	temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
			// 	if(temp_id->name == cstr_obj) {
			// 		where = i;
			// 		break;
			// 	} 
			// }

			// if(where == -1){

			// 	struct fbgc_identifier id;		
			// 	id.name = cstr_obj; id.content = rhs;
			// 	local_array = push_back_fbgc_array_object(local_array,&id);
			// 	where = size_fbgc_array_object(local_array)-1;
			// 	cast_fbgc_object_as_field(main_field)->locals = local_array;
			// }else{

			// 	cast_fbgc_object_as_id_opcode(temp_id)->loc = where;
			// 	temp_id->content = rhs;
			// }
			// set_id_flag_GLOBAL(iter);

			if(load_key == 2) break;

			cc = cm->functions[i];
		}
	}


	return new_fbgc_cmodule_object(cm);
}





// struct fbgc_object * fbgc_load_module_specific(const char * module_name, const char * fun_name){
		

// 	struct fbgc_cmodule * cm = NULL; 
// 	int load_key = 1;

// 	for(uint8_t i = 0; i<sizeof(__cmodules)/sizeof(__cmodules[0]); ++i){
// 		if(strcmp(module_name,__cmodules[i]->initializer->name) == 0){
// 			cm = __cmodules[i];
// 		}
// 	}

// 	if(cm == NULL){
// 		cprintf(100,"Undefined module!\n");
// 		return NULL;
// 	}


// 	if(load_key == 1){
// 		const struct fbgc_cfunction * cc = cm->functions[0];
// 	 	for(int i = 1; cc!= NULL; i++){
// 			struct fbgc_object *rhs = new_fbgc_cfun_object(cc->function);



// 			const char * str1 = cc->name;
// 			struct fbgc_object * iter = new_fbgc_symbol_from_substr(str1,str1 + strlen(str1));
// 				//this location is from symbols, we need to find location in fields
// 			struct fbgc_object * cstr_obj = get_object_in_fbgc_tuple_object(fbgc_symbols,cast_fbgc_object_as_id_opcode(iter)->loc);


// 			struct fbgc_object * local_array = cast_fbgc_object_as_field(main_field)->locals;
// 			struct fbgc_identifier * temp_id; 
// 			int where = -1;

// 			for(int i = 0; i<size_fbgc_array_object(local_array); i++){
// 				temp_id = (struct fbgc_identifier *) get_address_in_fbgc_array_object(local_array,i);
// 				if(temp_id->name == cstr_obj) {
// 					where = i;
// 					break;
// 				} 
// 			}

// 			if(where == -1){

// 				struct fbgc_identifier id;		
// 				id.name = cstr_obj; id.content = rhs;
// 				local_array = push_back_fbgc_array_object(local_array,&id);
// 				where = size_fbgc_array_object(local_array)-1;
// 				cast_fbgc_object_as_field(main_field)->locals = local_array;
// 			}else{

// 				cast_fbgc_object_as_id_opcode(temp_id)->loc = where;
// 				temp_id->content = rhs;
// 			}
// 			set_id_flag_GLOBAL(iter);

// 			cc = cm->functions[i];
// 		}
// 	}

// 	if(cm != NULL){
// 		return new_fbgc_cmodule_object(cm);
// 	}
// 	return NULL;
// }



/*
struct fbgc_object * module_deneme(const char * x){


	


				struct fbgc_cmodule_object * cm = cast_fbgc_object_as_cmodule(o);
			const struct fbgc_cfunction * cc = cm->module->functions[0];
			//cprintf(111,"Functions:\n");
			for (int i = 1; cc!=NULL; ++i){
				//optimize strlen part
				if(!my_strcmp(str,cc->name) ){
					#ifdef PARSER_DEBUG
					cprintf(010,"\n**Function [%s] matched with str [%s]\n",cc->name,str);
					#endif
					return new_fbgc_cfun_object(cc->function);
				} 
				//cprintf(101,"{%s}\n",cc->name);
				cc = cm->module->functions[i];
			}

*/

	/*if(strcmp(x,fbgc_math_initializer_struct.name) == 0){
		cm = &fbgc_math_module;
	}
	else {
		cprintf(100,"Module could not be found!\n");
	}

	if(cm != NULL){
		struct fbgc_cmodule_object * cmo = (struct fbgc_cmodule_object *) fbgc_malloc(sizeof(struct fbgc_cmodule_object ));
		cmo->module = cm;
		cmo->base.type = CMODULE;//CHANGE THIS

		//load_module_in_field_object(main_field,cm);
		return cmo;
	}


}*/

/*

void module_load_all(struct fbgc_object * field_obj,struct fbgc_object * module_obj){
	struct fbgc_field_object * ll = cast_fbgc_object_as_field(field_obj);
	struct fbgc_cmodule_object * cm = module_obj;

	for (uint8_t i = 0; cc!=NULL; ++i){
		const struct fbgc_cfunction * cc = cm->module->functions[i];
		new_fbgc_cfun_object(cc->function);
	}
	cm = (struct fbgc_cmodule_object * )cm->base.next;


	new_fbgc_id_opcode(size_fbgc_tuple_object(fbgc_symbols)-1);
		
}*/

int main(int argc, char **argv){

#ifdef INTERPRETER_DEBUG    
cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 
#endif

//******************************************************************
	initialize_fbgc_memory_block();
	initialize_fbgc_symbol_table();
	 main_field = new_fbgc_field_object();
	//load_module_in_field_object(main_field,&fbgc_math_module);
	load_module_in_field_object(main_field,&fbgc_io_module);
	load_module_in_field_object(main_field,&fbgc_stl_module);
	//load_module_in_field_object(main_field,&fbgc_file_module);

	if(argc > 1)
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
	}

	free_fbgc_memory_block();
//******************************************************************

#ifdef INTERPRETER_DEBUG 
	cprintf(110,"\n[=======================================================================]\n\n\n\n\n\n"); 
#endif	
	return 0;
}



