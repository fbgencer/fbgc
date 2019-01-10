#include "fbgc.h"


struct
fbgc_object * new_fbgc_object(fbgc_token token){
	struct fbgc_object *o =  (struct fbgc_object*) malloc(sizeof(struct fbgc_object));
    o->type = token;
    o->next = NULL;  
    return (struct fbgc_object*) o;
}

void push_fbgc_object_ll(struct fbgc_object ** head,struct fbgc_object ** tail,struct fbgc_object * obj){
    //cast head as integer so we can change its content as our list size
    struct fbgc_int_object * head_int = (struct fbgc_int_object*) *head;
   	//connect the list
   	head_int->content++;
   	if(head_int->base.next == NULL){
   		head_int->base.next = obj; //[H] -> [obj]
   		(*tail)->next = obj; // [obj]<-[T]
   		obj->next = *tail;  //[H] -> [obj] <-> [T] 		
   	}
   	else{
   		//[H] -> [obj] <-> [T] 
   		(*tail)->next->next = obj; //[H] -> [obj]->[obj_new] 
   		obj->next = *tail;//[H] -> [obj]->[obj_new]->[T] 
   		(*tail)->next = obj;//[H] -> [obj]->[obj_new]<-> [T] 
   	}
   	//head_int->base.next = obj;
   	//	obj->next = head_int->base.next;
	//head_int->base.next = obj;
}

void print_fbgc_object_ll(struct fbgc_object * head){
	printf("\033[1;37m[-------------Linked List printer----------------]\033[0m\n");
	struct fbgc_object * iter = head->next;
	struct fbgc_int_object * head_int = (struct fbgc_int_object*) head;
	printf("\033[1;31m[Head:%d]\033[0m\n",head_int->content);
	
	for(int i = 0; i<head_int->content; i++){
		print_fbgc_object(iter);
		iter = iter->next;
	}
	printf("\033[1;37m[-----------------------------]\033[0m\n");
} 
void free_fbgc_object_ll(struct fbgc_object * head){
		struct fbgc_int_object * head_int = (struct fbgc_int_object*) head;
	struct fbgc_object * iter = head_int->base.next;
	struct fbgc_object * temp = iter;
	for(int i = 0; i<head_int->content; i++){
		temp = iter->next;
		free_fbgc_object(iter);	
		iter = temp;	
	}
	
	head_int->content = 0;
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
