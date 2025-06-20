#include <stdlib.h>
#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <cjson/cJSON.h>
#include "tmem.h"
#include "cjson-converters.h"

#define ML_JSON 500
#define IS_EMPTY(str) ((str)[0] == '\0')
#define CPY_JSON(str, ...) (snprintf((str), ML_JSON, __VA_ARGS__))
#define ALBUM_NAME "Test album"
#define ARTIST_NAME "Test artist"
#define PLAYLIST_NAME "Test playlist"
#define TRACK_NAME "Test track"
#define USER_NAME "Test user"
#define ID "abc123"
#define DATE "2025-01-01"
#define HREF "https://test.com"

static string get_array_json(string (*get_item_json)(void));
static string get_album_json(void);
static string get_simplified_album_json(void);
static string get_saved_album_json(void);
static string get_artist_json(void);
static string get_simplified_artist_json(void);
static string get_playlist_json(void);
static string get_simplified_playlist_json(void);
static string get_playlist_track_json(void);
static string get_track_json(void);
static string get_simplified_track_json(void);
static string get_saved_track_json(void);
static string get_user_json(void);
static string get_simplified_user_json(void);
static string get_followers_json(void);
static string get_page_json(void);
static string get_restrictions_json(void);
static string get_search_json(void);

cJSON *cjson_ptr = NULL;
void *type_struct_ptr = NULL;
void (*free_function)(void *type_struct_ptr) = NULL;
bool array = false;

static void teardown(void) {
  if (cjson_ptr != NULL) cJSON_Delete(cjson_ptr);
  if (free_function != NULL) {
    if (array) {
      free_array(type_struct_ptr, free_function);
      array = false;
    } else tfree(free_function, type_struct_ptr);
  }
}

Test(cJSON_to_array, creates_array_of_content, .fini = teardown) {
  cJSON *cJSON_array_object = cjson_ptr = 
    cJSON_Parse(get_array_json(get_user_json));
  cJSON *cJSON_array = 
    cJSON_GetObjectItemCaseSensitive(cJSON_array_object, "array");

  User *users = type_struct_ptr = 
    cJSON_to_array(cJSON_array, cJSON_to_user);
  for (User *ptr = users; ptr != NULL; ptr++) {
    User user = *ptr;
    cr_expect(eq(str, user->display_name, USER_NAME),
              "Expected user's display_name to be %s", USER_NAME);
  }
  array = true;
  free_function = free_user;
}

Test(cJSON_to_array, terminates_program_when_given_invalid_data, 
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_user = cjson_ptr = cJSON_Parse(get_user_json());

  type_struct_ptr = cJSON_to_array(cJSON_user, cJSON_to_user);
  array = true;
  free_function = free_user;
}

Test(cJSON_to_album, creates_album_structure, .fini = teardown) {
  cJSON *cJSON_album = cjson_ptr = cJSON_Parse(get_album_json());

  Album album = type_struct_ptr = cJSON_to_album(cJSON_album);
  cr_expect(eq(str, album->id, ID),
            "Expected album's id to be %s", ID);
  cr_expect(eq(str, album->name, ALBUM_NAME),
            "Expected album's name to be %s", ALBUM_NAME);
  free_function = free_album;
}

Test(cJSON_to_album, terminates_program_when_given_invalid_data, 
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_simplified_album = cjson_ptr = 
    cJSON_Parse(get_simplified_album_json());

  type_struct_ptr = cJSON_to_album(cJSON_simplified_album);
  free_function = free_album;
}

Test(cJSON_to_simplified_album, creates_simplified_album_structure, 
     .fini = teardown) {
  cJSON *cJSON_simplified_album = cjson_ptr = 
    cJSON_Parse(get_simplified_album_json());

  SimplifiedAlbum simplified_album =
    type_struct_ptr = cJSON_to_simplified_album(cJSON_simplified_album);
  cr_expect(eq(str, simplified_album->id, ID),
            "Expected simplified album's id to be %s", ID);
  cr_expect(eq(simplified_album->name, ALBUM_NAME),
            "Expect simplified album's name to be %s", ALBUM_NAME);
  free_function = free_simplified_album;
}

