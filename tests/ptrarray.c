#include <stdlib.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "ptrarray.h"

PtrArray ptr_array = NULL;

static void setup(void) {
  ptr_array = new_ptr_array();
  if (ptr_array == NULL) exit(EXIT_FAILURE);
}

static void teardown(void) {
  if (ptr_array != NULL) free_ptr_array(ptr_array, true, NULL);
}

Test(new_ptr_array, returns_pointer_to_ptr_array_structure, .fini = teardown) {
  ptr_array = new_ptr_array();
  cr_expect(ptr_array != NULL,
            "Expected ptr_array to point to a new ptr_array structure");
}

Test(add_item, returns_number_of_item_after_adding_item,
     .init = setup, .fini = teardown) {
  cr_expect(eq(add_item(ptr_array, (void *) NULL), 1),
            "Expected add_item to return new number of items");
  cr_expect(eq(add_item(ptr_array, (void *) NULL), 2),
            "Expected add_item to return new number of items");
}

Test(get_size, returns_array_size, .init = setup, .fini = teardown) {
  cr_expect(zero(get_size(ptr_array)),
            "Expected get_size to return the number of items in array");
  add_item(ptr_array, (void *) NULL);
  cr_expect(eq(get_size(ptr_array), 1),
            "Expected get_size to return the number of items in array");
  add_item(ptr_array, (void *) NULL);
  cr_expect(eq(get_size(ptr_array), 2),
            "Expected get_size to return the number of items in array");
}

Test(get_array, returns_null_terminated_array, 
     .init = setup, .fini = teardown) {
  int i = 1, j = 2;
  add_item(ptr_array, &i);
  add_item(ptr_array, &j);
  int **int_array = (int **) get_array(ptr_array);
  for (int k = 0; int_array[k] != NULL; k++) {
    cr_assert(eq(k + 1, *int_array[k]),
              "Expected item %s of array to have value %s", k, k + 1);
  }
}
