#ifndef FBGC_MEMORY
#define FBGC_MEMORY

#ifdef __cplusplus
extern "C" {
#endif

#define KILOBYTE 1024


#define PAGE_SIZE  10* KILOBYTE //[byte]


#define MEM_OBJECT_POOL 0 
#define MEM_RAW_BUFFER_POOL 1

struct fbgc_memory_pool{
	uint8_t type;
	uint8_t * data;
	uint8_t * tptr;	
	size_t size;
	struct fbgc_memory_pool * next;
};


struct fbgc_memory_block{
	struct fbgc_memory_pool * raw_buffer_head;
	struct fbgc_memory_pool * object_pool_head;
	struct fbgc_object empty_chunk_head;
};

struct fbgc_raw_buffer{
	uint32_t mark_bit : 2;
	uint32_t capacity : 30;
	uint8_t data[0];
};

#define cast_from_raw_buffer_data_to_raw_buffer(x)( (struct fbgc_raw_buffer *)((uint8_t*)x-sizeof(struct fbgc_raw_buffer))   )


extern struct fbgc_memory_block fbgc_memb;

void initialize_fbgc_memory_block();
void print_fbgc_memory_block();
void free_fbgc_memory_block();
void * fbgc_malloc(size_t size);
void * fbgc_malloc_object(size_t size);
void * fbgc_malloc_static(size_t size);
void * fbgc_realloc(void *ptr, size_t size);
void fbgc_free(void *ptr);




uint32_t capacity_in_bytes_fbgc_raw_memory(void * x);

#define capacity_fbgc_raw_memory(x,block_size) (capacity_in_bytes_fbgc_raw_memory(x)/block_size)


uint8_t fbgc_gc_register_pointer(void * base_ptr,uint8_t nested, ...);

enum {
	GC_STATE_NOT_INITIALIZED = 0,
	GC_STATE_READY_TO_SWEEP,
	GC_STATE_INITIALIZED,
	GC_STATE_PAUSED,
	GC_STATE_MARKING,
	GC_STATE_SWEEPING,
	GC_STATE_STOPPED
}GC_STATES;

#define TRACEABLE_POINTER_LIST_INITIAL_CAPACITY 2

struct traceable_pointer_entry{
	void * base_ptr;
	uint8_t * tptr;
	size_t block_size;
	uint8_t which_memory_pool;
};

struct traceable_pointer_list{
	struct traceable_pointer_entry * data;
	size_t size;
	size_t capacity;
};


struct fbgc_garbage_collector{
	uint8_t state;
	size_t threshold;
	size_t cycle;
	uint8_t * last_location;
	struct fbgc_memory_pool * current_raw_buffer;
	struct fbgc_memory_pool * current_object_pool;
	struct traceable_pointer_list ptr_list;
	struct fbgc_queue tpe_queue;
};

extern struct fbgc_garbage_collector fbgc_gc;


#define GC_WHITE 0
#define GC_GRAY  1
#define GC_BLACK 2
#define GC_DARK 3

#define set_gc_white(x)	( (x)->mark_bit = GC_WHITE)
#define set_gc_gray(x)	( (x)->mark_bit = GC_GRAY)
#define set_gc_black(x)	( (x)->mark_bit = GC_BLACK)
#define set_gc_dark(x)	( (x)->mark_bit = GC_DARK)

#define is_gc_white(x)	( (x)->mark_bit == GC_WHITE)
#define is_gc_gray(x)	( (x)->mark_bit == GC_GRAY)
#define is_gc_black(x)	( (x)->mark_bit == GC_BLACK)
#define is_gc_dark(x)	( (x)->mark_bit == GC_DARK)

#define set_raw_buffer_gc_white(x) ( set_gc_white(cast_from_raw_buffer_data_to_raw_buffer(x)) )
#define set_raw_buffer_gc_gray(x) ( set_gc_gray(cast_from_raw_buffer_data_to_raw_buffer(x)) )
#define set_raw_buffer_gc_black(x) ( set_gc_black(cast_from_raw_buffer_data_to_raw_buffer(x)) )
#define set_raw_buffer_gc_dark(x) ( set_gc_dark(cast_from_raw_buffer_data_to_raw_buffer(x)) )


void fbgc_gc_mark_object(struct fbgc_object * obj);
uint8_t fbgc_gc_mark_pointer(void * base_ptr, size_t block_size);
void fbgc_gc_init(struct fbgc_object * root);
void fbgc_gc_mark();
void fbgc_gc_sweep();


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