Test(cJSON_to_simplified_album, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_album = cjson_ptr = cJSON_Parse(get_album_json());

  type_struct_ptr = cJSON_to_simplified_album(cJSON_album);
  free_function = free_simplified_album;
}

Test(cJSON_to_saved_album, creates_saved_album_structure, .fini = teardown) {
  cJSON *cJSON_saved_album = cjson_ptr = cJSON_Parse(get_saved_album_json());

  SavedAlbum saved_album = type_struct_ptr = 
    cJSON_to_saved_album(cJSON_saved_album);
  cr_expect(eq(str, saved_album->album->id, ID),
            "Expected saved album's album id to be %s", ID);
  cr_expect(eq(str, saved_album->added_at, DATE),
            "Expected saved album add date to be %s", DATE);
  free_function = free_saved_album;
}

Test(cJSON_to_saved_album, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_album = cjson_ptr = cJSON_Parse(get_album_json());

  type_struct_ptr = cJSON_to_saved_album(cJSON_album);
  free_function = free_saved_album;
}

Test(cJSON_to_artist, creates_artist_structure, .fini = teardown) {
  cJSON *cJSON_artist = cjson_ptr = cJSON_Parse(get_artist_json());

  Artist artist = type_struct_ptr = cJSON_to_artist(cJSON_artist);
  cr_expect(eq(str, artist->id, ID),
            "Expected artist's id to be %s", ID);
  cr_expect(eq(str, artist->name, ARTIST_NAME),
            "Expected artist's name to be %s", ARTIST_NAME);
  free_function = free_artist;
}

Test(cJSON_to_artist, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_simplified_artist = cjson_ptr =
    cJSON_Parse(get_simplified_artist_json());

  type_struct_ptr = cJSON_to_artist(cJSON_simplified_artist);
  free_function = free_artist;
}

Test(cJSON_to_simplified_artist, creates_simplified_artist_structure, 
     .fini = teardown) {
  cJSON *cJSON_simplified_artist = cjson_ptr = 
    cJSON_Parse(get_simplified_artist_json());
  
  SimplifiedArtist simplified_artist =
    type_struct_ptr = cJSON_to_simplified_artist(cJSON_simplified_artist);
  cr_expect(eq(str, simplified_artist->id, ID),
            "Expected simplified artist's id to be %s", ID);
  cr_expect(eq(str, simplified_artist->name, ARTIST_NAME),
            "Expected simplified artist's name to be %s", ARTIST_NAME);
  free_function = free_simplified_artist;
}

Test(cJSON_to_simplified_artist, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_artist = cjson_ptr = cJSON_Parse(get_artist_json());

  type_struct_ptr = cJSON_to_simplified_artist(cJSON_artist);
  free_function = free_simplified_artist;
}

Test(cJSON_to_playlist, creates_playlist_structure, .fini = teardown) {
  cJSON *cJSON_playlist = cjson_ptr = cJSON_Parse(get_playlist_json());
  
  Playlist playlist = type_struct_ptr = cJSON_to_playlist(cJSON_playlist);
  cr_expect(eq(str, playlist->id, ID),
            "Expected playlist's id to be %s", ID);
  cr_expect(eq(str, playlist->name, PLAYLIST_NAME),
            "Expected playlist's name to be %s", PLAYLIST_NAME);
  free_function = free_playlist;
}

Test(cJSON_to_playlist, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_simplified_playlist = cjson_ptr =
    cJSON_Parse(get_simplified_playlist_json());
  
  type_struct_ptr = cJSON_to_playlist(cJSON_simplified_playlist);
  free_function = free_playlist;
}

Test(cJSON_to_simplified_playlist, creates_simplified_playlist_structure,
     .fini = teardown) {
  cJSON *cJSON_simplified_playlist = cjson_ptr =
    cJSON_Parse(get_simplified_playlist_json());
  
  SimplifiedPlaylist simplified_playlist =
    type_struct_ptr = cJSON_to_simplified_playlist(cJSON_simplified_playlist);
  cr_expect(eq(str, simplified_playlist->snapshot_id, ID),
            "Expected simplified playlist's snapshot id to be %s", ID);
  cr_expect(eq(str, simplified_playlist->name, PLAYLIST_NAME),
            "Expected simplified playlist's name to be %s", PLAYLIST_NAME);
  free_function = free_simplified_playlist;
}

