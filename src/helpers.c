#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "query.h"
#include "ptrarray.h"
#include "tmem.h"
#include "tprint.h"
#include "readers.h"
#include "helpers.h"

#define IS_NULL(ptr) (ptr == NULL)

User user = NULL;
SimplifiedPlaylist *owned_playlists = NULL,
                   *followed_playlists = NULL;
Artist *followed_artists = NULL;


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

void update_playlists(void) {
  free_array((void **) owned_playlists, free_simplified_playlist);
  free_array((void **) followed_playlists, free_simplified_playlist);

  PtrArray owned_playlists_ptr_array = new_ptr_array();
  PtrArray followed_playlists_ptr_array = new_ptr_array();
  
  int playlists_count = 0;
  for (;;) {
    Page page = query_get_user_playlists(playlists_count);
    SimplifiedPlaylist *playlists = page->items;
    for (int i = 0; !IS_NULL(playlists[i]); i++) {
      if (!strcmp(playlists[i]->owner->display_name, user->display_name)) {
        add_item(owned_playlists_ptr_array, playlists[i]);
      } else add_item(followed_playlists_ptr_array, playlists[i]);
      playlists_count++;
    }
    if (playlists_count >= page->total) {
      tfree(free_page, page);
      break;
    }
    tfree(free_page, page);
  }

  owned_playlists = 
    (SimplifiedPlaylist *) get_array(owned_playlists_ptr_array);
  followed_playlists =
    (SimplifiedPlaylist *) get_array(followed_playlists_ptr_array);
  free_ptr_array(owned_playlists_ptr_array, false, NULL);
  free_ptr_array(followed_playlists_ptr_array, false, NULL);
}

void update_followed_artists(void) {
  free_array((void **) followed_artists, free_artist);

  PtrArray ptr_array = new_ptr_array();
  int artists_count = 0;
  string previous = NULL;
  for (;;) {
    Page page = query_get_followed_artists(previous);
    Artist *artists = page->items;
    for (int i = 0; !IS_NULL(artists[i]); i++) {
      add_item(ptr_array, artists[i]);
      artists_count++;
      previous = artists[i]->id;
    }
    if (artists_count >= page->total) {
      tfree(free_page, page);
      break;
    }
    tfree(free_page, page);
  }

  followed_artists = (Artist *) get_array(ptr_array);
  free_ptr_array(ptr_array, false, NULL);
}
