#include "fbgc.h"


int main(){
	

	cprintf(110,"\n\n\n\n\n[=======================================================================]\n"); 

	struct fbgc_object * head = new_fbgc_ll_object(LINKED_LIST);
	char line[100];
	FILE *input_file = fopen("ex.fbgc","r");
    
    while (fgets(line, sizeof(line), input_file)){
    	if(line[0] == ':' && line[1] == '>') continue; 
		if(!regex_lexer(&head,line)) return 0;
    }
    fclose(input_file);
    print_fbgc_ll_object(head,"Main");
	head = parser(head); print_fbgc_ll_object(head,"Main");
	free_fbgc_ll_object(head);

	cprintf(110,"[=======================================================================]\n\n\n\n\n\n"); 
	return 0;
}

