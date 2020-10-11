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
#ifndef FBGC_QUEUEH
#define FBGC_QUEUEH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_queue{
	uint8_t * content;
	void * front;
	size_t size;
	size_t block_size;
};



struct fbgc_queue * new_fbgc_queue(size_t cap, size_t block_size);
struct fbgc_queue * from_array_new_fbgc_queue(const void * array_ptr, size_t array_len, size_t block_size);
struct fbgc_queue * copy_queue_fbgc_queue(struct fbgc_queue * self);


void init_static_fbgc_queue_with_allocater(struct fbgc_queue * queue, size_t cap, size_t block_size, void * (*allocator)(size_t ));
void init_static_fbgc_queue(struct fbgc_queue * queue,size_t cap, size_t block_size);
size_t size_fbgc_queue(const struct fbgc_queue * queue);
size_t capacity_fbgc_queue(const struct fbgc_queue * queue);
bool is_full_fbgc_queue(const struct fbgc_queue * queue);
bool is_empty_fbgc_queue(const struct fbgc_queue * queue);
void * front_fbgc_queue(const struct fbgc_queue * queue);
void * back_fbgc_queue(const struct fbgc_queue * queue);
void push_fbgc_queue(struct fbgc_queue * queue, const void * item);
void push_back_fbgc_queue(struct fbgc_queue * queue, const void * item);
void pop_fbgc_queue(struct fbgc_queue * queue);
void pop_front_fbgc_queue(struct fbgc_queue * queue);
void swap_with_another_queue_fbgc_queue(struct fbgc_queue * self,struct fbgc_queue * queue2 );

bool iterator_fbgc_queue_front_to_back(struct fbgc_queue * queue, void ** it);

#ifdef  __cplusplus
}
#endif

#endif