#ifndef FBGC_ARRAY_OBJECTH
#define FBGC_ARRAY_OBJECTH

#ifdef __cplusplus
extern "C" {
#endif

/*! @details fbgc_array_object holds typical C-arrays for any size of a block.
In order to add, remove, push or whatever the operation you need for an array, see the functions of this object.

There are some important parts of its implementation: 

Normally we were calculating the content location by just adding 
the size of each variable in the structure, however GCC allows us to put void * content[0] variable and it does not affect
the size of the structure. It's like not there but compiler already known its address and gives us an option to use it.
Yet, it is a void pointer we must cast it before using this array. One can also use this array pointer or can call wrapper
functions. I believe functions are safe and more meaningful to use.

See /module_test/array_test.c to understand how to use the following functions.

*/
struct fbgc_array_object{
    struct fbgc_object base; 	//!< Base object that holds type information
    size_t block_size; 			//!< Block size of each slot
    size_t capacity; 			//!< Capacity that we can hold
    size_t size; 				//!< Actual size of the array
    void * content[0];			//!< The content of the array object, it tricks the compiler to use this variable as our data array
};


/*! @def cast_fbgc_object_as_array(x)
    \brief Casts struct #fbgc_object* to struct #fbgc_array_object*, typical C casting but it is more readable
    \param x : Array object
*/
#define cast_fbgc_object_as_array(x)(((struct fbgc_array_object*) x))

/*! @def size_fbgc_array_object(x)
    \brief Gets the size of #fbgc_array_object, instead of using access operator we call it as a function
    \param x : Array object
*/
#define size_fbgc_array_object(x)(cast_fbgc_object_as_array(x)->size)

/*! @def capacity_fbgc_array_object(x)
    \brief Gets the capacity of #fbgc_array_object, instead of using access operator we call it as a function
    \param x : Array object
*/
#define capacity_fbgc_array_object(x)(cast_fbgc_object_as_array(x)->capacity)

/*! @def block_size_fbgc_array_object(x)
    \brief Gets the block size of #fbgc_array_object, instead of using access operator we call it as a function
    \param x : Array object
*/
#define block_size_fbgc_array_object(x)(cast_fbgc_object_as_array(x)->block_size)

/*! @def sizeof_fbgc_array_object(x)
    \brief Gets the total size of the given structure, it also calculates the size of data and block size, hence we use it to know the actual boundaries of our object
    \param x : Array object
*/
#define sizeof_fbgc_array_object(x) (sizeof(struct fbgc_array_object)+ ( capacity_fbgc_array_object(x) * block_size_fbgc_array_object(x) ) )

/*! @def at_fbgc_array_object(ar,it)
    \brief Gets the value at location 'it', calculates where the data lies in the memory by using block size
    \details Note that we need to cast content object as uint8_t* because we want to find the address by adding bytes so compiler makes addition for one byte at a time
    \param x : Array object
    \param it : Location of the variable
*/
#define at_fbgc_array_object(x,it)((uint8_t*)cast_fbgc_object_as_array(x)->content + it * block_size_fbgc_array_object(x))

/*! @def is_empty_fbgc_array_object(x)
    \brief Returns 1 if the array object is empty, otherwise returns zero
    \param x : Array object
*/
#define is_empty_fbgc_array_object(x)(size_fbgc_array_object(x) == 0)


/*! @fn struct fbgc_object * new_fbgc_array_object(size_t cap, size_t b_size)
    \brief Returns a new array object for a requested size of capacity and block size
    \details Note that it may give more than you wanted because it calculates capacity of an array by the multiples of 2, for more info see #array_calculate_new_capacity_from_size
    \param cap : Requested capacity
    \param bsize : Requested block size
    \return Address of the newly requested array object
*/
struct fbgc_object * new_fbgc_array_object(size_t cap, size_t bsize);

/*! @fn void set_in_fbgc_array_object(struct fbgc_object * self,void * obj,int index)
    \brief Set a variable inside an array object
    \details Instead of using array representation of data content, this function allows to put data at a given index value.
    \details Negative index values are valid and allows to put data in reverse order. -1 index is equal to size-1 index. If the index
    value is greater than the size of the array, does nothing.
    \details Note that if the size is not defined before using this function, it may give some error. It does not change the size of the array object,
    to use this function first allocate enough space and then change size. It differs from push_back function which can change the memory content of array object.
    \param self : Array object
    \param obj : Variable to be assigned at index value
    \param index : Location of the assigned value in the data array, it can be negative
*/
void set_in_fbgc_array_object(struct fbgc_object * self,void * obj,int index);

/*! @fn void * get_address_in_fbgc_array_object(struct fbgc_object * self,int index)
    \brief Get the address of a variable inside an array object
    \details Return the address of a given index, negative index values are valid (reverse ordering). If the index
    value is greater than the size of the array, returns NULL. Before calling this function casting is compulsary.
    \param self : Array object
    \param obj : Variable at index value
    \param index : Location of the requested value in the data array, it can be negative.
    \return Data at a given indexn the array as void*, casting is compulsary
*/
void * get_address_in_fbgc_array_object(struct fbgc_object * self,int index);

/*! @fn void * get_back_in_fbgc_array_object(struct fbgc_object * self)
    \brief Returns the last value in the array, same as #get_address_in_fbgc_array_object(self,-1)
    \param self : Array object
    \return Last value of the array as void*, casting is compulsary
*/
void * get_back_in_fbgc_array_object(struct fbgc_object * self);

/*! @fn void * get_front_in_fbgc_array_object(struct fbgc_object * self)
    \brief Returns the first value in the array, same as #get_address_in_fbgc_array_object(self,0)
    \param self : Array object
    \return First value of the array as void*, casting is compulsary
*/
void * get_front_in_fbgc_array_object(struct fbgc_object * self);

/*! @fn struct fbgc_object * push_back_fbgc_array_object(struct fbgc_object * self,void * obj)
    \brief Pushes from back to given object into the array object
    \details The location of self object may change while updating the array, thus returns the new address of the array. It may change or not it depends on the capacity of the object.
    \param self : Array object
    \param obj : The object wanted to put in the array
    \return New address of the array object, if may return exactly the self itself or change it.
*/
struct fbgc_object * push_back_fbgc_array_object(struct fbgc_object * self,void * obj);

void _push_back_fbgc_array_object(struct fbgc_object ** arr,void * obj);


/*! \cond DOCUMENT_EVERYTHING
	Lets not document this part, just log definitions nothing important 
*/
#ifdef LOG_ARRAY_OBJECT
#define ARRAY_OBJECT_LOGV(format,...) LOGV(format,##__VA_ARGS__)
#define ARRAY_OBJECT_LOGD(format,...) LOGD(format,##__VA_ARGS__)
#define _ARRAY_OBJECT_LOGV(format,...) _LOGV(format,##__VA_ARGS__)
#define _ARRAY_OBJECT_LOGD(format,...) _LOGD(format,##__VA_ARGS__) 
#else
#define ARRAY_OBJECT_LOGV(...)
#define ARRAY_OBJECT_LOGD(...)
#define _ARRAY_OBJECT_LOGV(...)
#define _ARRAY_OBJECT_LOGD(...)
#endif

#ifdef  __cplusplus
}
#endif

#endif
