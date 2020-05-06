#include "../src/fbgc.h"


#ifdef MODULE_TEST

void array_test();
void map_test();
void memory_test();

int main(int argc, char **argv){	
	initialize_fbgc_memory_block();

	//array_test();

	//map_test();

	memory_test();
	


	free_fbgc_memory_block();    
	return 0;
}

#endif