Test(cJSON_to_simplified_playlist, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_playlist = cjson_ptr = cJSON_Parse(get_playlist_json());

  type_struct_ptr = cJSON_to_simplified_playlist(cJSON_playlist);
  free_function = free_simplified_playlist;
}

Test(cJSON_to_playlist_track, creates_playlist_track_structure,
     .fini = teardown) {
  cJSON *cJSON_playlist_track = cjson_ptr = 
    cJSON_Parse(get_playlist_track_json());

  PlaylistTrack playlist_track = type_struct_ptr = 
    cJSON_to_playlist_track(cJSON_playlist_track);
  cr_expect(eq(str, playlist_track->added_at, DATE),
            "Expected playlist track's added date to be %s", DATE);
  cr_expect(eq(str, playlist_track->added_by.id, ID),
            "Expected id of user that added playlist_track to be %s", ID);
  free_function = free_playlist_track;
}

Test(cJSON_to_playlist_track, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_track = cjson_ptr = cJSON_Parse(get_track_json());

  type_struct_ptr = cJSON_to_playlist_track(cJSON_track);
  free_function = free_playlist_track;
}

Test(cJSON_to_track, creates_track_structure, .fini = teardown) {
  cJSON *cJSON_track = cjson_ptr = cJSON_Parse(get_track_json());
  
  Track track = type_struct_ptr = cJSON_to_track(cJSON_track);
  cr_expect(eq(str, track->id, ID),
            "Expected track's id to be %s", ID);
  cr_expect(eq(str, track->name, TRACK_NAME),
            "Expected track's name to be %s", TRACK_NAME);
  free_function = free_track;
}

Test(cJSON_to_track, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_simplified_track = cjson_ptr = cJSON_Parse(get_simplified_track_json());

  type_struct_ptr = cJSON_to_track(cJSON_simplified_track);
  free_function = free_track;
}

Test(cJSON_to_simplified_track, creates_simplified_track_structure, 
     .fini = teardown) {
  cJSON *cJSON_simplified_track = cjson_ptr = cJSON_Parse(get_simplified_track_json());

  SimplifiedTrack simplified_track =
    type_struct_ptr = cJSON_to_simplified_track(cJSON_simplified_track);
  cr_expect(eq(str, simplified_track->id, ID),
            "Expected simplified track's id to be %s", ID);
  cr_expect(eq(str, simplified_track->name, TRACK_NAME),
            "Expected simplified track's name to be %s", TRACK_NAME);
  free_function = free_simplified_track;
}

Test(cJSON_to_simplified_track, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_track = cjson_ptr = cJSON_Parse(get_track_json());

  type_struct_ptr = cJSON_to_simplified_track(cJSON_track);
  free_function = free_simplified_track;
}

Test(cJSON_to_saved_track, creates_saved_track_structure, .fini = teardown) {
  cJSON *cJSON_saved_track = cjson_ptr = cJSON_Parse(get_saved_track_json());

  SavedTrack saved_track = type_struct_ptr = 
    cJSON_to_saved_track(cJSON_saved_track);
  cr_expect(eq(str, saved_track->added_at, DATE),
            "Expected saved track's added date to be %s", DATE);
  cr_expect(eq(str, saved_track->track->id, ID),
            "Expected saved track's track id to be %s", ID);
  free_function = free_saved_track;
}

Test(cJSON_to_saved_track, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_track = cjson_ptr = cJSON_Parse(get_track_json());

  type_struct_ptr = cJSON_to_saved_track(cJSON_track);
  free_function = free_saved_track;
}

Test(cJSON_to_user, creates_user_structure, .fini = teardown) {
  cJSON *cJSON_user = cjson_ptr = cJSON_Parse(get_user_json());

  User user = type_struct_ptr = cJSON_to_user(cJSON_user);
  cr_expect(eq(str, user->id, ID),
            "Expected user's id to be %s", ID);
  cr_expect(eq(str, user->display_name, USER_NAME),
            "Expected user's display name to be %s", USER_NAME);
  free_function = free_user;
}

