#ifndef FBGC_MEMORY
#define FBGC_MEMORY

#ifdef __cplusplus
extern "C" {
#endif

#define KILOBYTE 1024


#define PAGE_SIZE  128 * KILOBYTE //[byte]


#define MEM_OBJECT_POOL 0 
#define MEM_RAW_BUFFER 1

struct fbgc_memory_pool{
	uint8_t type;
	void * data;
	void * tptr;	
	size_t size;
	struct fbgc_memory_pool * next;
};


struct fbgc_memory_block{
	struct fbgc_memory_pool * raw_buffer_head;	
	struct fbgc_memory_pool * object_pool_head;
	struct fbgc_object empty_chunk_head;
};



extern struct fbgc_memory_block fbgc_memb;

void initialize_fbgc_memory_block();
void print_fbgc_memory_block();
void free_fbgc_memory_block();
void * fbgc_malloc(size_t size);
void * fbgc_malloc_object(size_t size);
void * fbgc_realloc(void *ptr, size_t size);
void fbgc_free(void *ptr);
long capacity_in_bytes_fbgc_raw_memory(void * x);
#define capacity_fbgc_raw_memory(x,block_size) (capacity_in_bytes_fbgc_raw_memory(x)/block_size)

#ifdef LOG_MEMORY
#define MEMORY_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define MEMORY_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _MEMORY_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _MEMORY_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define MEMORY_LOGV(...)
#define MEMORY_LOGD(...)
#define _MEMORY_LOGV(...)
#define _MEMORY_LOGD(...)
#endif



#ifdef  __cplusplus
}
#endif

#endif