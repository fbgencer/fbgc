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
#ifndef FBGC_VECTORH
#define FBGC_VECTORH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_vector{
	uint8_t * content;
	size_t size;
	size_t block_size;
};



struct fbgc_vector * new_fbgc_vector(size_t cap, size_t block_size);
struct fbgc_vector * from_array_new_fbgc_vector(const void * array_ptr, size_t array_len, size_t block_size);
struct fbgc_vector * copy_vector_fbgc_vector(struct fbgc_vector * self);


void init_static_fbgc_vector(struct fbgc_vector * vector,size_t cap, size_t block_size);
void init_static_fbgc_vector_with_allocater(struct fbgc_vector * vector, size_t cap, size_t block_size, void * (*allocator)(size_t ));
size_t size_fbgc_vector(const struct fbgc_vector * vector);
size_t capacity_fbgc_vector(const struct fbgc_vector * vector);
bool is_full_fbgc_vector(const struct fbgc_vector * vector);
bool is_empty_fbgc_vector(const struct fbgc_vector * vector);
void * front_fbgc_vector(const struct fbgc_vector * vector);
void * back_fbgc_vector(const struct fbgc_vector * vector);
void push_back_fbgc_vector(struct fbgc_vector * vector, const void * item);
void pop_back_fbgc_vector(struct fbgc_vector * vector);
void erase_fbgc_vector(struct fbgc_vector * vector);
void reserve_fbgc_vector(struct fbgc_vector * vector, size_t new_cap);
void shrink_fbgc_vector(struct fbgc_vector * vector, size_t new_cap);
void shrink_to_fit_fbgc_vector(struct fbgc_vector * vector);
void hard_shrink_to_fit_fbgc_vector(struct fbgc_vector * vector);

void * at_fbgc_vector(const struct fbgc_vector * vector, size_t index);
void * get_item_fbgc_vector(const struct fbgc_vector * vector, int index);
void set_item_fbgc_vector(const struct fbgc_vector * vector, int index, const void * item);
void insert_fbgc_vector(struct fbgc_vector * vector, size_t index_start, const void * item, size_t item_len);
void sort_fbgc_vector(struct fbgc_vector * vector, int (*compare_function)(const void *, const void *) );
//void swap_with_another_vector_fbgc_vector(struct fbgc_vector * self,struct fbgc_vector * vector2 );


#ifdef  __cplusplus
}
#endif

#endif