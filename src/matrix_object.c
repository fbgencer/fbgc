#include "fbgc.h"


struct fbgc_object * new_fbgc_matrix_object(unsigned int column_size){
	struct fbgc_matrix_object *matrixo =  (struct fbgc_matrix_object*) malloc(sizeof(struct fbgc_matrix_object));
    matrixo->base.type = MATRIX;
    matrixo->base.next = NULL;
    matrixo->row = 0;
    matrixo->column = column_size;
    matrixo->contents = (double *) malloc(sizeof(column_size)); 
    return (struct fbgc_object*) matrixo;
}

void append_row_to_fbgc_matrix_object(struct fbgc_object * mat,struct fbgc_object * data){
    if(cast_fbgc_object_as_matrix(mat)->row == 0){
        for(unsigned int i = cast_fbgc_object_as_matrix(mat)->column; i>=0; i--){
            data = data->next;
            *(cast_fbgc_object_as_matrix(mat)->contents+i) = convert_fbgc_object_to_double(data);
        }
        cast_fbgc_object_as_matrix(mat)->row = 1;
    }
}


void print_fbgc_matrix_object(struct fbgc_object * obj){
    struct fbgc_matrix_object * matrixo = (struct fbgc_matrix_object*) obj;
    for(unsigned int i = 0; i<matrixo->row; i++){
        cprintf(011,"[");
        for(unsigned int j = 0; j<matrixo->column; j++){
            cprintf(011,"%f,",matrixo->contents[i * matrixo->column + j]);
        }
        cprintf(011,"\b]"); 
    } 
}

void free_fbgc_matrix_object(struct fbgc_object * obj){
}
