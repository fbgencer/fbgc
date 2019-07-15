#ifndef FBGC_MEMORY
#define FBGC_MEMORY

#ifdef __cplusplus
extern "C" {
#endif

#define KILOBYTE 1024


#define INITIAL_INTERNAL_BUFFER_SIZE 1 //[byte]
#define PAGE_SIZE  4*KILOBYTE //[byte]

struct fbgc_memory_pool{
	void * data;
	void * tptr;	
	size_t size;
	struct fbgc_memory_pool * next;
};


struct fbgc_memory_block{
	void * internal_buffer_head;	
	size_t internal_buffer_size;

	struct fbgc_memory_pool * object_pool_head;
	size_t object_pool_size;

	struct fbgc_object empty_chunk_head;
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