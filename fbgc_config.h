#ifndef FBGC_CONFIG_H
#define FBGC_CONFIG_H

#define TRUE 1
#define FALSE 0


//############## SYMBOL TABLE ######################

//SYMBOL TABLE STATIC SIZE
#define INITIAL_SYMBOL_TABLE_SIZE 8


//############## MEMORY ######################

#define KILOBYTE (1024)
#define MEGABYTE (1024*KILOBYTE)

/*! @def FBGC_DEFAULT_OBJECT_POOL_SIZE
    \brief fbgc default object pool size in terms of bytes before compilation of the program, this parameter must be changed for different platforms depends on the memory of the system
	\details If the requested memory is bigger than the default pool size, new pools have size by the integer multiplication of the default pool size
*/
#define FBGC_DEFAULT_OBJECT_POOL_SIZE  ((size_t)2048)

/*! @def FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE
    \brief fbgc default object pool size in terms of bytes before compilation of the program, this parameter must be changed for different platforms depends on the memory of the system
	\details If the requested memory is bigger than the default pool size, new pools have size by the integer multiplication of the default pool size
*/
#define FBGC_DEFAULT_RAW_MEMORY_POOL_SIZE  ((size_t)5*KILOBYTE)

/*! @def FBGC_STATIC_INTERNAL_MEMORY_SIZE
    \brief fbgc static internal memory size in bytes. 
	\details Size value can be changed depends on the platform memory
*/
#define FBGC_STATIC_INTERNAL_MEMORY_SIZE ((size_t)(100*KILOBYTE))


/*! @def FBGC_MAX_MEMORY_SIZE 
    \brief fbgc tries to increase its memory sizes unless if hits the max memory size, it is directive to hold memory not grow too much
	\details Maximum size value can be changed depends on the platform memory, if it is an embedded system this value could be useful for a reliable run time
*/
#define FBGC_MAX_MEMORY_SIZE (size_t) 0


#define	FBGC_MALLOC_OPTIMIZATION_NONE       (0x00)
#define	FBGC_MALLOC_OPTIMIZATION_FOR_MEMORY (0x01)
#define	FBGC_MALLOC_OPTIMIZATION_FOR_SPEED  (0x02)
#define	FBGC_MALLOC_OPTIMIZATION_BOTH       (FBGC_MALLOC_OPTIMIZATION_FOR_MEMORY | FBGC_MALLOC_OPTIMIZATION_FOR_SPEED)

#define FBGC_MALLOC_OPTIMIZATION FBGC_MALLOC_OPTIMIZATION_FOR_MEMORY

//############## GARBAGE COLLECTOR ######################

#define FBGC_GC_ROOT_SIZE 3
#define FBGC_GC_MAXIMUM_CYCLE 100000

#define FBGC_GC_RUN_IN_ALLOCATORS TRUE

#define TRACEABLE_POINTER_LIST_INITIAL_CAPACITY 100

#define OBJECT_FREE_LIST_INITIAL_SIZE 32
#define RAW_MEMORY_FREE_LIST_INITIAL_SIZE 64


//############## INTERPRETER ######################

#define FBGC_RUNTIME_PROGRAM_STACK_SIZE 100
#define FBGC_RUNTIME_PROGRAM_RECURSION_LIMIT 100


//########### LOGS


#define LOG_LEVEL_NONE      0
#define LOG_LEVEL_ERROR     1
#define LOG_LEVEL_WARNING 	2
#define LOG_LEVEL_DEBUG     3
#define LOG_LEVEL_VERBOSE   4

#define LOG_LEVEL LOG_LEVEL_VERBOSE

//#define LOG_NO_COLOR

#endif