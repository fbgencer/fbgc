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

/*! @def FBGC_DEFAULT_POOL_SIZE
    \brief fbgc default pool size in terms of bytes before compilation of the program, this parameter must be changed for different platforms depends on the memory of the system
	\details If the requested memory is bigger than the default pool size, new pools have size by the integer multiplication of the default pool size
*/
#define FBGC_DEFAULT_POOL_SIZE  10*KILOBYTE

/*! @def FBGC_STATIC_INTERNAL_MEMORY_SIZE
    \brief fbgc static internal memory size in bytes. 
	\details Size value can be changed depends on the platform memory
*/
#define FBGC_STATIC_INTERNAL_MEMORY_SIZE 2*KILOBYTE


/*! @def FBGC_MAX_MEMORY_SIZE 
    \brief fbgc tries to increase its memory sizes unless if hits the max memory size, it is directive to hold memory not grow too much
	\details Maximum size value can be changed depends on the platform memory, if it is an embedded system this value could be useful for a reliable run time
*/
#define FBGC_MAX_MEMORY_SIZE -1


/*! @fn void * fbgc_malloc(size_t size)
    \brief Similar to c-malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
*/
void initialize_fbgc_memory_block();

/*! @fn void * fbgc_malloc(size_t size)
    \brief Similar to c-malloc function
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes
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


/*! @fn void * fbgc_malloc_static(size_t size, bool is_raw_memory)
    \brief Allocated from fbgc internal static buffer
	\details fbgc_object or any other data type can be allocated with this function. 
		These object never collected by the garbage collector nor they die. They live until the end of the program
    \param size : Requested size in bytes
	\param is_raw_memory : If the requested byte is not for fbgc_object types then this parameter must be true, otherwise it must be false
    \return Allocated address of the requested bytes
*/
void * fbgc_malloc_static(size_t size, bool is_raw_memory);

/*! @fn void * fbgc_realloc(void *ptr, size_t size)
    \brief Similar to c-realloc function
	\details fbgc_objects pointers are immutable so they cannot be reallocated because (most of the time) 
		they hold another data structure that lives in raw memory pool (such as tuples, matrices etc..). Only the object are allocated by fbgc_malloc can be reallocated.
		Otherwise function asserts and close the program because it is a programmer error
	\details 
    \param size : Requested size in bytes
    \return Allocated address of the requested bytes (It may be new address or returns the old address if there is enough space to grow)
*/
void * fbgc_realloc(void *ptr, size_t size);

/*! @fn void fbgc_free(void *ptr)
    \brief Adds pointer to the free list, for the next allocation this given memory can help allocater function to not grow its memory (if there is no space).
	\details If allocated object (can be allocated any allocater function, see fbgc_malloc and fbgc_malloc_object) is really not need for the next cycle of the program, then giving
		pointer to fbgc memory api can help to not allocate new variables all the time. This given pointer will be added to free list and for the next allocation it may be used because
		fbgc memory try to minimize fragmentation in its internal memory pools
    \param size : Address of the garbage memory
*/
void fbgc_free(void *ptr);

/*! @fn print_fbgc_memory_block()
    \brief Dumps all the content in fbgc memory pools
*/
void print_fbgc_memory_block();


uint32_t capacity_in_bytes_fbgc_raw_memory(void * x);

#define capacity_fbgc_raw_memory(x,block_size) (capacity_in_bytes_fbgc_raw_memory(x)/block_size)


uint8_t fbgc_gc_register_pointer(void * base_ptr,uint8_t nested, ...);


/**
 * @brief fbgc Garbage collector states
 */
typedef enum {
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
	struct fbgc_memory_pool * current_raw_memory;
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

#define set_raw_memory_gc_white(x) ( set_gc_white(cast_from_raw_memory_data_to_raw_memory(x)) )
#define set_raw_memory_gc_gray(x) ( set_gc_gray(cast_from_raw_memory_data_to_raw_memory(x)) )
#define set_raw_memory_gc_black(x) ( set_gc_black(cast_from_raw_memory_data_to_raw_memory(x)) )
#define set_raw_memory_gc_dark(x) ( set_gc_dark(cast_from_raw_memory_data_to_raw_memory(x)) )


void fbgc_gc_mark_object(struct fbgc_object * obj);
uint8_t fbgc_gc_mark_pointer(void * base_ptr, size_t block_size);
void fbgc_gc_init(struct fbgc_object * root);
void fbgc_gc_mark();
void fbgc_gc_sweep();
void fbgc_gc_run();


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