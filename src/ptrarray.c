#include <stdlib.h>
#include "ptrarray.h"

struct ptr_array {
  size_t size;
  void **array;
};

/*
 * resize_ptr_array:
 * Resizes ptr_array to fit for new_size pointers (+1 for the null pointer).
 * The function's return value will match new_size if no error occurred,
 * else it will return 0.
 */
static size_t resize_ptr_array(PtrArray ptr_array, size_t new_size);

PtrArray new_ptr_array(void) {
  PtrArray ptr_array = malloc(sizeof(ptr_array));
  if (ptr_array == NULL) return ptr_array;
  ptr_array->array = malloc(sizeof(void *));
  if (ptr_array->array == NULL) {
    free(ptr_array);
    return NULL;
  }
  ptr_array->size = 0;

  return ptr_array;
}

void free_ptr_array(PtrArray ptr_array, bool free_content) {
  if (free_content) free(ptr_array->array);
  free(ptr_array);
}

size_t add_item(PtrArray ptr_array, void *ptr) {
  if (!resize_ptr_array(ptr_array, ptr_array->size + 1)) return 0;
  ptr_array->array[ptr_array->size - 1] = ptr;
  return ptr_array->size;
}

size_t get_size(PtrArray ptr_array) {
  return ptr_array->size;
}

void **get_array(PtrArray ptr_array) {
  return ptr_array->array;
}

static size_t resize_ptr_array(PtrArray ptr_array, size_t new_size) {
  void **new_array = realloc(ptr_array->array, 
                             (new_size + 1) * sizeof(void *));
  if (new_array == NULL) return 0;
  ptr_array->array = new_array;
  ptr_array->size = new_size;
  ptr_array->array[ptr_array->size] = NULL;

  return new_size;
}
