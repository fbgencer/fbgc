#ifndef FBGC_MATRIX_OBJECTH
#define FBGC_MATRIX_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_matrix_object{
    struct fbgc_object base;
    double * contents;
    unsigned int row,column;
};

#define cast_fbgc_object_as_matrix(x)(((struct fbgc_matrix_object*) x))

struct fbgc_object * new_fbgc_matrix_object(unsigned int);
void append_row_to_fbgc_matrix_object(struct fbgc_object * mat,struct fbgc_object * data);

/*
struct
fbgc_object * add_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b);
struct
fbgc_object * subtract_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b);
struct
fbgc_object * multiply_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b);
struct
fbgc_object * divide_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b);
*/

void print_fbgc_matrix_object(struct fbgc_object *);
void free_fbgc_matrix_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif