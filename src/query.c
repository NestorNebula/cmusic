#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <cjson/cJSON.h>
#include "tmem.h"
#include "fetch.h"
#include "cjson-converters.h"
#include "query.h"

#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

#define BASE_URL "https://api.spotify.com/v1"

#define create_uri(type, id) create_string("spotify:%s:%s", type, id)

#define cJSON_HasError(cJSON) \
  cJSON_IsObject(cJSON_GetObjectItemCaseSensitive(cJSON, "error"))

/*
 * extend_string:
 * Equivalent to str = create_string(format, ...) but freeing str after 
 * the allocation of the new string (possibly using str as a value
 * of the format string). str is then set to the new string.
 */
#define extend_string(str, format, ...) \
 string _new_extended_string = create_string(format, __VA_ARGS__); \
 free(str); \
 str = _new_extended_string;

/*
 * create_string:
 * Creates a string using format as the format and the other arguments
 * are used as the format values.
 * If the number of parameters doesn't correspond to the number of values 
 * expected by format, the behavior of the function is undefined.
 * Returns a pointer to the first character of the string if no error occurred,
 * else terminates progam.
 * The returned string is allocated dynamically, thus it needs to be
 * freed at some point to avoid a memory leak.
 */
static string create_string(string format, ...);


Album query_get_album(string id) {
  string url = create_string("%s/albums/%s", BASE_URL, id);
  cJSON *cJSON_album = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_album)) {
    cJSON_Delete(cJSON_album);
    return NULL;
  }

  Album album = cJSON_to_album(cJSON_album);
  cJSON_Delete(cJSON_album);

  return album;
}

Page query_get_album_tracks(string id, size_t offset) {
  string url = create_string("%s/albums/%s/tracks?limit=%u&offset=%u",
                             BASE_URL, id, LIMIT, offset);
  cJSON *cJSON_album_tracks = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_album_tracks)) {
    cJSON_Delete(cJSON_album_tracks);
    return NULL;
  }

  Page album_tracks = cJSON_to_page(cJSON_album_tracks,
                                    cJSON_to_simplified_track);
  cJSON_Delete(cJSON_album_tracks);

  return album_tracks;
}

Page query_get_user_saved_albums(size_t offset) {
  string url = create_string("%s/me/albums?limit=%u&offset=%u" BASE_URL,
                             LIMIT, offset);
  cJSON *cJSON_saved_albums = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_saved_albums)) {
    cJSON_Delete(cJSON_saved_albums);
    return NULL;
  }

  Page saved_albums = cJSON_to_page(cJSON_saved_albums, cJSON_to_saved_album);
  cJSON_Delete(cJSON_saved_albums);

  return saved_albums;
}

void query_put_user_saved_albums(Album *albums) {
  if (IS_NULL(*albums)) return;
  string url = create_string("%s/me/albums?ids=", BASE_URL);
  for (int i = 0; !IS_NULL(albums[i]); i++) {
    extend_string(url, "%s%s%s", url, albums[i]->id, 
                  !IS_NULL(albums[i + 1]) ? "," : "");
  }
  cJSON *cJSON_res = fetch(url, PUT, "{}");
  free(url);
  if (!IS_NULL(cJSON_res)) cJSON_Delete(cJSON_res);
}

void query_delete_user_saved_albums(Album *albums) {
  if (IS_NULL(*albums)) return;
  string url = create_string("%s/me/albums?ids=" BASE_URL);
  for (int i = 0; !IS_NULL(albums[i]); i++) {
    extend_string(url, "%s%s%s", url, albums[i]->id,
                  !IS_NULL(albums[i + 1]) ? "," : "");
  }
  cJSON *cJSON_res = fetch(url, DELETE, NULL);
  free(url);
  cJSON_Delete(cJSON_res);
}

Page query_get_new_albums(size_t offset) {
  string url = create_string("%s/browse/new-releases?limit=%u&offset=%u",
                             BASE_URL, LIMIT, offset);
  cJSON *cJSON_new_albums = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_new_albums)) {
    cJSON_Delete(cJSON_new_albums);
    return NULL;
  }

  Page new_albums = cJSON_to_page(cJSON_new_albums, cJSON_to_simplified_album);
  cJSON_Delete(cJSON_new_albums);

  return new_albums;
}

