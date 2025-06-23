#include <stdlib.h>
#include <string.h>
#include "ptrarray.h"
#include "tmem.h"
#include "cjson-converters.h"

#define cJSON_to_number(cJSON_number) ((cJSON_number)->valueint)

/*
 * cJSON_get_item_safe:
 * Searches an item inside cJSON_object that has a key of key.
 * If an item is found, check that it has the expected type using
 * the function pointed by cJSON_isType.
 * If the item has the expected type, returns it.
 * If any error occurs or any condition isn't met, terminates program.
 */
static cJSON *get_cJSON_item_safe(cJSON *cJSON_object, string key, 
                            int (*cJSON_IsType)(const cJSON *));
/*
 * cJSON_to_string:
 * Copy cJSON_string's value into a new string.
 * Returns a pointer to the first character of the string if no error occurred,
 * else terminates program.
 */
static void *cJSON_to_string(cJSON *cJSON_string);

void **cJSON_to_array(cJSON *cJSON_array,
                      void *(*cJSON_to_item_type)(cJSON *item)) {
  END_IF(!cJSON_IsArray(cJSON_array));
  PtrArray ptr_array = new_ptr_array();
  END_IF(IS_NULL(ptr_array));
  cJSON *cJSON_item = NULL;
  int i = 0;
  cJSON_ArrayForEach(cJSON_item, cJSON_array) {
    END_IF(add_item(ptr_array, cJSON_to_item_type(cJSON_item)) != ++i);
  }
  void **array = get_array(ptr_array);
  free_ptr_array(ptr_array, false, NULL);
  return array;
}


void *cJSON_to_album(cJSON *cJSON_album) {
  END_IF(!cJSON_IsObject(cJSON_album));
  Album album = talloc(new_album);
  END_IF(IS_NULL(album));

  cJSON *cJSON_album_type = 
    get_cJSON_item_safe(cJSON_album, "album_type", cJSON_IsString),
  *cJSON_total_tracks = 
    get_cJSON_item_safe(cJSON_album, "total_tracks", cJSON_IsNumber),
  *cJSON_id = get_cJSON_item_safe(cJSON_album, "id", cJSON_IsString),
  *cJSON_name = get_cJSON_item_safe(cJSON_album, "name", cJSON_IsString),
  *cJSON_release_date =
    get_cJSON_item_safe(cJSON_album, "release_date", cJSON_IsString),
  *cJSON_restrictions = 
    cJSON_GetObjectItemCaseSensitive(cJSON_album, "restrictions"),
  *cJSON_artists = get_cJSON_item_safe(cJSON_album, "artists", cJSON_IsArray),
  *cJSON_tracks = get_cJSON_item_safe(cJSON_album, "tracks", cJSON_IsObject),
  *cJSON_popularity =
    get_cJSON_item_safe(cJSON_album, "popularity", cJSON_IsNumber);

  album->restrictions = cJSON_IsObject(cJSON_restrictions) 
    ? cJSON_to_restrictions(cJSON_restrictions) 
    : NULL;

  album->artists = 
    (SimplifiedArtist *) cJSON_to_array(cJSON_artists, 
                                        cJSON_to_simplified_artist);

  album->tracks = cJSON_to_page(cJSON_tracks, cJSON_to_simplified_track);

  album->album_type = cJSON_to_string(cJSON_album_type); 
  album->id = cJSON_to_string(cJSON_id);
  album->name = cJSON_to_string(cJSON_name);
  album->release_date = cJSON_to_string(cJSON_release_date);
  
  album->total_tracks = cJSON_to_number(cJSON_total_tracks);
  album->popularity = cJSON_to_number(cJSON_popularity);

  return album;
}

