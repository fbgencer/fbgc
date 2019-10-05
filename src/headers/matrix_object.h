#ifndef FBGC_MATRIX_OBJECTH
#define FBGC_MATRIX_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

 
#define UNINITIALIZED_MATRIX 10
#define ZEROS_MATRIX 0
#define ONES_MATRIX 1

struct fbgc_matrix_object{
    struct fbgc_object base;
   	size_t row;
   	size_t column;
};

#define cast_fbgc_object_as_matrix(x)(((struct fbgc_matrix_object*) x))
#define sizeof_fbgc_matrix_object(x)(sizeof(struct fbgc_matrix_object) + sizeof(double) * (cast_fbgc_object_as_matrix(x)->row*cast_fbgc_object_as_matrix(x)->column) )

struct fbgc_object * new_fbgc_matrix_object(size_t row,size_t col,char tp);
struct fbgc_object * new_fbgc_matrix_object_from_range(struct fbgc_object * robj);
struct fbgc_object * matrix_creation_from_stack(struct fbgc_object ** , int, int, int );

struct fbgc_object * subscript_fbgc_matrix_object(struct fbgc_object * mat, size_t r, size_t c);

#define content_fbgc_matrix_object(x) ((double *) ( (char*)(&cast_fbgc_object_as_matrix(x)->column) + sizeof(size_t) ))

struct fbgc_object * multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2);
struct fbgc_object * add_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2);

struct fbgc_object * binary_op_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

void print_fbgc_matrix_object(struct fbgc_object *);
void free_fbgc_matrix_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif