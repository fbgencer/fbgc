#include "double_object.h"




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

