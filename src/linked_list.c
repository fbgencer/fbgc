#include "fbgc.h"


struct
fbgc_object * new_fbgc_ll_object(void){
	struct fbgc_ll_object *llo =  (struct fbgc_ll_object*) malloc(sizeof(struct fbgc_ll_object));
    llo->base.type = LINKED_LIST;
    llo->base.next = NULL;
    llo->size = 0;
   	llo->tail = (struct fbgc_object*) malloc(sizeof(struct fbgc_object));
   	llo->tail->next = (struct fbgc_object *) llo;
    return (struct fbgc_object*) llo;
}

void free_fbgc_ll_object(struct fbgc_object * obj){
    struct fbgc_ll_object *llo = cast_fbgc_object_as_ll(obj);
   	free(llo->tail);
    free(llo);
}

struct
fbgc_object * push_front_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj){
    //cast head as ll so we can change its content as our list size
    struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
   	//connect the list
   	head_ll->size++;

   	if(head_ll->base.next == NULL){ //[H] ->NULL, [H]<-[T] 
   		head_ll->base.next = obj; //[H] -> [obj]
   		head_ll->tail->next = obj; // [obj]<-[T]
   		obj->next = head_ll->tail;  //[H] -> [obj] <-> [T] 		
   	}
   	else{ //[H] -> [obj0] <-> [T] 

   		obj->next = head_ll->base.next;// [obj]->[obj0]<->[T] and [H]->[obj0]<->[T] head next and obj next shows previous top object
   		head_ll->base.next = obj; // Now [H]->[obj]->[obj0]<->[T]
   	}
   	return obj;
}


void push_fbgc_object_ll(struct fbgc_object ** head,struct fbgc_object ** tail,struct fbgc_object * obj){
    //cast head as integer so we can change its content as our list size
    struct fbgc_int_object * head_int = (struct fbgc_int_object*) *head;
   //cprintf(111,"Gelen [%s]\n",object_name_array[obj->type]);
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
	//cprintf(111,"[---------Linked List printer---------]\n");
	struct fbgc_object * iter = head->next;
	struct fbgc_int_object * head_int = (struct fbgc_int_object*) head;
	/*printf("\033[1;31m[Head:%d]\033[0m\n",head_int->content);
	
	for(int i = 0; i<head_int->content; i++){
		print_fbgc_object(iter);
		iter = iter->next;
	}*/

	cprintf(101,"[H]->");

	iter = head->next;

	for(int i = 0; i<head_int->content; i++){
		if(iter->type == INT) cprintf(011,"{%d}",cast_fbgc_object_as_int(iter)->content);   
		else if(iter->type == DOUBLE) cprintf(011,"{%f}",cast_fbgc_object_as_double(iter)->content); 
		else cprintf(011,"{%s}",object_name_array[iter->type]);

		if(i!= head_int->content-1) cprintf(110,"->");

		iter = iter->next;
	}	
	cprintf(101,"<->[T]\n");
	//cprintf(111,"[-----------------------------]\n");

	
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