void *cJSON_to_simplified_album(cJSON *cJSON_simplified_album) {
  END_IF(!cJSON_IsObject(cJSON_simplified_album));
  SimplifiedAlbum simplified_album = talloc(new_simplified_album);
  END_IF(IS_NULL(simplified_album));

  cJSON *cJSON_album_type =
    get_cJSON_item_safe(cJSON_simplified_album, "album_type", cJSON_IsString),
  *cJSON_total_tracks =
    get_cJSON_item_safe(cJSON_simplified_album, "total_tracks",
                        cJSON_IsNumber),
  *cJSON_href =
    get_cJSON_item_safe(cJSON_simplified_album, "href", cJSON_IsString),
  *cJSON_id =
    get_cJSON_item_safe(cJSON_simplified_album, "id", cJSON_IsString),
  *cJSON_name =
    get_cJSON_item_safe(cJSON_simplified_album, "name", cJSON_IsString),
  *cJSON_release_date =
    get_cJSON_item_safe(cJSON_simplified_album, "release_date",
                        cJSON_IsString),
  *cJSON_restrictions = 
    cJSON_GetObjectItemCaseSensitive(cJSON_simplified_album, "restrictions"),
  *cJSON_artists =
    get_cJSON_item_safe(cJSON_simplified_album, "artists", cJSON_IsArray);

  simplified_album->restrictions = cJSON_IsObject(cJSON_restrictions)
    ? cJSON_to_restrictions(cJSON_restrictions)
    : NULL;

  simplified_album->artists = 
    (SimplifiedArtist *) cJSON_to_array(cJSON_artists,
                                        cJSON_to_simplified_artist);

  simplified_album->album_type = cJSON_to_string(cJSON_album_type);
  simplified_album->href = cJSON_to_string(cJSON_href);
  simplified_album->id = cJSON_to_string(cJSON_id);
  simplified_album->name = cJSON_to_string(cJSON_name);
  simplified_album->release_date = cJSON_to_string(cJSON_release_date);

  simplified_album->total_tracks = cJSON_to_number(cJSON_total_tracks);

  return simplified_album;
}

void *cJSON_to_saved_album(cJSON *cJSON_saved_album) {
  END_IF(!cJSON_IsObject(cJSON_saved_album));
  SavedAlbum saved_album = talloc(new_saved_album);
  END_IF(IS_NULL(saved_album));

  cJSON *cJSON_added_at =
    get_cJSON_item_safe(cJSON_saved_album, "added_at", cJSON_IsString),
  *cJSON_album = 
    get_cJSON_item_safe(cJSON_saved_album, "album", cJSON_IsObject);

  saved_album->album = cJSON_to_album(cJSON_album);

  saved_album->added_at = cJSON_to_string(cJSON_added_at);

  return saved_album;
}

void *cJSON_to_artist(cJSON *cJSON_artist) {
  END_IF(!cJSON_IsObject(cJSON_artist));
  Artist artist = talloc(new_artist);
  END_IF(IS_NULL(artist));

  cJSON *cJSON_followers =
    get_cJSON_item_safe(cJSON_artist, "followers", cJSON_IsObject),
  *cJSON_genres = get_cJSON_item_safe(cJSON_artist, "genres", cJSON_IsArray),
  *cJSON_id = get_cJSON_item_safe(cJSON_artist, "id", cJSON_IsString),
  *cJSON_name = get_cJSON_item_safe(cJSON_artist, "name", cJSON_IsString),
  *cJSON_popularity = 
    get_cJSON_item_safe(cJSON_artist, "popularity", cJSON_IsNumber);

  artist->followers = cJSON_to_followers(cJSON_followers);
  artist->genres = 
    (string *) cJSON_to_array(cJSON_genres, cJSON_to_string);

  artist->id = cJSON_to_string(cJSON_id);
  artist->name = cJSON_to_string(cJSON_name);

  artist->popularity = cJSON_to_number(cJSON_popularity);

  return artist;
}


