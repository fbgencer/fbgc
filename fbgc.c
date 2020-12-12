// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.

#include "fbgc.h"
#include "cmodules.h"
#include "fbgc_config.h"


static void compile_file(struct fbgc_object * main_field,const char *file_name){

	clock_t begin,end;
	double parser_time,interpreter_time;

	assert(main_field);

	begin = clock();
   	int par = parse_file(&main_field,file_name);
	end = clock();
	parser_time = (double)(end - begin) / CLOCKS_PER_SEC; 

	 
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



void fbgc_handle_program_args(int argc, char const *argv[]){
	if(argc > 1){   
		if(!strcmp(argv[1],"-s")){
			//compile_one_line(main_field,argv[2]);
		}
		else{
			compile_file(current_field, argv[1]); 
		}		
	}
	else{
		//compile_file(current_field, "ex.fbgc");
	   //realtime_fbgc(main_field);
	}
}


int main(int argc, char const *argv[]){

	initialize_fbgc_memory_block(); //Initialize the memory and other things for the program

	//fbgc_gc_init(__builtin_frame_address(0), 3,&fbgc_symbols,&current_field,&__fbgc_runtime_program_stack); //Initialize garbage collector

	initialize_fbgc_symbol_table();
	current_field = new_fbgc_field(2,&_fbgc_io_property_holder,&_fbgc_stl_property_holder);
	
	//FBGC_LOGW("Initialization is finished\n");
	
	fbgc_handle_program_args(argc,argv);
	//fbgc_gc_run(1000000);
	//print_fbgc_memory_block();
	//print_fbgc_memory_object_pool();
	//print_fbgc_memory_raw_memory();
	free_fbgc_memory_block(); //Free the all memory that we allocated and close the program

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
