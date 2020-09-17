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
size_t size_fbgc_vector(const struct fbgc_vector * vector);
size_t capacity_fbgc_vector(const struct fbgc_vector * vector);
bool is_full_fbgc_vector(const struct fbgc_vector * vector);
bool is_empty_fbgc_vector(const struct fbgc_vector * vector);
void * front_fbgc_vector(const struct fbgc_vector * vector);
void * back_fbgc_vector(const struct fbgc_vector * vector);
void push_back_fbgc_vector(struct fbgc_vector * vector, const void * item);
void pop_back_fbgc_vector(struct fbgc_vector * vector);

void * at_fbgc_vector(const struct fbgc_vector * vector, size_t index);
void * get_item_fbgc_vector(const struct fbgc_vector * vector, int index);
void set_item_fbgc_vector(const struct fbgc_vector * vector, int index, const void * item);
void insert_fbgc_vector(struct fbgc_vector * vector, size_t index_start, const void * item, size_t item_len);
//void swap_with_another_vector_fbgc_vector(struct fbgc_vector * self,struct fbgc_vector * vector2 );


#ifdef  __cplusplus
}
#endif

#endif