#include "../src/fbgc.h"


#ifdef MODULE_TEST

int main(int argc, char **argv){	
	initialize_fbgc_memory_block();

	array_test();

	


	free_fbgc_memory_block();    
	return 0;
}

#endif