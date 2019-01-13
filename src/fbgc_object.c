#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) malloc(sizeof(struct fbgc_object));
    o->type = token;
    o->next = NULL;  
   // cprintf(101,"Hazirlanan [%s]\n",object_name_array[token]);
    return (struct fbgc_object*) o;
}


void print_fbgc_object(struct fbgc_object * self){
	
	switch(self->type){
		case INT:
			print_fbgc_int_object(self);
		break;
		case DOUBLE:
			print_fbgc_double_object(self);
		break;
		default:
			printf("\033[1;31m[%s]\033[0m\n",object_name_array[self->type]);  
		break;

	}
}

void free_fbgc_object(struct fbgc_object * self){
	free(self);
}
