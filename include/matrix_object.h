// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#ifndef FBGC_MATRIX_OBJECTH
#define FBGC_MATRIX_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

 
#define UNINITIALIZED_MATRIX 10
#define ZEROS_MATRIX 0
#define ONES_MATRIX 1
#define EYE_MATRIX 2


struct fbgc_matrix_object{
    struct fbgc_object base;
    fbgc_token sub_type;
   	size_t row;
   	size_t column;
};
extern const struct fbgc_object_property_holder fbgc_matrix_object_property_holder;

#define cast_fbgc_object_as_matrix(x)(((struct fbgc_matrix_object*) x))


#define content_fbgc_matrix_object(x) ((double *) ( (char*)(&cast_fbgc_object_as_matrix(x)->column) + sizeof(size_t) ))
#define complex_content_fbgc_matrix_object(x) ( content_fbgc_matrix_object(x) + 1)

#define row_fbgc_matrix_object(x)(cast_fbgc_object_as_matrix(x)->row)
#define column_fbgc_matrix_object(x)(cast_fbgc_object_as_matrix(x)->column)

struct fbgc_object * new_fbgc_matrix_object(fbgc_token sub_type, size_t row,size_t col,char tp);
struct fbgc_object * new_fbgc_matrix_object_from_range(struct fbgc_object * robj);
struct fbgc_object * matrix_creation_from_stack(struct fbgc_object ** , int, int, int , fbgc_token);

struct fbgc_object * get_object_in_fbgc_matrix_object(struct fbgc_object * mat, size_t r, size_t c);
struct fbgc_object * set_object_in_fbgc_matrix_object(struct fbgc_object * mat, size_t r, size_t c,struct fbgc_object * obj);


struct fbgc_object * transpose_fbgc_matrix_object(struct fbgc_object * mat1);

struct fbgc_object * multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2);
struct fbgc_object * fast_multiply_fbgc_matrix_object(struct fbgc_object * mat1, struct fbgc_object * mat2);

struct fbgc_object * operator_fbgc_matrix_object(struct fbgc_object * a,struct fbgc_object * b,fbgc_token op);

uint8_t print_fbgc_matrix_object(struct fbgc_object *);
void free_fbgc_matrix_object(struct fbgc_object * );


#ifdef  __cplusplus
}
#endif

#endif