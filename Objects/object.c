#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define OBJECT 0
#define DOUBLE 1
#define INT 2
#define STRING 3
#define OPERATOR 4
#define PLUS 5
#define MINUS 6
#define MULT 7
#define DIVIDE 8
#define ASSIGN 9
#define VAR 10


struct fbgc_object{
    uint8_t type;
    struct fbgc_object * next;
};

struct fbgc_double_object{
    struct fbgc_object base;
    double *data;
    uint8_t size;  
};

struct fbgc_op_object{
    struct fbgc_object base;
    uint8_t *data;
    uint8_t size;  
};


struct fbgc_var_object{
    struct fbgc_object base;
	struct fbgc_object *data; 
};

void free_fbgc_object(struct fbgc_object *);


void push_fbgc_object(struct fbgc_object *main_obj,struct fbgc_object *inc_next ){
    struct fbgc_object * obj = main_obj;

    while(obj->next != NULL){
        obj = obj->next;
    }
    obj->next = (struct fbgc_object *)  inc_next;
    obj->next->next = NULL;
}


void pop_fbgc_object(struct fbgc_object *head){
    struct fbgc_object * obj = head;

	while(obj->next->next != NULL){
        obj = obj->next;
    }

   free_fbgc_object(obj->next);
   obj->next = NULL;
}


int fbgc_object_list_size(struct fbgc_object *self){
    struct fbgc_object * obj = self;

    int i  = 0;
    while(obj->next != NULL){
        obj = obj->next;
        i++;
    }

    return i+1;
}

struct
fbgc_object * add_fbgc_double_object(struct fbgc_object *obj, double inc_data){
    struct fbgc_double_object *dbo = (struct fbgc_double_object*) obj;
    if(obj == NULL){
    	dbo =  (struct fbgc_double_object*) malloc(sizeof(struct fbgc_double_object));
    	dbo->base.type = DOUBLE;
    	dbo->base.next = NULL;
    	dbo->data = NULL;	
    	dbo->size = 0;
    }

    dbo->data = (double*) realloc(dbo->data,sizeof(double) * ++dbo->size);
    *(dbo->data+dbo->size-1) = inc_data;
    return (struct fbgc_object*) dbo;
}
void print_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object * dbo = (struct fbgc_double_object*) obj;

	for(int i = 0; i< dbo->size; i++){
		printf("Double Value : %f\n",*(dbo->data+i));	
	}   
    
}
void free_fbgc_double_object(struct fbgc_object * obj){
    struct fbgc_double_object *dbo = (struct fbgc_double_object*) obj;
    free(dbo->data);
    free(dbo);
}




struct
fbgc_object * add_fbgc_op_object(struct fbgc_object *obj, uint8_t inc_data){
    struct fbgc_op_object *opo = (struct fbgc_op_object*) obj;
    if(obj == NULL){
    	opo =  (struct fbgc_op_object*) malloc(sizeof(struct fbgc_op_object));
    	opo->base.type = OPERATOR;
    	opo->base.next = NULL;
    	opo->data = NULL;	
    	opo->size = 0;
    }

    opo->data = (uint8_t *) realloc(opo->data,sizeof(uint8_t) * ++opo->size);
    *(opo->data+opo->size-1) = inc_data;
    return (struct fbgc_object*) opo;
}
void print_fbgc_op_object(struct fbgc_object * obj){
    struct fbgc_op_object * opo = (struct fbgc_op_object*) obj;

	for(int i = 0; i< opo->size; i++){
		printf("Operator Value : ");
		if(*(opo->data+i) == PLUS) printf("+\n");
		else if(*(opo->data+i) == MINUS) printf("-\n");	
		else if(*(opo->data+i) == MULT) printf("*\n");	
		else if(*(opo->data+i) == DIVIDE) printf("/\n");
		else if(*(opo->data+i) == ASSIGN) printf("=\n");	
	}   
    
}
void free_fbgc_op_object(struct fbgc_object * obj){
    struct fbgc_op_object *opo = (struct fbgc_op_object*) obj;
    free(opo->data);
    free(opo);
}

void print_fbgc_object(struct fbgc_object * obj){
    struct fbgc_object *iter = obj;
    
    while(iter != NULL){
		if(iter->type == DOUBLE) print_fbgc_double_object(iter);
   		else if (iter->type == OPERATOR) print_fbgc_op_object(iter);
   		else if(iter->type == VAR) {
   			struct fbgc_var_object *varo = (struct fbgc_var_object*) iter;
   			printf("<:Variable:>\n");
   			print_fbgc_object(varo->data);
   		}
        iter = iter->next;
    	printf("=============================\n");
    }
    printf("-------------------------------------------------------\n");
}




struct
fbgc_object * new_fbgc_var_object(struct fbgc_object *self){
	struct fbgc_var_object *varo = (struct fbgc_var_object*) self;
	
	varo = (struct fbgc_var_object*) malloc(sizeof(struct fbgc_var_object));
	varo->base.type = VAR;
	varo->base.next = NULL;	
	varo->data = NULL;	
	
	return (struct fbgc_object *) varo;
}