void *cJSON_to_simplified_artist(cJSON *cJSON_simplified_artist) {
  END_IF(!cJSON_IsObject(cJSON_simplified_artist));
  SimplifiedArtist simplified_artist = talloc(new_simplified_artist);
  END_IF(IS_NULL(simplified_artist));

  cJSON *cJSON_href = 
    get_cJSON_item_safe(cJSON_simplified_artist, "href", cJSON_IsString),
  *cJSON_id = 
    get_cJSON_item_safe(cJSON_simplified_artist, "id", cJSON_IsString),
  *cJSON_name =
    get_cJSON_item_safe(cJSON_simplified_artist, "name", cJSON_IsString);

  simplified_artist->href = cJSON_to_string(cJSON_href);
  simplified_artist->id = cJSON_to_string(cJSON_id);
  simplified_artist->name = cJSON_to_string(cJSON_name);

  return simplified_artist;
}

void *cJSON_to_playlist(cJSON *cJSON_playlist) {
  END_IF(!cJSON_IsObject(cJSON_playlist));
  Playlist playlist = talloc(new_playlist);
  END_IF(IS_NULL(playlist));

  cJSON *cJSON_description =
    cJSON_GetObjectItemCaseSensitive(cJSON_playlist, "description"),
  *cJSON_id = get_cJSON_item_safe(cJSON_playlist, "id", cJSON_IsString),
  *cJSON_name = get_cJSON_item_safe(cJSON_playlist, "name", cJSON_IsString),
  *cJSON_owner =
    get_cJSON_item_safe(cJSON_playlist, "owner", cJSON_IsObject),
  *cJSON_public =
    get_cJSON_item_safe(cJSON_playlist, "public", NULL),
  *cJSON_snapshot_id =
    get_cJSON_item_safe(cJSON_playlist, "snapshot_id", cJSON_IsString),
  *cJSON_tracks =
    get_cJSON_item_safe(cJSON_playlist, "tracks", cJSON_IsObject);

  playlist->owner = cJSON_to_simplified_user(cJSON_owner);
  playlist->tracks = cJSON_to_page(cJSON_tracks, cJSON_to_playlist_track);

  playlist->description = cJSON_IsString(cJSON_description)
    ? cJSON_to_string(cJSON_description)
    : NULL;
  playlist->id = cJSON_to_string(cJSON_id);
  playlist->name = cJSON_to_string(cJSON_name);
  playlist->snapshot_id = cJSON_to_string(cJSON_snapshot_id);

  playlist->public = cJSON_IsTrue(cJSON_public);

  return playlist;
}

void *cJSON_to_simplified_playlist(cJSON *cJSON_simplified_playlist) {
  END_IF(!cJSON_IsObject(cJSON_simplified_playlist));
  SimplifiedPlaylist simplified_playlist = talloc(new_simplified_playlist);
  END_IF(IS_NULL(simplified_playlist));

  cJSON *cJSON_description =
    cJSON_GetObjectItemCaseSensitive(cJSON_simplified_playlist, "description"),
  *cJSON_href = 
    get_cJSON_item_safe(cJSON_simplified_playlist, "href", cJSON_IsString),
  *cJSON_id =
    get_cJSON_item_safe(cJSON_simplified_playlist, "id", cJSON_IsString),
  *cJSON_name =
    get_cJSON_item_safe(cJSON_simplified_playlist, "name", cJSON_IsString),
  *cJSON_owner =
    get_cJSON_item_safe(cJSON_simplified_playlist, "owner", cJSON_IsObject),
  *cJSON_public =
    get_cJSON_item_safe(cJSON_simplified_playlist, "public", NULL),
  *cJSON_snapshot_id =
    get_cJSON_item_safe(cJSON_simplified_playlist, "snapshot_id",
                        cJSON_IsString),
  *cJSON_tracks =
    get_cJSON_item_safe(cJSON_simplified_playlist, "tracks", cJSON_IsObject);
  cJSON *cJSON_tracks_href =
    get_cJSON_item_safe(cJSON_tracks, "href", cJSON_IsString),
  *cJSON_tracks_total = 
    get_cJSON_item_safe(cJSON_tracks, "total", cJSON_IsNumber);

  simplified_playlist->owner = cJSON_to_simplified_user(cJSON_owner);
  
  simplified_playlist->description = cJSON_to_string(cJSON_description);
  simplified_playlist->href = cJSON_to_string(cJSON_href);
  simplified_playlist->id = cJSON_to_string(cJSON_id);
  simplified_playlist->name = cJSON_to_string(cJSON_name);
  simplified_playlist->snapshot_id = cJSON_to_string(cJSON_snapshot_id);
  simplified_playlist->tracks.href = cJSON_to_string(cJSON_tracks_href);

  simplified_playlist->public = cJSON_IsTrue(cJSON_public);
  
  simplified_playlist->tracks.total = cJSON_to_number(cJSON_tracks_total);

  return simplified_playlist;
}

