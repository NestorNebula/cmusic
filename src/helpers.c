#include <stdio.h>
#include <stdarg.h>
#include "tprint.h"
#include "readers.h"
#include "helpers.h"

User user = NULL;

int handle_option_choice(size_t options_count, ...) {
  va_list ap;
  va_start(ap, options_count);
  print_to_stream("\nAvailable options:\n");
  for (int i = 1; i <= options_count; i++) {
    string option_str = va_arg(ap, string);
    print_to_stream("%d: %s\n", i, option_str);
  }
  va_end(ap);

  print_to_stream("\nChoose an option by entering its associated number "
                  "(Enter 0 or another character to return): ");
  bool success = false;
  int option = read_integer(stdin, &success);
  if (!success || option < 1 || option > options_count) return -1;
  return option - 1;
}
