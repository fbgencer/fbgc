#include "fbgc.h"


int main(){
	

	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	struct fbgc_object * head = new_fbgc_ll_object();
	char line[100];
	FILE *input_file = fopen("ex.fbgc","r");
    
    while (fgets(line, sizeof(line), input_file)) {
		regex_lexer(&head,line);
    }
    fclose(input_file);
    print_fbgc_ll_object(head,"Main");
	head = parser(head); print_fbgc_ll_object(head,"Main");
	free_fbgc_ll_object(head);

	cprintf(110,"[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}