Test(cJSON_to_user, terminates_progam_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_simplified_user = cjson_ptr = 
    cJSON_Parse(get_simplified_user_json());

  type_struct_ptr = cJSON_to_user(cJSON_simplified_user);
  free_function = free_user;
}

Test(cJSON_to_simplified_user, creates_simplified_user_structure, 
     .fini = teardown) {
  cJSON *cJSON_simplified_user = cjson_ptr = 
    cJSON_Parse(get_simplified_user_json());

  SimplifiedUser simplified_user = 
    type_struct_ptr = cJSON_to_simplified_user(cJSON_simplified_user);
  cr_expect(eq(str, simplified_user->id, ID),
            "Expected simplified user's id to be %s", ID);
  cr_expect(eq(str, simplified_user->display_name, USER_NAME),
            "Expected simplified user's display name to be %s", USER_NAME);
  free_function = free_simplified_user;
}

Test(cJSON_to_simplified_user, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_user = cjson_ptr = cJSON_Parse(get_user_json());

  type_struct_ptr = cJSON_to_simplified_user(cJSON_user);
  free_function = free_simplified_user;
}

Test(cJSON_to_followers, creates_followers_structure, .fini = teardown) {
  cJSON *cJSON_followers = cjson_ptr = cJSON_Parse(get_followers_json());

  Followers followers = type_struct_ptr = cJSON_to_followers(cJSON_followers);
  cr_expect(followers != NULL, "Expected followers structures to be created");
  free_function = free_followers;
}

Test(cJSON_to_followers, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_restrictions = cjson_ptr = cJSON_Parse(get_restrictions_json());

  type_struct_ptr = cJSON_to_followers(cJSON_restrictions);
  free_function = free_followers;
}

Test(cJSON_to_page, creates_page_structure, .fini = teardown) {
  cJSON *cJSON_page = cjson_ptr = cJSON_Parse(get_page_json());

  Page page = type_struct_ptr = cJSON_to_page(cJSON_page, NULL);
  cr_expect(eq(str, page->href, HREF),
            "Expected page's href to be %s", HREF);
  cr_expect(page->items != NULL, "Expected page's items array to be created");
  free_function = free_page;
}

Test(cJSON_to_page, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_search = cjson_ptr = cJSON_Parse(get_search_json());

  type_struct_ptr = cJSON_to_page(cJSON_search, NULL);
  free_function = free_page;
}

Test(cJSON_to_restrictions, creates_restrictions_structure, .fini = teardown) {
  cJSON *cJSON_restrictions = cjson_ptr = cJSON_Parse(get_restrictions_json());

  Restrictions restrictions = type_struct_ptr = 
    cJSON_to_restrictions(cJSON_restrictions);
  cr_expect(restrictions != NULL, 
            "Expected restrictions structure to be created");
  free_function = free_restrictions;
}

Test(cJSON_to_restrictions, terminates_program_when_given_invalid_data,
     .exit_code = EXIT_FAILURE, .fini = teardown) {
  cJSON *cJSON_followers = cjson_ptr = cJSON_Parse(get_followers_json());

  type_struct_ptr = cJSON_to_restrictions(cJSON_followers);
  free_function = free_restrictions;
}

Test(cJSON_to_search, creates_search_structure, .fini = teardown) {
  cJSON *cJSON_search = cjson_ptr = cJSON_Parse(get_search_json());

  Search search = type_struct_ptr = cJSON_to_search(cJSON_search);
  cr_expect(eq(str, search->tracks->href, HREF),
            "Expected search's tracks page href to be %s", HREF);
  cr_expect(eq(str, search->artists->href, HREF),
            "Expected track's artists page href to be %s", HREF);
  free_function = free_search;
}

Test(cJSON_to_search, terminates_program_when_given_invalid_data, 
     .fini = teardown) {
  cJSON *cJSON_page = cjson_ptr = cJSON_Parse(get_page_json());
  
  type_struct_ptr = cJSON_to_search(cJSON_page);
  free_function = free_search;
}

