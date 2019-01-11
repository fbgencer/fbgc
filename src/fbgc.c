#include "fbgc.h"


int main(){
	
	printf("\033[1;33m[%s]\033[0m\n","======================================================================="); 
	struct fbgc_object * head, * tail, * iter;

	head = new_fbgc_int_object(0);
	tail =  new_fbgc_int_object(100);
	iter = head; 

	//8 * ( 5 + 10 / ( 2 - 3 ) ) 
	//Expected :8,5,10,2,3,-,/,+,*
	/*
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(5));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(10));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(DIV));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(2));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(3));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));		
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));	
	*/

	//((2+3))
	/*
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(2));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(3));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));		
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));	
	*/


	//3.14/8*78+112
	
	/*push_fbgc_object_ll(&head,&tail,new_fbgc_double_object(3.14));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(DIV));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(78));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(112));
	*/



	//3.14/((8*78+112)-5)
	
	push_fbgc_object_ll(&head,&tail,new_fbgc_double_object(3.14));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(DIV));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(LPARA));	
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(8));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MULT));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(78));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(PLUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(112));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(MINUS));
	push_fbgc_object_ll(&head,&tail,new_fbgc_int_object(5));			
	push_fbgc_object_ll(&head,&tail,new_fbgc_object(RPARA));		
		


	print_fbgc_object_ll(head);

	head = parser(head);

	print_fbgc_object_ll(head);

	free_fbgc_object_ll(head);

	free_fbgc_int_object(head);
	free_fbgc_int_object(tail);


	/*double dbar[] = {3.14,2.727585};
	struct fbgc_object * head, * iter;
	head = new_fbgc_double_object(10.0);
	iter = head;
	for(int i = 0; i<sizeof(dbar)/sizeof(double); i++){
		struct fbgc_object *db = new_fbgc_double_object(dbar[i]); 
		iter->next = db; 
		iter = iter->next;
	}

	fbgc_token tokenlist[] = {PLUS,MINUS,MULT,DIV};

	for(int i = 0; i<sizeof(tokenlist)/sizeof(fbgc_token); i++){
		struct fbgc_object *o = new_fbgc_object(tokenlist[i]);
		iter->next = o;
		iter = iter->next;
	}
	
	print_fbgc_object_ll(head);
	free_fbgc_object_ll(head);*/

	printf("\033[1;33m[%s]\033[0m\n","=======================================================================");
	return 0;
}