Artist query_get_artist(string id) {
  string url = create_string("%s/artists/%s", BASE_URL, id);
  cJSON *cJSON_artist = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_artist)) {
    cJSON_Delete(cJSON_artist);
    return NULL;
  }

  Artist artist = cJSON_to_artist(cJSON_artist);
  cJSON_Delete(cJSON_artist);
  
  return artist;
}

Page query_get_artist_albums(string id, size_t offset) {
  string url = create_string("%s/artists/%s/albums?limit=%u&offset=%u", 
                             BASE_URL, id, LIMIT, offset);
  cJSON *cJSON_artist_albums = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_artist_albums)) {
    cJSON_Delete(cJSON_artist_albums);
    return NULL;
  }

  Page artist_albums = cJSON_to_page(cJSON_artist_albums,
                                     cJSON_to_simplified_album);
  cJSON_Delete(cJSON_artist_albums);

  return artist_albums;
}

Track *query_get_artist_top_tracks(string id) {
  string url = create_string("%s/artists/%s/top-tracks", BASE_URL, id);
  cJSON *cJSON_res = fetch(url, GET, NULL);
  free(url);
  cJSON *cJSON_artist_top_tracks = 
    cJSON_GetObjectItemCaseSensitive(cJSON_res, "tracks");
  if (cJSON_HasError(cJSON_res) || IS_NULL(cJSON_artist_top_tracks)) {
    cJSON_Delete(cJSON_res);
    return NULL;
  }

  Track *artist_top_tracks = (Track *) cJSON_to_array(cJSON_artist_top_tracks,
                                                      cJSON_to_track);
  cJSON_Delete(cJSON_artist_top_tracks);

  return artist_top_tracks;
}

Playlist query_get_playlist(string id) {
  string url = create_string("%s/playlists/%s", BASE_URL, id);
  cJSON *cJSON_playlist = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_playlist)) {
    cJSON_Delete(cJSON_playlist);
    return NULL;
  }

  Playlist playlist = cJSON_to_playlist(cJSON_playlist);
  cJSON_Delete(cJSON_playlist);
  
  return playlist;
}

void query_put_playlist_details(Playlist playlist) {
  string url = create_string("%s/playlists/%s", BASE_URL, playlist->id);
  string body = create_string(
"{\
  \"name\": \"%s\",\
  \"description\": \"%s\"\
 }", playlist->name, playlist->description);
  cJSON *cJSON_res = fetch(url, PUT, body);

  free(url);
  free(body);
  cJSON_Delete(cJSON_res);
}

Page query_get_playlist_tracks(string id, size_t offset) {
  string url = create_string("%s/playlists/%s/tracks?limit=%u&offset=%u",
                             BASE_URL, id, LIMIT, offset);
  cJSON *cJSON_playlist_tracks = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_playlist_tracks)) {
    cJSON_Delete(cJSON_playlist_tracks);
    return NULL;
  }

  Page playlist_tracks = cJSON_to_page(cJSON_playlist_tracks,
                                       cJSON_to_playlist_track);
  cJSON_Delete(cJSON_playlist_tracks);

  return playlist_tracks;
}

void query_post_playlist_tracks(Playlist playlist, Track *tracks) {
  if(IS_NULL(*tracks)) return;
  string url = create_string("%s/playlists/%s/tracks?uris=", BASE_URL, 
                             playlist->id);
  for (int i = 0; !IS_NULL(tracks[i]); i++) {
    string uri = create_uri("track", tracks[i]->id);
    extend_string(url, "%s%s%s", url, uri, !IS_NULL(tracks[i + 1]) ? "," : "");
    free(uri);
  }
  cJSON *cJSON_res = fetch(url, POST, "{}");
  free(url);

  cJSON *cJSON_snapshot_id = 
    cJSON_GetObjectItemCaseSensitive(cJSON_res, "snapshot_id");
  if (cJSON_IsString(cJSON_snapshot_id)) {
    free(playlist->snapshot_id);
    playlist->snapshot_id = 
      create_string("%s", cJSON_snapshot_id->valuestring);
  }
  cJSON_Delete(cJSON_res);
}

