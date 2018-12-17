#include "fbgc.h"


int main(){
		
	double dbar[] = {3.14,2.727585};
	struct fbgc_object * head, * iter;
	head = new_fbgc_double_object(10.0);
	iter = head;
	for(int i = 0; i<sizeof(dbar)/sizeof(double); i++){
		struct fbgc_object *db = new_fbgc_double_object(dbar[i]); 
		iter->next = db; 
		iter = db;
	}
	
	print_fbgc_object_ll(head);
	free_fbgc_object_ll(head);

	return 0;
}