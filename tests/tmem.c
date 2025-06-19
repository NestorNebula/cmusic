#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "tmem.h"
#include "types.h"

#define CR_NOT_NULL(ptr) ((ptr) != NULL)

void *type_structure_ptr = NULL;
void (*free_type_structure)(void *type_structure_ptr) = NULL;

static void teardown(void) {
  if (type_structure_ptr != NULL && free_type_structure != NULL) {
    (*free_type_structure)(type_structure_ptr);
  }
}

Test(talloc, allocates_memory_for_a_structure_using_function_ptr_argument) {
  type_structure_ptr = talloc(new_album);
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_album;
}

Test(new_album, allocates_memory_for_album_structure, .fini = teardown) {
  type_structure_ptr = new_album();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_album;
}

Test(new_simplified_album, allocates_memory_for_simplified_album_structure,
     .fini = teardown) {
  type_structure_ptr = new_simplified_album();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_simplified_album;
}

Test(new_saved_album, allocates_memory_for_saved_album, .fini = teardown) {
  type_structure_ptr = new_saved_album();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_saved_album;
}

Test(new_artist, allocates_memory_for_artist_structure, .fini = teardown) {
  type_structure_ptr = new_artist();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_artist;
}

Test(new_simplified_artist, allocates_memory_for_simplified_artist_structure, 
     .fini = teardown) {
  type_structure_ptr = new_simplified_artist();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_simplified_artist;
}

Test(new_playlist, allocates_memory_for_playlist_structure, .fini = teardown) {
  type_structure_ptr = new_playlist();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_playlist;
}

Test(new_simplified_playlist, 
     allocates_memory_for_simplified_playlist_structure, 
     .fini = teardown) {
  type_structure_ptr = new_simplified_playlist();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_simplified_playlist;
}

Test(new_playlist_track, allocates_memory_for_playlist_track_structure, 
     .fini = teardown) {
  type_structure_ptr = new_playlist_track();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_playlist_track;
}

Test(new_track, allocates_memory_for_track_structure, .fini = teardown) {
  type_structure_ptr = new_track();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_track;
}

Test(new_simplified_track, allocates_memory_for_simplified_track_structure,
     .fini = teardown) {
  type_structure_ptr = new_simplified_track();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_simplified_track;
}

Test(new_saved_track, allocates_memory_for_saved_track_structure,
     .fini = teardown) {
  type_structure_ptr = new_saved_track();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_saved_track;
}

Test(new_user, allocates_memory_for_user_structure, .fini = teardown) {
  type_structure_ptr = new_user();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_user;
}

Test(new_simplified_user, allocates_memory_for_simplified_user_structure,
     .fini = teardown) {
  type_structure_ptr = new_simplified_user();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_simplified_user;
}

Test(new_followers, allocates_memory_for_followers_structure, 
     .fini = teardown) {
  type_structure_ptr = new_followers();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_followers;
}

Test(new_page, allocates_memory_for_page_structure, .fini = teardown) {
  type_structure_ptr = new_page();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_page;
}

Test(new_restrictions, allocates_memory_for_restrictions_structure,
     .fini = teardown) {
  type_structure_ptr = new_restrictions();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_restrictions;
}

Test(new_search, allocates_memory_for_search_structure, .fini = teardown) {
  type_structure_ptr = new_search();
  CR_NOT_NULL(type_structure_ptr);
  free_type_structure = free_search;
}
