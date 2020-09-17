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