void *cJSON_to_playlist_track(cJSON *cJSON_playlist_track) {
  END_IF(!cJSON_IsObject(cJSON_playlist_track));
  PlaylistTrack playlist_track = talloc(new_playlist_track);
  END_IF(IS_NULL(playlist_track));

  cJSON *cJSON_added_at =
    get_cJSON_item_safe(cJSON_playlist_track, "added_at", cJSON_IsString),
  *cJSON_added_by =
    get_cJSON_item_safe(cJSON_playlist_track, "added_by", cJSON_IsObject),
  *cJSON_track =
    get_cJSON_item_safe(cJSON_playlist_track, "track", cJSON_IsObject);
  cJSON *cJSON_added_by_href =
    get_cJSON_item_safe(cJSON_added_by, "href", cJSON_IsString),
  *cJSON_added_by_id =
    get_cJSON_item_safe(cJSON_added_by, "id", cJSON_IsString);

  playlist_track->track = cJSON_to_track(cJSON_track);
  playlist_track->added_at = cJSON_to_string(cJSON_added_at);
  playlist_track->added_by.href = cJSON_to_string(cJSON_added_by_href);
  playlist_track->added_by.id = cJSON_to_string(cJSON_added_by_id);

  return playlist_track;
}

void *cJSON_to_track(cJSON *cJSON_track) {
  END_IF(!cJSON_IsObject(cJSON_track));
  Track track = talloc(new_track);
  END_IF(IS_NULL(track));

  cJSON *cJSON_album = 
    get_cJSON_item_safe(cJSON_track, "album", cJSON_IsObject),
  *cJSON_artists = get_cJSON_item_safe(cJSON_track, "artists", cJSON_IsArray),
  *cJSON_duration_ms =
    get_cJSON_item_safe(cJSON_track, "duration_ms", cJSON_IsNumber),
  *cJSON_id = get_cJSON_item_safe(cJSON_track, "id", cJSON_IsString),
  *cJSON_restrictions =
    cJSON_GetObjectItemCaseSensitive(cJSON_track, "description"),
  *cJSON_name = get_cJSON_item_safe(cJSON_track, "name", cJSON_IsString),
  *cJSON_popularity =
    get_cJSON_item_safe(cJSON_track, "popularity", cJSON_IsNumber);

  track->restrictions = IS_NULL(cJSON_restrictions)
    ? NULL
    : cJSON_to_restrictions(cJSON_restrictions);

  track->album = cJSON_to_simplified_album(cJSON_album);
  track->artists = 
    (SimplifiedArtist *) cJSON_to_array(cJSON_artists, 
                                        cJSON_to_simplified_artist);
  
  track->id = cJSON_to_string(cJSON_id);
  track->name = cJSON_to_string(cJSON_name);

  track->duration_ms = cJSON_to_number(cJSON_duration_ms);
  track->popularity = cJSON_to_number(cJSON_popularity);
  
  return track;
}

