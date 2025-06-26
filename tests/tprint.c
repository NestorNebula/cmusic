#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdlib.h>
#include <string.h>
#include "ptrarray.h"
#include "tmem.h"
#include "tprint.h"

#define DL_CONTENT 100
#define ITEM_COUNT 3

#define USER_NAME "Test user name"
#define NAME "Test name"
#define DESCRIPTION "Test description"
#define GENRE "Test genre"
#define DATE "2025-01-01"

#define END_IF(condition) if (condition) exit(EXIT_FAILURE)
#define IS_NULL(ptr) ptr == NULL

extern FILE *print_stream;
FILE *stream;
string printed_content;

static void setup(void) {
  print_stream = stream = tmpfile();
}

static void teardown(void) {
  free(printed_content);
  fclose(stream);
  print_stream = stdout;
}

static string get_printed_content(void);

static string create_string(string str);

Test(print_array, prints_every_item, .init = setup, .fini = teardown) {
  PtrArray ptr_array = new_ptr_array();
  for (int i = 0; i < ITEM_COUNT; i++) {
    SimplifiedAlbum simplified_album = talloc(new_simplified_album);
    simplified_album->name = create_string(NAME);
    add_item(ptr_array, simplified_album);
  }
  
  print_array(get_array(ptr_array), print_album, false, false);
  get_printed_content();
  string previous_search = NULL;
  for (int i = 0; i < ITEM_COUNT; i++) {
    previous_search = strstr(IS_NULL(previous_search)
                              ? printed_content
                              : previous_search, NAME);
    cr_expect(not(IS_NULL(previous_search)),
              "Expected item's name to be printed");
    if (IS_NULL(previous_search)) break;
  }

  free_ptr_array(ptr_array, true, free_simplified_album);
}

Test(print_album, prints_details, .init = setup, .fini = teardown) {
  Album album = talloc(new_album);
  album->name = create_string(NAME);
  album->release_date = create_string(DATE);

  print_album(album, true, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, album->name))),
            "Expected album's name to be printed");
  cr_expect(not(IS_NULL(strstr(printed_content, album->release_date))),
            "Expected album's release date to be printed");

  tfree(free_album, album);
}

Test(print_album, prints_essentials, .init = setup, .fini = teardown) {
  Album album = talloc(new_album);
  album->name = create_string(NAME);
  album->release_date = create_string(DATE);

  print_album(album, false, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, album->name))),
            "Expected album's name to be printed");
  cr_expect(IS_NULL(strstr(printed_content, album->release_date)),
            "Expected album's release date not to be printed");
  
  tfree(free_album, album);
}

Test(print_artist, prints_details, .init = setup, .fini = teardown) {
  Artist artist = talloc(new_artist);
  PtrArray ptr_array = new_ptr_array();
  for (int i = 0; i < ITEM_COUNT; i++) {
    add_item(ptr_array, create_string(GENRE));
  }
  artist->genres = (string *) get_array(ptr_array);
  artist->name = create_string(NAME);

  print_artist(artist, true, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, "Followers"))),
            "Expected artist's followers count to be printed");
  for (int i = 0; i < ITEM_COUNT; i++) {
    cr_expect(not(IS_NULL(strstr(printed_content, artist->genres[i]))),
              "Expected artist's music genres to be printed");
  }
  cr_expect(not(IS_NULL(strstr(printed_content, artist->name))),
            "Expected artist's name to be printed");

  free_ptr_array(ptr_array, false, NULL);
  tfree(free_artist, artist);
}

Test(print_artist, prints_essentials, .init = setup, .fini = teardown) {
  Artist artist = talloc(new_artist);
  artist->name = create_string(NAME);

  print_artist(artist, false, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, artist->name))),
            "Expected artist's name to be printed");
  cr_expect(IS_NULL(strstr(printed_content, "Followers")),
            "Expected artist's followers count not to be printed");

  tfree(free_artist, artist);
}

Test(print_playlist, prints_details, .init = setup, .fini = teardown) {
  Playlist playlist = talloc(new_playlist);
  playlist->description = create_string(DESCRIPTION);
  playlist->name = create_string(NAME);
  playlist->owner = talloc(new_simplified_user);
  playlist->owner->display_name = create_string(USER_NAME);

  print_playlist(playlist, true, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, playlist->description))),
            "Expected playlist's description to be printed");
  cr_expect(not(IS_NULL(strstr(printed_content, playlist->name))),
            "Expected playlist's name to be printed");
  cr_expect(not(IS_NULL(strstr(printed_content,
                               playlist->owner->display_name))),
            "Expected playlist owner's name to be printed");

  tfree(free_playlist, playlist);
}

Test(print_playlist, prints_essentials, .init = setup, .fini = teardown) {
  Playlist playlist = talloc(new_playlist);
  playlist->description = create_string(DESCRIPTION);
  playlist->name = create_string(NAME);

  print_playlist(playlist, false, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, playlist->name))),
            "Expected playlist's name to be printed");
  cr_expect(IS_NULL(strstr(printed_content, playlist->description)),
            "Expected playlist's description not to be printed");

  tfree(free_playlist, playlist);
}

Test(print_track, prints_details, .init = setup, .fini = teardown) {
  Track track = talloc(new_track);
  track->name = create_string(NAME);
  
  print_track(track, true, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, track->name))),
            "Expected track's name to be printed");
  cr_expect(not(IS_NULL(strstr(printed_content, "Duration"))),
            "Expected track's duration to be printed");

  tfree(free_track, track);
}

Test(print_track, prints_essentials, .init = setup, .fini = teardown) {
  Track track = talloc(new_track);
  track->name = create_string(NAME);

  print_track(track, false, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, track->name))),
            "Expected track's name to be printed");
  cr_expect(IS_NULL(strstr(printed_content, "Duration")),
            "Expected track's duration not to be printed");

  tfree(free_track, track);
}

Test(print_user, prints_details, .init = setup, .fini = teardown) {
  User user = talloc(new_user);
  user->display_name = create_string(USER_NAME);

  print_user(user, true, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, user->display_name))),
            "Expected user's name to be printed");
  cr_expect(not(IS_NULL(strstr(printed_content, "Followers"))),
            "Expected user's followers count to be printed");

  tfree(free_user, user);
}

Test(print_user, prints_essentials, .init = setup, .fini = teardown) {
  User user = talloc(new_user);
  user->display_name = create_string(USER_NAME);

  print_user(user, false, false);
  get_printed_content();
  cr_expect(not(IS_NULL(strstr(printed_content, user->display_name))),
            "Expected user's name to be printed");
  cr_expect(IS_NULL(strstr(printed_content, "Followers")),
            "Expected user's followers count not to be printed");

  tfree(free_user, user);
}

static string get_printed_content(void) {
  rewind(stream);
  size_t size = DL_CONTENT, len = 0;
  printed_content = malloc(size + 1);
  END_IF(IS_NULL(printed_content));
  int ch;
  while ((ch = getc(stream)) != EOF) {
    if (len >= size) {
      printed_content = realloc(printed_content, size * 2 + 1);
      END_IF(IS_NULL(printed_content));
      size *= 2;
    }
    printed_content[len++] = ch;
  }
  printed_content[len] = '\0';
  return printed_content;
}

static string create_string(string str) {
  string new_string = malloc(strlen(str) + 1);
  END_IF(IS_NULL(new_string));
  strcpy(new_string, str);
  return new_string;
}
