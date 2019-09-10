#ifndef FBGC_MATRIX_OBJECTH
#define FBGC_MATRIX_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_matrix_object{
    struct fbgc_object base;
   	size_t row;
   	size_t column;
    //double * contents;
};

#define cast_fbgc_object_as_matrix(x)(((struct fbgc_matrix_object*) x))

struct fbgc_object * new_fbgc_matrix_object(unsigned int);
struct fbgc_object * matrix_creation_from_stack(struct fbgc_object ** , int, int, int );

#define matrix_object_content(x) (double *) ( (char*)(&cast_fbgc_object_as_matrix(m)->column) +sizeof(size_t) )

//#define matrix_object_content(x)( (struct fbgc_object **)((char*) &cast_fbgc_object_as_tuple(x)->size+sizeof(cast_fbgc_object_as_tuple(x)->size)));

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