void *cJSON_to_simplified_track(cJSON *cJSON_simplified_track) {
  END_IF(!cJSON_IsObject(cJSON_simplified_track));
  SimplifiedTrack simplified_track = talloc(new_simplified_track);
  END_IF(IS_NULL(simplified_track));

  cJSON *cJSON_artists =
    get_cJSON_item_safe(cJSON_simplified_track, "artists", cJSON_IsArray),
  *cJSON_duration_ms =
    get_cJSON_item_safe(cJSON_simplified_track, "duration_ms", cJSON_IsNumber),
  *cJSON_href =
    get_cJSON_item_safe(cJSON_simplified_track, "href", cJSON_IsString),
  *cJSON_id =
    get_cJSON_item_safe(cJSON_simplified_track, "id", cJSON_IsString),
  *cJSON_restrictions =
    cJSON_GetObjectItemCaseSensitive(cJSON_simplified_track, "restrictions"),
  *cJSON_name =
    get_cJSON_item_safe(cJSON_simplified_track, "name", cJSON_IsString);

  simplified_track->restrictions = cJSON_IsObject(cJSON_restrictions)
    ? cJSON_to_restrictions(cJSON_restrictions)
    : NULL;

  simplified_track->artists = 
    (SimplifiedArtist *) cJSON_to_array(cJSON_artists, 
                                        cJSON_to_simplified_artist);
  
  simplified_track->href = cJSON_to_string(cJSON_href);
  simplified_track->id = cJSON_to_string(cJSON_id);
  simplified_track->name = cJSON_to_string(cJSON_name);
  
  simplified_track->duration_ms = cJSON_to_number(cJSON_duration_ms);

  return simplified_track;
}

void *cJSON_to_saved_track(cJSON *cJSON_saved_track) {
  END_IF(!cJSON_IsObject(cJSON_saved_track));
  SavedTrack saved_track = talloc(new_saved_track);
  END_IF(IS_NULL(saved_track));

  cJSON *cJSON_added_at =
    get_cJSON_item_safe(cJSON_saved_track, "added_at", cJSON_IsString),
  *cJSON_track = 
    get_cJSON_item_safe(cJSON_saved_track, "track", cJSON_IsObject);

  saved_track->track = cJSON_to_track(cJSON_track);
  
  saved_track->added_at = cJSON_to_string(cJSON_added_at);

  return saved_track;
}

void *cJSON_to_user(cJSON *cJSON_user) {
  END_IF(!cJSON_IsObject(cJSON_user));
  User user = talloc(new_user);
  END_IF(IS_NULL(user));

  cJSON *cJSON_display_name = 
    get_cJSON_item_safe(cJSON_user, "display_name", NULL),
  *cJSON_followers = 
    get_cJSON_item_safe(cJSON_user, "followers", cJSON_IsObject),
  *cJSON_id = get_cJSON_item_safe(cJSON_user, "id", cJSON_IsString);

  user->display_name = cJSON_IsString(cJSON_display_name)
    ? cJSON_to_string(cJSON_display_name)
    : NULL;

  user->followers = cJSON_to_followers(cJSON_followers);

  user->id = cJSON_to_string(cJSON_id);

  return user;
}

void *cJSON_to_simplified_user(cJSON *cJSON_simplified_user) {
  END_IF(!cJSON_IsObject(cJSON_simplified_user));
  SimplifiedUser simplified_user = talloc(new_simplified_user);
  END_IF(IS_NULL(simplified_user));

  cJSON *cJSON_href =
    get_cJSON_item_safe(cJSON_simplified_user, "href", cJSON_IsString),
  *cJSON_id = get_cJSON_item_safe(cJSON_simplified_user, "id", cJSON_IsString),
  *cJSON_display_name =
    get_cJSON_item_safe(cJSON_simplified_user, "display_name", NULL);

  simplified_user->display_name = cJSON_IsString(cJSON_display_name)
    ? cJSON_to_string(cJSON_display_name)
    : NULL;

  simplified_user->href = cJSON_to_string(cJSON_href);
  simplified_user->id = cJSON_to_string(cJSON_id);

  return simplified_user;
}

