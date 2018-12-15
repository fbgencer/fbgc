#ifndef FBGC_OBJECTH
#define FBGC_OBJECTH


struct fbgc_object{
    fbgc_token type;
    struct fbgc_object * next;
};

void print_fbgc_object(struct fbgc_object *);
void print_fbgc_object_ll(struct fbgc_object *);
void free_fbgc_object(struct fbgc_object *);



#define new_fbgc_object(x)((struct fbgc_object *) malloc(sizeof(fbgc_object)*x))


//Just connect two objects,check first in order to make sure without losing reference of the objects between x and y
// [X]->.., [y]->.. : [x]->[y]->.. 
#define connect_fbgc_objects(x,y) ({x->next = y;})

//call this if there are objects between x and y, it will deletes everything and connects x and y
//#define hard_connect_fbgc_objects(x,y)({struct fbgc_object * dummy; while() })

void print_fbgc_object_ll(struct fbgc_object * head){
	struct fbgc_object * iter = head;
	while(iter!= NULL){
		print_fbgc_object(iter);
		iter = iter->next;
	}
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

#endif