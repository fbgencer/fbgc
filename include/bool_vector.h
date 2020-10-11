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
#ifndef FBGC_BOOLVECTORH
#define FBGC_BOOLVECTORH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_bool_vector{
	uint8_t * content;
	size_t size;
};



struct fbgc_bool_vector * new_fbgc_bool_vector(size_t cap);

void init_static_fbgc_bool_vector(struct fbgc_bool_vector * bv,size_t cap);
size_t size_fbgc_bool_vector(const struct fbgc_bool_vector * bv);
size_t capacity_fbgc_bool_vector(const struct fbgc_bool_vector * bv);
bool is_full_fbgc_bool_vector(const struct fbgc_bool_vector * bv);
bool is_empty_fbgc_bool_vector(const struct fbgc_bool_vector * bv);
bool front_fbgc_bool_vector(const struct fbgc_bool_vector * bv);
bool back_fbgc_bool_vector(const struct fbgc_bool_vector * bv);
void push_back_fbgc_bool_vector(struct fbgc_bool_vector * bv, bool value);
void pop_back_fbgc_bool_vector(struct fbgc_bool_vector * bv);
void shrink_fbgc_bool_vector(struct fbgc_bool_vector * bv, size_t cap);

bool at_fbgc_bool_vector(const struct fbgc_bool_vector * bv, size_t index);
bool get_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index);
void set_bit_value_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index, bool value);
void set_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index);
void clear_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index);
void toggle_bit_fbgc_bool_vector(const struct fbgc_bool_vector * bv, int index);
void set_back_fbgc_bool_vector(const struct fbgc_bool_vector * bv, bool value);
//void insert_fbgc_bool_vector(struct fbgc_bool_vector * bv, const void * item, size_t index_start, size_t item_len);
//void swap_with_another_bool_vector_fbgc_bool_vector(struct fbgc_bool_vector * self,struct fbgc_bool_vector * bool_vector2 );


#ifdef  __cplusplus
}
#endif

#endif