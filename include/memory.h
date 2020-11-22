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



extern size_t allocation;

struct __fbgc_mem_free_list_entry{
	void * start;
	void * end;
	size_t size;
};



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
	GC_STATE_INITIALIZED,
	GC_STATE_PAUSED,
	GC_STATE_SCANNING_POOLS,
	GC_STATE_SCANNING_CSTACK,
	GC_STATE_FREE_LIST_READY,
	GC_STATE_STOPPED
}GC_STATES;


struct fbgc_garbage_collector{
	struct fbgc_object ** roots[FBGC_GC_ROOT_SIZE];
	size_t max_cycle;
	size_t cycle;
	void * stack_bottom;
	struct fbgc_queue tpe_queue;
	uint8_t state;
	
	uint8_t roots_scanned:1;
	uint8_t run_gc_for_next_cycle:1;
	uint8_t pools_scan_finished:1;
	uint8_t cstack_scan_finished:1;
	uint8_t free_list_is_ready:1;
};

void fbgc_gc_mark_raw_memory_pointer(void * base_ptr, size_t block_size);
void fbgc_gc_mark_fbgc_object(struct fbgc_object * obj_ptr);

void fbgc_gc_init(void * stack_bottom, uint8_t roots_len, ...);
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