static string get_array_json(string (*get_item_json)(void)) {
  static char array_json[ML_JSON] = "";
  CPY_JSON(array_json,
"{"
  "\"array\": [%s, %s]"
"}", (*get_item_json)(), (*get_item_json)());

  return array_json;
}

static string get_album_json(void) {
  static char album_json[ML_JSON] = "";
  if (IS_EMPTY(album_json)) {
    CPY_JSON(album_json,
"{"
  "\"album_type\": \"album_type\","
  "\"total_tracks\": 0,"
  "\"id\": \"%s\","
  "\"name\": \"%s\","
  "\"release_date\": \"%s\","
  "\"restrictions\": %s,"
  "\"artists\": [],"
  "\"tracks\": %s,"
  "\"popularity\": 50"
"}", ID, ALBUM_NAME, DATE, get_restrictions_json(), get_page_json());
  }

  return album_json;
}

static string get_simplified_album_json(void) {
  static char simplified_album_json[ML_JSON] = "";
  if (IS_EMPTY(simplified_album_json)) {
    CPY_JSON(simplified_album_json,
"{"
  "\"album_type\": \"album_type\","
  "\"total_tracks\": 0,"
  "\"href\": \"%s\","
  "\"id\": \"%s\","
  "\"name\": \"%s\","
  "\"release_date\": \"%s\","
  "\"restrictions\": %s,"
  "\"artists\": []"
"}", HREF, ID, ALBUM_NAME, DATE, get_restrictions_json());
  }

  return simplified_album_json;
}

static string get_saved_album_json(void) {
  static char saved_album_json[ML_JSON] = "";
  if (IS_EMPTY(saved_album_json)) {
    CPY_JSON(saved_album_json,
"{"
  "\"added_at\": \"%s\","
  "\"album\": %s"
"}", DATE, get_album_json());
  }

  return saved_album_json;
}

static string get_artist_json(void) {
  static char artist_json[ML_JSON] = "";
  if (IS_EMPTY(artist_json)) {
    CPY_JSON(artist_json,
"{"
  "\"followers\": %s,"
  "\"genres\": [],"
  "\"id\": \"%s\","
  "\"name\": \"%s\","
  "\"popularity\": 50"
"}", get_followers_json(), ID, ARTIST_NAME);
  }

  return artist_json;
}

static string get_simplified_artist_json(void) {
  static char simplified_artist_json[ML_JSON] = "";
  if (IS_EMPTY(simplified_artist_json)) {
    CPY_JSON(simplified_artist_json,
"{"
  "\"href\": \"%s\","
  "\"id\": \"%s\","
  "\"name\": \"%s\""
"}", HREF, ID, ARTIST_NAME);
  }

  return simplified_artist_json;
}

static string get_playlist_json(void) {
  static char playlist_json[ML_JSON] = "";
  if (IS_EMPTY(playlist_json)) {
    CPY_JSON(playlist_json,
"{"
  "\"description\": \"Playlist description\","
  "\"id\": \"%s\","
  "\"name\": \"%s\","
  "\"owner\": %s,"
  "\"public\": true,"
  "\"snapshot_id\": \"%s\","
  "\"tracks\": %s"
"}", ID, PLAYLIST_NAME, get_simplified_user_json(), ID, get_page_json());
  }

  return playlist_json;
}
static string get_simplified_playlist_json(void) {
  static char simplified_playlist_json[ML_JSON] = "";
  if (IS_EMPTY(simplified_playlist_json)) {
    CPY_JSON(simplified_playlist_json,
"{"
  "\"description\": \"Playlist description\","
  "\"href\": \"%s\","
  "\"id\": \"%s\","
  "\"name\": \"%s\","
  "\"owner\": %s,"
  "\"public\": true,"
  "\"snapshot_id\": \"%s\","
  "\"tracks\": {"
    "\"href\": \"%s\","
    "\"total\": 0"
  "}"
"}", HREF, ID, PLAYLIST_NAME, get_simplified_user_json(), ID, HREF);
  }

  return simplified_playlist_json;
}