struct
fbgc_object * assign_fbgc_var_object(struct fbgc_object *self,struct fbgc_object *obj2){
	struct fbgc_var_object *varo = (struct fbgc_var_object*) self;
	if(self == NULL){
		varo = (struct fbgc_var_object*) malloc(sizeof(struct fbgc_var_object));
		varo->base.type = VAR;
		varo->base.next = NULL;		
	}

	if(varo->data == NULL) varo->data = obj2;
	else {
		free_fbgc_object(varo->data);
		varo->data = obj2;
	}

	return (struct fbgc_object *) varo;
}


void free_fbgc_var_object(struct fbgc_object * obj){
    struct fbgc_var_object *varo = (struct fbgc_var_object*) obj;
    if(varo->data != NULL)free_fbgc_double_object(varo->data);
    free(varo);
}



void free_fbgc_object(struct fbgc_object * self){
	if(self->type == DOUBLE) free_fbgc_double_object(self);
	else if(self->type == OPERATOR) free_fbgc_op_object(self);
	else if(self->type == VAR) free_fbgc_var_object(self);
}


struct fbgc_object * stack[20];
int stack_iter = 0;
#define push_stack(x)(stack[stack_iter++] = x)
#define pop_stack(x)(stack[--stack_iter])
#define tos (stack[stack_iter-1])

void print_stack(){
	
	for(int i = 0; i<stack_iter; i++){
		if(stack[i]->type == DOUBLE) printf("%d DOUBLE\n",i);
		else if(stack[i]->type == VAR) printf("%d VAR\n",i);
		else if(stack[i]->type == OPERATOR) printf("%d OPERATOR\n",i);

	}
}

void reader(struct fbgc_object *head){
	printf("\n....Reader.....\n");
	print_fbgc_object(head);

	printf("Size : %d\n",fbgc_object_list_size(head));
	push_stack(head);
	struct fbgc_object * iter = head;
	
	printf("=============STACK =========\n");
	print_stack();

}	


int main(){

	printf("--------fBGc examples--------\n");
/*
	basit bir cozumu parse edip elle cozdurelim
	x = 5*(4-7/(2+5)); x:15
	x = x * 2; x:30

	x,5,4,7,2,5,+,/,-,*,=,x,x,2,*,=
	Array olarak aldığımz için
	VAR,DB,OPS,VAR,VAR,DB,OPS
	x	5	=   x 	x   2  =
		4	*		       *
		7	-	
		2	/
		5	+


	operator okurken en alttakine gidiyoruz
	operator stackteki son elemanın size'ı 2ye büyük eşitse son 2 rakamı alıyor ve num1 op num2 yapıyor
*/
	struct fbgc_object * head = NULL;
	struct fbgc_object  * var0 = NULL, *dbo0 = NULL, *ops0 = NULL, *var1 = NULL,*dbo1 = NULL,*opo2 = NULL;
	
	//adding var x
	var0 = new_fbgc_var_object(var0);
	head = var0;
	//adding 5,4,7,2,5
	double nums[] = {5,4,7,2,5};
	for(int i = 0; i<sizeof(nums)/sizeof(double); i++) dbo0 = add_fbgc_double_object(dbo0,nums[i]);
	//operators
	uint8_t op_array[] = {ASSIGN,MULT,MINUS,DIVIDE,PLUS};
	for(int i = 0; i<sizeof(op_array)/sizeof(uint8_t); i++) ops0 = add_fbgc_op_object(ops0,op_array[i]);
	
	push_fbgc_object(head,dbo0);
	push_fbgc_object(head,ops0);



	reader(head);

	
	
	free_fbgc_object(ops0);
	free_fbgc_object(dbo0);
	free_fbgc_object(var0);

	/*struct fbgc_object * dbo1 = NULL, * dbo2 = NULL;

	double nums[] = {3.5,4.5,5.5,6.5};
	for(int i = 0; i<sizeof(nums)/sizeof(double); i++){
		dbo1 = push_fbgc_double_object(dbo1,nums[i]);
	}
	
	struct fbgc_object * opo1 = NULL;
	uint8_t ops[] = {PLUS};
	for(int i = 0; i<sizeof(ops)/sizeof(uint8_t); i++){
		opo1 = push_fbgc_op_object(opo1,ops[i]);
	}

	dbo2 = push_fbgc_double_object(dbo2,1.25);
	dbo2 = push_fbgc_double_object(dbo2,5);

	struct fbgc_object * var1 = NULL;
	var1 = assign_fbgc_var_object(var1,dbo2);


	push_fbgc_object(dbo1,opo1);
	push_fbgc_object(dbo1,var1);
	print_fbgc_object(dbo1);
	pop_fbgc_object(dbo1);
	pop_fbgc_object(dbo1);
	print_fbgc_object(dbo1);

	free_fbgc_op_object(opo1);
	//free_fbgc_double_object(dbo2);
	free_fbgc_var_object(var1);
	free_fbgc_double_object(dbo1);*/


	printf("--------finished--------\n");
	return 0;
}
