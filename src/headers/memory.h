#ifndef FBGC_MEMORY
#define FBGC_MEMORY

#ifdef __cplusplus
extern "C" {
#endif


#define INITIAL_INTERNAL_BUFFER_SIZE 1 //[byte]
#define INITIAL_OBJECT_POOL_SIZE 20 //[byte]

struct fbgc_memory_block{
	void * internal_buffer_head;	
	size_t internal_buffer_size;

	void * object_pool_head;
	size_t object_pool_size;
	void * object_pool_ptr;
};


struct fbgc_memory_block fbgc_memb;

void initialize_fbgc_memory_block();
void print_fbgc_memory_block();
void free_fbgc_memory_block();
void * fbgc_malloc(size_t size);
void * fbgc_realloc(void *ptr, size_t size);
void fbgc_free(void *ptr);



#ifdef  __cplusplus
}
#endif

#endif