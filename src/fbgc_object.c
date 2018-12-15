#include "fbgc.h"

void print_fbgc_object_ll(struct fbgc_object * head){
	struct fbgc_object * iter = head;
	while(iter!= NULL){
		print_fbgc_object(iter);
		iter = iter->next;
	}
} 
void free_fbgc_object_ll(struct fbgc_object * head){
	struct fbgc_object * iter = head;
	while(head != NULL){
		head = iter->next;
		free_fbgc_object(iter);	
		iter = head;	
	}

		//iter->[]->[]->[]->
	
}

void print_fbgc_object(struct fbgc_object * self){
	if(self->type == DOUBLE){
		print_fbgc_double_object(self);
	}else{
		printf("undefined type in %s\n",__FUNCTION__ );
	}
}

void free_fbgc_object(struct fbgc_object * self){
	free(self);
}
