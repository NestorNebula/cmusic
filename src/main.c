#include <stdlib.h>
#include <stdio.h>
#include "type-handlers.h"
#include "query.h"
#include "tprint.h"
#include "readers.h"

#define IS_NULL(ptr) (ptr == NULL)

extern string token;

User user = NULL;

/*
 * handle_user_connection:
 * Queries the API to get user's informations.
 * Terminates program if the query fails, else stores
 * user informations in user.
 */
void handle_user_connection(void);

int main(int argc, char **argv) {
  print_stream = stdout;

  if (argc != 2) {
    print_to_stream("\nUsage: cmusic token\n");
    exit(EXIT_FAILURE);
  } else {
    token = argv[1];
    handle_user_connection();
  }

  print_to_stream("\nHello %s!\n", user->display_name);
}

void handle_user_connection(void) {
  user = query_get_user();
  if (IS_NULL(user) || IS_NULL(user->display_name)) {
    print_to_stream("\nInvalid token. You can get a valid token by following "
                    "the steps provided in the docs.\n");
    exit(EXIT_FAILURE);
  }
}
