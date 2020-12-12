#include "fbgc.h"
#include "stdio.h"

int main(int argc, char **argv){	

	printf("Hello U am test!\n");
	printf("sqrt(4) = %g\n",sqrt(4));

	initialize_fbgc_memory_block(); //Initialize the memory and other things for the program
	
	struct fbgc_int_object * io = new_fbgc_int_object(100);

	print_fbgc_int_object(io);

	printf("\n");

	free_fbgc_memory_block(); //Free the all memory that we allocated and close the program

	return 0;
}
