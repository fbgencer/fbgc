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


#ifndef FBGC_MEMORY
#define FBGC_MEMORY

#ifdef __cplusplus
extern "C" {
#endif

#define KILOBYTE 1024
#define MEGABYTE 1024*KILOBYTE


/*! @def FBGC_DEFAULT_OBJECT_POOL_SIZE
    \brief fbgc default object pool size in terms of bytes before compilation of the program, this parameter must be changed for different platforms depends on the memory of the system
	\details If the requested memory is bigger than the default pool size, new pools have size by the integer multiplication of the default pool size
*/
#define FBGC_DEFAULT_OBJECT_POOL_SIZE  (size_t)1*KILOBYTE

/*! @def FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE
    \brief fbgc default object pool size in terms of bytes before compilation of the program, this parameter must be changed for different platforms depends on the memory of the system
	\details If the requested memory is bigger than the default pool size, new pools have size by the integer multiplication of the default pool size
*/
#define FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE  (size_t)1024*1024*KILOBYTE

/*! @def FBGC_STATIC_INTERNAL_MEMORY_SIZE
    \brief fbgc static internal memory size in bytes. 
	\details Size value can be changed depends on the platform memory
*/
#define FBGC_STATIC_INTERNAL_MEMORY_SIZE (size_t)(100*1024*KILOBYTE)


/*! @def FBGC_MAX_MEMORY_SIZE 
    \brief fbgc tries to increase its memory sizes unless if hits the max memory size, it is directive to hold memory not grow too much
	\details Maximum size value can be changed depends on the platform memory, if it is an embedded system this value could be useful for a reliable run time
*/
#define FBGC_MAX_MEMORY_SIZE (size_t) 0

struct fbgc_memory_pool{
	uint8_t * data; 	//<! Actual data pointer which holds the pool buffer, all allocated objects live in this buffer
	uint8_t * tptr;		//<! Traveler pointer which moves only forward in the data buffer
	uint8_t * end; 		//<! Pointer holds the end of the pool, pool size can be extracted from pool_end - data
	size_t size;    	//<! Size of the pool
	struct fbgc_memory_pool * next; //<! Pools are connected to each other via linked list, each pool holds the next pool till NULL
};

struct fbgc_memory_block{
	struct fbgc_memory_pool * raw_memory_head; 	//<! Raw memory pools are held by this pointer as a linked list data structure
	struct fbgc_memory_pool * object_pool_head; //<! Object memory pools are held by this pointer as a linked list data structure
	struct fbgc_vector raw_memory_free_list;	//<! Raw memory free list as fbgc_vector
	struct fbgc_vector object_free_list; 		//<! Object free list as fbgc_vector
	size_t total_allocated_size;				//<! Total allocated size from the system heap, used to check condition for FBGC_MAX_MEMORY_SIZE
};

struct fbgc_static_memory_pool{
	uint8_t data [FBGC_STATIC_INTERNAL_MEMORY_SIZE];  //<! fbgc static memory in bytes
	uint8_t * tptr;	//<! Traveler pointer for new allocations
	uint8_t * end; 	//<! End of the static buffer which is the same data + FBGC_STATIC_INTERNAL_MEMORY_SIZE
};

/*! @fn void initialize_fbgc_memory_block()
    \brief Initialize memory enviroenment for the program
*/
void initialize_fbgc_memory_block();

/*! @fn free_fbgc_memory_block()
    \brief Gives all the allocated pools to the system and clears everything about fbgc
*/
void free_fbgc_memory_block();

/*! @fn void * fbgc_malloc(size_t size)
    \brief Similar to c-malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
void * fbgc_malloc(size_t size);

/*! @fn struct fbgc_object * fbgc_malloc_object(size_t size)
    \brief Similar to c-malloc function but only for fbgc_object types
	\details Other than fbgc_objects, allocation must be performed by fbgc_malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
struct fbgc_object * fbgc_malloc_object(size_t size);


/*! @fn void * fbgc_malloc_static(size_t size)
    \brief Allocated from fbgc internal static buffer
	\details Raw data types can be allocated in the internal buffer with this allocator. These object never collected by the garbage collector nor they die. They live until the end of the program
    \param size : Requested size in bytes
*/
void * fbgc_malloc_static(size_t size);

/*! @fn void * fbgc_malloc_static(size_t size)
    \brief Allocated from fbgc internal static buffer
	\details fbgc object types can be allocated in the internal buffer with this allocator. These object never collected by the garbage collector nor they die. They live until the end of the program
    \param size : Requested size in bytes
*/
void * fbgc_malloc_object_static(size_t size);

/*! @fn void * fbgc_realloc(void *ptr, size_t size)
    \brief Similar to c-realloc function
	\details fbgc_objects pointers are immutable so they cannot be reallocated because (most of the time) 
		they hold another data structure that lives in raw memory pool (such as tuples, matrices etc..). Only the object are allocated by fbgc_malloc can be reallocated.
		Otherwise function asserts and close the program because it is a programmer error
	\param ptr : Old address of the allocated memory
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes (It may be new address or returns the old address if there is enough space to grow)
*/
void * fbgc_realloc(void *ptr, size_t size);

/*! @fn void fbgc_free(void *ptr)
    \brief Adds pointer to the free list, for the next allocation this given memory can help allocater function to not grow its memory (if there is no space).
	\details If allocated object (can be allocated any allocater function, see fbgc_malloc and fbgc_malloc_object) is really not need for the next cycle of the program, then giving
		pointer to fbgc memory api can help to not allocate new variables all the time. This given pointer will be added to free list and for the next allocation it may be used because
		fbgc memory try to minimize fragmentation in its internal memory pools. Given 
    \param ptr : Address of the garbage memory (can be NULL)
*/
void fbgc_free(void *ptr);

/*! @fn print_fbgc_memory_block()
    \brief Dumps all the content in fbgc memory pools
*/
void print_fbgc_memory_block();


/*! @fn uint32_t capacity_fbgc_raw_memory(void * ptr,size_t block_size)
    \brief Returns the capacity of the allocated memory from checking the internal memory
	\details If a data block is allocated by fbgc_malloc, fbgc_memory adds capacity tag to this pointer and user does not need to specify capacity in its own data structure
		memory api can check the capacity and returns it.
	\param ptr : Allocated data pointer returned by fbgc_malloc allocater
	\param block_size : Block size of the allocated content. For example if ptr is an integer array then block size is simply sizeof(int)
	\return Number of blocks that ptr can hold
*/
uint32_t capacity_fbgc_raw_memory(void * ptr,size_t block_size);


#define	FBGC_MALLOC_OPTIMIZATION_NONE 	0x00
#define	FBGC_MALLOC_OPTIMIZATION_FOR_MEMORY 0x01
#define	FBGC_MALLOC_OPTIMIZATION_FOR_SPEED 	0x02
#define	FBGC_MALLOC_OPTIMIZATION_MEMORY_AND_SPEED 0x01 | 0x02

extern size_t allocation;

struct __fbgc_mem_free_list_entry{
	void * start;
	void * end;
	size_t size;
};

#define TRACEABLE_POINTER_LIST_INITIAL_CAPACITY 1024*100

/*! @def struct traceable_pointer_entry 
    \brief fbgc.
*/
struct traceable_pointer_entry{
	void * base_ptr; 		//<! Base pointer of the memory which is claimed by an owner from the program
	uint8_t * tptr;			//<! Traveler pointer to scan all the array based on base_ptr
	size_t block_size;		//<! In order to find new object tptr must be shifted by block_size, total size is know from base_ptr(either fbgc_object or fbgc_raw_memory)
};

/**
 * @brief fbgc Garbage collector states
 */
typedef enum {
	GC_STATE_NOT_INITIALIZED = 0,
	GC_STATE_READY_TO_SWEEP,
	GC_STATE_INITIALIZED,
	GC_STATE_PAUSED,
	GC_STATE_SCANNING_POOLS,
	GC_STATE_SCANNING_CSTACK,
	GC_STATE_FREE_LIST_READY,
	GC_STATE_STOPPED
}GC_STATES;

#define FBGC_GC_MAXIMUM_CYCLE 1000000

struct fbgc_garbage_collector{
	size_t max_cycle;
	size_t cycle;
	//uint8_t * last_location;
	void * stack_bottom;
	//struct fbgc_memory_pool * current_raw_memory;
	//struct fbgc_memory_pool * current_object_pool;
	//struct traceable_pointer_list ptr_list;
	struct fbgc_queue tpe_queue;
	uint8_t state;
	uint8_t pools_scan_finished:1;
	uint8_t cstack_scan_finished:1;
	uint8_t free_list_is_ready:1;
};



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

#define set_raw_memory_gc_white(x) ( set_gc_white(cast_from_raw_memory_data_to_raw_memory(x)) )
#define set_raw_memory_gc_gray(x) ( set_gc_gray(cast_from_raw_memory_data_to_raw_memory(x)) )
#define set_raw_memory_gc_black(x) ( set_gc_black(cast_from_raw_memory_data_to_raw_memory(x)) )
#define set_raw_memory_gc_dark(x) ( set_gc_dark(cast_from_raw_memory_data_to_raw_memory(x)) )


void fbgc_gc_mark_pointer(void * base_ptr, size_t block_size);
void fbgc_gc_init(struct fbgc_object * root,void * stack_bottom);
void fbgc_gc_run(size_t run_cycle);


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