static string get_playlist_track_json(void) {
  static char playlist_track_json[ML_JSON] = "";
  if (IS_EMPTY(playlist_track_json)) {
    CPY_JSON(playlist_track_json,
"{"
  "\"added_at\": \"%s\","
  "\"added_by\": {"
    "\"href\": \"%s\","
    "\"id\": \"%s\""
  "},"
  "\"track\": %s"
"}", DATE, HREF, ID, get_track_json());
  }

  return playlist_track_json;
}

static string get_track_json(void) {
  static char track_json[ML_JSON] = "";
  if (IS_EMPTY(track_json)) {
    CPY_JSON(track_json,
"{"
  "\"album\": %s,"
  "\"artists\": [],"
  "\"duration_ms\": 180000,"
  "\"id\": \"%s\","
  "\"restrictions\": %s,"
  "\"name\": \"%s\","
  "\"popularity\": 50"
"}", get_simplified_album_json(), ID, get_restrictions_json(), TRACK_NAME);
  }

  return track_json;
}

static string get_simplified_track_json(void) {
  static char simplified_track_json[ML_JSON] = "";
  if (IS_EMPTY(simplified_track_json)) {
    CPY_JSON(simplified_track_json,
"{"
  "\"artists\": [],"
  "\"duration_ms\": 180000,"
  "\"href\": \"%s\","
  "\"id\": \"%s\","
  "\"restrictions\": %s,"
  "\"name\": \"%s\""
"}", HREF, ID, get_restrictions_json(), TRACK_NAME);
  }

  return simplified_track_json;
}

static string get_saved_track_json(void) {
  static char saved_track_json[ML_JSON] = "";
  if (IS_EMPTY(saved_track_json)) {
    CPY_JSON(saved_track_json,
"{"
  "\"added_at\": \"%s\","
  "\"track\": %s"
"}", DATE, get_track_json());
  }

  return saved_track_json;
}

static string get_user_json(void) {
  static char user_json[ML_JSON] = "";
  if (IS_EMPTY(user_json)) {
    CPY_JSON(user_json, 
"{"
  "\"display_name\": \"%s\","
  "\"followers\": %s,"
  "\"id\": \"%s\""
"}", USER_NAME, get_followers_json(), ID);
  }

  return user_json;
}
static string get_simplified_user_json(void) {
  static char simplified_user_json[ML_JSON] = "";
  if (IS_EMPTY(simplified_user_json)) {
    CPY_JSON(simplified_user_json,
"{"
  "\"href\": \"%s\","
  "\"id\": \"%s\","
  "\"display_name\": \"%s\""
"}", HREF, ID, USER_NAME);
  }

  return simplified_user_json;
}
static string get_followers_json(void) {
  static char followers_json[ML_JSON] = "";
  if (IS_EMPTY(followers_json)) {
    CPY_JSON(followers_json,
"{"
  "\"total\": 0"
"}");
  }

  return followers_json;
}
static string get_page_json(void) {
  static char page_json[ML_JSON] = "";
  if (IS_EMPTY(page_json)) {
    CPY_JSON(page_json,
"{"
  "\"href\": \"%s\","
  "\"limit\": 0,"
  "\"next\": \"%s/next\","
  "\"total\": 0,"
  "\"items\": []"
"}", HREF, HREF);
  }

  return page_json;
}
static string get_restrictions_json(void) {
  static char restrictions_json[ML_JSON] = "";
  if (IS_EMPTY(restrictions_json)) {
    CPY_JSON(restrictions_json,
"{"
  "\"reason\": \"restrictions' reason\""
"}");
  }

  return restrictions_json;
}
static string get_search_json(void) {
  static char search_json[ML_JSON] = "";
  if (IS_EMPTY(search_json)) {
    CPY_JSON(search_json,
"{"
  "\"tracks\": %s,"
  "\"artists\": %s,"
  "\"albums\": %s,"
  "\"playlists\": %s"
"}", get_page_json(), get_page_json(), get_page_json(), get_page_json());
  }

  return search_json;
}
