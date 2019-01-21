#include "fbgc.h"


struct
fbgc_object * new_fbgc_ll_object(void){
	struct fbgc_ll_object *llo =  (struct fbgc_ll_object*) malloc(sizeof(struct fbgc_ll_object));
    llo->base.type = LINKED_LIST;
    llo->size = 0;
   	llo->tail = new_fbgc_object(LINKED_LIST);
   	llo->tail->next = (struct fbgc_object *) llo;
   	llo->base.next = llo->tail;
    return (struct fbgc_object*) llo;
}

void free_fbgc_ll_object(struct fbgc_object * head){
  
  struct fbgc_ll_object *head_ll = cast_fbgc_object_as_ll(head);
  struct fbgc_object * iter = head_ll->base.next;
	struct fbgc_object * temp = iter;
	while(iter!=head_ll->tail){
		temp = iter->next;
		free_fbgc_object(iter);
		iter = temp;
	}    
    
   	free(head_ll->tail);
    free(head_ll);
  
}

struct
fbgc_object * push_front_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj){
     //cast head as ll so we can change its content as our list size
     struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
   	//connect the list
   	 
   	//[H] <-> [T] 
   	obj->next = head_ll->base.next;// [obj]->[T] and 
   	//[H]->[T] head next and obj next shows previous top object
   	head_ll->base.next = obj; // Now [H]->[obj]->[T]
   	
   	if(head_ll->size == 0) head_ll->tail->next = obj; // Now [H]->[obj]<->[T] connect the tail just for one time
   	head_ll->size++;
   	return (struct fbgc_object *) head_ll;
}


struct
fbgc_object * push_back_fbgc_ll_object(struct fbgc_object * head,struct fbgc_object * obj){
    //cast head as ll so we can change its content as our list size
    struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
   	 
   	//[H] -> [O1] <-> [T] 
    head_ll->tail->next->next = obj; //[O1]->[obj] 
    obj->next = head_ll->tail;//[O1]->[obj]->[T]
    head_ll->tail->next = obj;//[O1]->[obj]<->[T]

   	head_ll->size++;
   	return (struct fbgc_object *) head_ll;
}

struct
fbgc_object * pop_front_fbgc_ll_object(struct fbgc_object *head){
    //cast head as ll so we can change its content as our list size
     struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
 	head_ll->base.next =  head_ll->base.next->next;
     head_ll->size--;
   	return (struct fbgc_object *) head_ll; 	
}

struct
fbgc_object * delete_front_fbgc_ll_object(struct fbgc_object *head){
    //cast head as ll so we can change its content as our list size
     struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
     struct fbgc_object * temp = head_ll->base.next;
     head_ll->base.next =  head_ll->base.next->next;
     free_fbgc_object(temp);
     head_ll->size--;
     return (struct fbgc_object *) head_ll;  
}


struct
fbgc_object * pop_back_fbgc_ll_object(struct fbgc_object *head){
     //cast head as ll so we can change its content as our list size
     struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
 	struct fbgc_object * iter = head_ll->base.next;
 	while(iter->next != head_ll->tail->next) iter = iter->next;
 	iter->next = head_ll->tail;
 	head_ll->tail->next = iter;
     head_ll->size--;
     return (struct fbgc_object *) head_ll; 	
}



void print_fbgc_ll_object(struct fbgc_object * head,const char *s1){
	struct fbgc_ll_object * head_ll = cast_fbgc_object_as_ll(head);
	struct fbgc_object * iter = head_ll->base.next;


	cprintf(101,"[%s]->",s1);

	while(iter!=head_ll->tail){
		if(iter->type == INT) cprintf(011,"{%d:INT}",cast_fbgc_object_as_int(iter)->content);   
		else if(iter->type == DOUBLE) cprintf(011,"{%f:DB}",cast_fbgc_object_as_double(iter)->content); 
		else if(iter->type == STRING || iter->type == WORD) cprintf(011,"{%s:%s}",cast_fbgc_object_as_str(iter)->content,object_name_array[iter->type]);
	//	else if(iter->type == WORD) cprintf(011,"{WORD}");
     //else cprintf(011,"{%s}",object_name_array[iter->type]);
		else if(is_fbgc_OPERATOR(iter->type)) cprintf(011,"{%s:%s}",get_token_as_str(iter->type),object_name_array[iter->type]);
    else cprintf(011,"{%s}",object_name_array[iter->type]);		
		iter = iter->next;
	}
	cprintf(101,"<->[T]\n");

}

//===============================================================================
//OLD

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
