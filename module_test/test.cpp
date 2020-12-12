#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "fbgc.h"


int main(){
    printf("Hello world! This is a test!\n");
	initialize_fbgc_memory_block(); //Initialize the memory and other things for the program
	
	struct fbgc_int_object * io = (struct fbgc_int_object *) new_fbgc_int_object(100);

	print_fbgc_int_object((struct fbgc_object *)io);


	free_fbgc_memory_block(); //Free the all memory that we allocated and close the program

    return 0;
}