void query_delete_playlist_tracks(Playlist playlist, Track *tracks) {
  if(IS_NULL(*tracks)) return;
  string url = create_string("%s/playlists/%s/tracks", BASE_URL, playlist->id);
  string body = create_string("");
  for (int i = 0; !IS_NULL(tracks[i]); i++) {
    string uri = create_uri("track", tracks[i]->id);
    extend_string(body, "%s\
 {\
  \"uri\": \"%s\"\
 }%s", body, uri, !IS_NULL(tracks[i + 1]) ? "," : "");
    free(uri);
  }

  extend_string(body,
"{\
  \"tracks\": [\
    %s\
  ],\
  \"snapshot_id\": \"%s\"\
 }", body, playlist->snapshot_id);
  cJSON *cJSON_res = fetch(url, DELETE, body);
  free(url);
  free(body);

  cJSON *cJSON_snapshot_id = 
    cJSON_GetObjectItemCaseSensitive(cJSON_res, "snapshot_id");
  if (cJSON_IsString(cJSON_snapshot_id)) {
    free(playlist->snapshot_id);
    playlist->snapshot_id = 
      create_string("%s", cJSON_snapshot_id->valuestring);
  }
  cJSON_Delete(cJSON_res);
}
  
Page query_get_user_playlists(size_t offset) {
  string url = create_string("%s/me/playlists", BASE_URL);
  cJSON *cJSON_user_playlists = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_user_playlists)) {
    cJSON_Delete(cJSON_user_playlists);
    return NULL;
  }

  Page user_playlists = cJSON_to_page(cJSON_user_playlists, 
                                      cJSON_to_simplified_playlist);
  cJSON_Delete(cJSON_user_playlists);

  return user_playlists;
}

Playlist query_post_playlist(User user, Playlist playlist) {
  string url = create_string("%s/users/%s/playlists", BASE_URL, user->id);
  string body = create_string(
"{\
  \"name\": \"%s\",\
  \"description\": \"%s\"\
 }", playlist->name, playlist->description);
  cJSON *cJSON_playlist = fetch(url, POST, body);
  free(url);
  free(body);
  if (cJSON_HasError(cJSON_playlist)) {
    cJSON_Delete(cJSON_playlist);
    return NULL;
  }

  Playlist new_playlist = cJSON_to_playlist(cJSON_playlist);
  cJSON_Delete(cJSON_playlist);
  
  return new_playlist;
}

Search query_get_all(string search_query, string album, string artist, 
                     string playlist, string track, string year, string genre,
                     bool new, bool hipster, size_t offset) {
  string url = create_string("%s/search?q=%s", BASE_URL, search_query);
  if (!IS_NULL(album)) {
    extend_string(url, "%s%%20album:%s", url, album);
  }
  if (!IS_NULL(artist)) {
    extend_string(url, "%s%%20artist:%s", url, artist);
  }
  if (!IS_NULL(playlist)) {
    extend_string(url, "%s%%20playlist:%s", url, playlist);
  }
  if (!IS_NULL(track)) {
    extend_string(url, "%s%%20track:%s", url, track);
  }
  if (!IS_NULL(year)) {
    extend_string(url, "%s%%20year:%s", url, year);
  }
  if (!IS_NULL(genre)) {
    extend_string(url, "%s%%20genre:%s", url, genre);
  }
  if (new) {
    extend_string(url, "%s%%20tag:new", url);
  }
  if (hipster) {
    extend_string(url, "%s%%20tag:hipster", url);
  }
  extend_string(url, "%s&limit=%u&offset=%u&type=album,artist,playlist,track",
                LIMIT, offset);
  cJSON *cJSON_search = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_search)) {
    cJSON_Delete(cJSON_search);
    return NULL;
  }

  Search search = cJSON_to_search(cJSON_search);
  cJSON_Delete(cJSON_search);

  return search;
}

Search query_get_albums(string album, string artist, string year,
                        bool new, bool hipster, size_t offset) {
  string url = create_string("%s/search?q=%s", BASE_URL, album);
  if (!IS_NULL(artist)) {
    extend_string(url, "%s%%20artist:%s", url, artist);
  }
  if (!IS_NULL(year)) {
    extend_string(url, "%s%%20year:%s", url, year);
  }
  if (new) {
    extend_string(url, "%s%%20tag:new", url);
  }
  if (hipster) {
    extend_string(url, "%s%%20tag:hipster", url);
  }
  extend_string(url, "%s&limit=%u&offset=%u&type=album", url, LIMIT, offset);
  cJSON *cJSON_search = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_search)) {
    cJSON_Delete(cJSON_search);
    return NULL;
  }

  Search search = cJSON_to_search(cJSON_search);
  cJSON_Delete(cJSON_search);

  return search;
}

