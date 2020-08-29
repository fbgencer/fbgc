#ifndef FBGC_QUEUEH
#define FBGC_QUEUEH

#ifdef __cplusplus
extern "C" {
#endif

struct fbgc_queue{
	void * data;
	void * front;
	size_t size;
	size_t capacity;
	size_t block_size;
};



struct fbgc_queue * new_fbgc_queue(size_t cap, size_t block_size);
void init_static_fbgc_queue(struct fbgc_queue * queue,size_t cap, size_t block_size);
size_t size_fbgc_queue(const struct fbgc_queue * queue);
bool is_fbgc_queue_full(const struct fbgc_queue * queue);
bool is_fbgc_queue_empty(const struct fbgc_queue * queue);
void * fbgc_queue_front(const struct fbgc_queue * queue);
void fbgc_queue_push(struct fbgc_queue * queue, const void * item);
void fbgc_queue_push_back(struct fbgc_queue * queue, const void * item);
void fbgc_queue_pop(struct fbgc_queue * queue);
void fbgc_queue_pop_front(struct fbgc_queue * queue);
void fbgc_queue_swap_with_another_queue(struct fbgc_queue * self,struct fbgc_queue * queue2 );


#ifdef  __cplusplus
}
#endif

#endif