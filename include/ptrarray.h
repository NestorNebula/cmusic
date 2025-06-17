#ifndef PTRARRAY_H
#define PTRARRAY_H

#include <stddef.h>
#include <stdbool.h>

/*
 * PtrArray:
 * This module helps in the creation of an array of pointers.
 * A ptr_array structure always keep track of the length of its
 * content (i.e. number of pointers stored) and is always null terminated.
 */
typedef struct ptr_array *PtrArray;

/*
 * new_ptr_array:
 * Returns a pointer to a new ptr_array structure.
 * Returns null if not enough memory was available to create a new structure.
 */
PtrArray new_ptr_array(void);

/*
 * free_ptr_array:
 * Releases space taken by the ptr_array structure.
 * If free_content is set to true, also releases the space taken by
 * the structure's array. If set to false, the space taken by the
 * structure's array won't be freed.
 * It is important to retrieve the structure's array using get_array before
 * calling this function with free_content set to false, else this will result
 * in a memory leak.
 */
void free_ptr_array(PtrArray ptr_array, bool free_content);

/*
 * add_item:
 * Add ptr to the ptr_array's array.
 * Returns the size of ptr_array's array after adding ptr.
 * Returns 0 if not enough space was available to store 
 * ptr in ptr_array's array.
 */
size_t add_item(PtrArray ptr_array, void *ptr);

/*
 * get_size:
 * Returns the number of items stored in ptr_array's array.
 */
size_t get_size(PtrArray ptr_array);

/*
 * get_array:
 * Returns the array stored in ptr_array 
 * (i.e. a pointer to the first element of the array).
 * If the array is empty, the pointer will point to a null pointer.
 */
void **get_array(PtrArray ptr_array);

#endif