Search query_get_artists(string artist, string year, string genre, 
                         size_t offset) {
  string url = create_string("%s/search?q=%s", BASE_URL, artist);
  if (!IS_NULL(year)) {
    extend_string(url, "%s%%20year:%s", url, year);
  }
  if (!IS_NULL(genre)) {
    extend_string(url, "%s%%20genre:%s", url, genre);
  }
  extend_string(url, "%s&limit=%u&offset=%u&type=artist", url, LIMIT,
                offset);
  cJSON *cJSON_search = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_search)) {
    cJSON_Delete(cJSON_search);
    return NULL;
  }

  Search search = cJSON_to_search(cJSON_search);
  cJSON_Delete(cJSON_search);

  return search;
}

Search query_get_playlists(string playlist, size_t offset) {
  string url =
    create_string("%s/search?q=%s&limit=%u&offset=%u&type=playlist", 
               BASE_URL, playlist, LIMIT, offset);
  cJSON *cJSON_search = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_search)) {
    cJSON_Delete(cJSON_search);
    return NULL;
  }

  Search search = cJSON_to_search(cJSON_search);
  cJSON_Delete(cJSON_search);

  return search;
}

Search query_get_tracks(string track, string artist, string year,
                        string album, string genre, size_t offset) {
  string url = create_string("%s/search?q=%s", BASE_URL, track);
  if (!IS_NULL(artist)) {
    extend_string(url, "%s%%20artist:%s", url, artist);
  }
  if (!IS_NULL(year)) {
    extend_string(url, "%s%%20year:%s", url, year);
  }
  if (!IS_NULL(album)) {
    extend_string(url, "%s%%20album:%s", url, album);
  }
  if (!IS_NULL(genre)) {
    extend_string(url, "%s%%20genre:%s", url, genre);
  }
  extend_string(url, "%s&limit=%u&offset=%u&type=track", url, LIMIT, offset);
  cJSON *cJSON_search = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_search)) {
    cJSON_Delete(cJSON_search);
    return NULL;
  }

  Search search = cJSON_to_search(cJSON_search);
  cJSON_Delete(cJSON_search);

  return search;
}

Track query_get_track(string id) {
  string url = create_string("%s/tracks/%s", BASE_URL, id);
  cJSON *cJSON_track = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_track)) {
    cJSON_Delete(cJSON_track);
    return NULL;
  }

  Track track = cJSON_to_track(cJSON_track);
  cJSON_Delete(cJSON_track);
  
  return track;
}

Page query_get_user_saved_tracks(size_t offset) {
  string url = create_string("%s/me/tracks?limit=%u&offset=%u", BASE_URL,
                             LIMIT, offset);
  cJSON *cJSON_saved_tracks = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_saved_tracks)) {
    cJSON_Delete(cJSON_saved_tracks);
    return NULL;
  }

  Page saved_tracks = cJSON_to_page(cJSON_saved_tracks, cJSON_to_saved_track);
  cJSON_Delete(cJSON_saved_tracks);
  
  return saved_tracks;
}


void query_put_user_saved_tracks(Track *tracks) {
  if (IS_NULL(*tracks)) return;
  string url = create_string("%s/me/tracks?ids=", BASE_URL);
  for (int i = 0; !IS_NULL(tracks[i]); i++) {
    extend_string(url, "%s%s%s", url, tracks[i]->id,
                  !IS_NULL(tracks[i + 1]) ? "," : "");
  }
  cJSON *cJSON_res = fetch(url, PUT, "{}");

  free(url);
  cJSON_Delete(cJSON_res);
}

void query_delete_user_saved_tracks(Track *tracks) {
  if (IS_NULL(*tracks)) return;
  string url = create_string("%s/me/tracks?ids=", BASE_URL);
  for (int i = 0; !IS_NULL(tracks[i]); i++) {
    extend_string(url, "%s%s%s", url, tracks[i]->id,
                  !IS_NULL(tracks[i + 1]) ? "," : "");
  }
  cJSON *cJSON_res = fetch(url, DELETE, NULL);

  free(url);
  cJSON_Delete(cJSON_res);
}