void *cJSON_to_followers(cJSON *cJSON_followers) {
  END_IF(!cJSON_IsObject(cJSON_followers));
  Followers followers = talloc(new_followers);
  END_IF(IS_NULL(followers));

  cJSON *cJSON_total = 
    get_cJSON_item_safe(cJSON_followers, "total", cJSON_IsNumber);

  followers->total = cJSON_to_number(cJSON_total);

  return followers;
}

void *cJSON_to_page(cJSON *cJSON_page,
                    void *(*cJSON_to_item_type)(cJSON *item)) {
  END_IF(!cJSON_IsObject(cJSON_page));
  Page page = talloc(new_page);
  END_IF(IS_NULL(page));

  cJSON *cJSON_href = get_cJSON_item_safe(cJSON_page, "href", cJSON_IsString),
  *cJSON_limit = get_cJSON_item_safe(cJSON_page, "limit", cJSON_IsNumber),
  *cJSON_next = get_cJSON_item_safe(cJSON_page, "next", NULL),
  *cJSON_total = get_cJSON_item_safe(cJSON_page, "total", cJSON_IsNumber),
  *cJSON_items = get_cJSON_item_safe(cJSON_page, "items", cJSON_IsArray);

  page->next = cJSON_IsString(cJSON_next)
    ? cJSON_to_string(cJSON_next)
    : NULL;

  page->items = cJSON_to_array(cJSON_items, cJSON_to_item_type);
  
  page->href = cJSON_to_string(cJSON_href);

  page->limit = cJSON_to_number(cJSON_limit);
  page->total = cJSON_to_number(cJSON_total);

  return page;
}

void *cJSON_to_restrictions(cJSON *cJSON_restrictions) {
  END_IF(!cJSON_IsObject(cJSON_restrictions));
  Restrictions restrictions = talloc(new_restrictions);
  END_IF(IS_NULL(restrictions));

  cJSON *cJSON_reason = 
    get_cJSON_item_safe(cJSON_restrictions, "reason", cJSON_IsString);

  restrictions->reason = cJSON_to_string(cJSON_reason);

  return restrictions;
}

void *cJSON_to_search(cJSON *cJSON_search) {
  END_IF(!cJSON_IsObject(cJSON_search));
  Search search = talloc(new_search);
  END_IF(IS_NULL(search));

  cJSON *cJSON_tracks =
    cJSON_GetObjectItemCaseSensitive(cJSON_search, "tracks"),
  *cJSON_artists = cJSON_GetObjectItemCaseSensitive(cJSON_search, "artists"),
  *cJSON_albums = cJSON_GetObjectItemCaseSensitive(cJSON_search, "albums"),
  *cJSON_playlists = 
    cJSON_GetObjectItemCaseSensitive(cJSON_search, "playlists");

  search->tracks = cJSON_IsObject(cJSON_tracks)
    ? cJSON_to_page(cJSON_tracks, cJSON_to_track)
    : NULL;

  search->artists = cJSON_IsObject(cJSON_artists)
    ? cJSON_to_page(cJSON_tracks, cJSON_to_artist)
    : NULL;

  search->albums = cJSON_IsObject(cJSON_albums)
    ? cJSON_to_page(cJSON_albums, cJSON_to_simplified_album)
    : NULL;

  search->playlists = cJSON_IsObject(cJSON_playlists)
    ? cJSON_to_page(cJSON_playlists, cJSON_to_simplified_playlist)
    : NULL;

  return search;
}

static cJSON *get_cJSON_item_safe(cJSON *cJSON_object, string key, 
                            int (*cJSON_IsType)(const cJSON *)) {
  cJSON *cJSON_item = cJSON_GetObjectItemCaseSensitive(cJSON_object, key);
  if (!IS_NULL(cJSON_IsType)) {
    END_IF(!cJSON_IsType(cJSON_item));
  } else END_IF(IS_NULL(cJSON_item));
  return cJSON_item;
}

static void *cJSON_to_string(cJSON *cJSON_string) {
  string str = malloc(strlen(cJSON_string->valuestring) + 1);
  END_IF(IS_NULL(str));
  strcpy(str , cJSON_string->valuestring);
  return str;
}