User query_get_user(void) {
  string url = create_string("%s/me", BASE_URL);
  cJSON *cJSON_user = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_user)) {
    cJSON_Delete(cJSON_user);
    return NULL;
  }

  User user = cJSON_to_user(cJSON_user);
  cJSON_Delete(cJSON_user);

  return user;
}

Page query_get_user_top_artists(size_t offset) {
  string url = create_string("%s/me/top/artists", BASE_URL);
  cJSON *cJSON_top_artists = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_top_artists)) {
    cJSON_Delete(cJSON_top_artists);
    return NULL;
  }

  Page top_artists = cJSON_to_page(cJSON_top_artists, cJSON_to_artist);
  cJSON_Delete(cJSON_top_artists);
  
  return top_artists;
}

Page query_get_user_top_tracks(size_t offset) {
  string url = create_string("%s/me/top/tracks", BASE_URL);
  cJSON *cJSON_top_tracks = fetch(url, GET, NULL);
  free(url);
  if (cJSON_HasError(cJSON_top_tracks)) {
    cJSON_Delete(cJSON_top_tracks);
    return NULL;
  }

  Page top_tracks = cJSON_to_page(cJSON_top_tracks, cJSON_to_track);
  cJSON_Delete(cJSON_top_tracks);

  return top_tracks;
}

void query_put_follow_playlist(Playlist playlist) {
  string url = create_string("%s/playlists/%s/followers", BASE_URL, 
                             playlist->id);
  cJSON *cJSON_res = fetch(url, PUT, "{}");

  free(url);
  cJSON_Delete(cJSON_res);
}

void query_delete_unfollow_playlist(Playlist playlist) {
  string url = create_string("%s/playlists/%s/followers", BASE_URL,
                             playlist->id);
  cJSON *cJSON_res = fetch(url, DELETE, NULL);

  free(url);
  cJSON_Delete(cJSON_res);
}

Page query_get_followed_artists(string after) {
  string url = create_string("%s/me/following?type=artist", BASE_URL);
  if (!IS_NULL(after)) {
    extend_string(url, "%s&after=%s", url, after);
  }
  extend_string(url, "%s&limit=%u", url, LIMIT);
  cJSON *cJSON_followed_artists = fetch(url, GET, NULL);
  cJSON *cJSON_followed_artists_page =
    cJSON_GetObjectItemCaseSensitive(cJSON_followed_artists, "artists");
  free(url);
  if (cJSON_HasError(cJSON_followed_artists)) {
    cJSON_Delete(cJSON_followed_artists);
    return NULL;
  }

  Page followed_artists = cJSON_to_page(cJSON_followed_artists_page,
                                        cJSON_to_artist);
  cJSON_Delete(cJSON_followed_artists);

  return followed_artists;
}

void query_put_follow_artists(Artist *artists) {
  if (IS_NULL(*artists)) return;
  string url = create_string("%s/me/following?type=artist&ids=", BASE_URL);
  for (int i = 0; !IS_NULL(artists[i]); i++) {
    extend_string(url, "%s%s%s", url, artists[i]->id,
                  !IS_NULL(artists[i + 1]) ? "," : "");
  }
  cJSON *cJSON_res = fetch(url, PUT, "{}");

  free(url);
  cJSON_Delete(cJSON_res);
}

void query_delete_unfollow_artists(Artist *artists) {
  if (IS_NULL(*artists)) return;
  string url = create_string("%s/me/following?type=artist&ids=", BASE_URL);
  for (int i = 0; !IS_NULL(artists[i]); i++) {
    extend_string(url, "%s%s%s", url, artists[i]->id,
                  !IS_NULL(artists[i + 1]) ? "," : "");
  }
  cJSON *cJSON_res = fetch(url, DELETE, NULL);

  free(url);
  cJSON_Delete(cJSON_res);
}

static string create_string(string format, ...) {
  size_t str_size = 1;
  string str = malloc(str_size);
  END_IF(IS_NULL(str));
  va_list ap;
  va_start(ap, format);
  size_t str_len = vsnprintf(str, str_size, format, ap);
  va_end(ap);

  while (str_len > str_size - 1) {
    str_size = str_len + 1;
    str = realloc(str, str_size);
    END_IF(IS_NULL(str));
    va_start(ap, format);
    str_len = vsnprintf(str, str_size, format, ap);
    va_end(ap);
  }

  return str;
}
