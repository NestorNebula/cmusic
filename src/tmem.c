#include <stdlib.h>
#include <stdarg.h>
#include "types.h"
#include "tmem.h"

#define RETURN_IF_NULL(ptr) if ((ptr) == NULL) return ptr
#define RETURN_VOID_IF_NULL(ptr) if ((ptr) == NULL) return
#define IF_NOT_NULL(ptr) if ((ptr) != NULL)

/*
 * free_all:
 * Releases memory taken by all structures pointed by pointers 
 * passed as arguments until it meets a null pointer. 
 * Thus the last argument should be a null pointer.
 * This function shouldn't be called for structures (except if the structure's
 * nested structures/strings were already freed) as it doesn't release 
 * the memory taken by nested structures/strings.
 */
static void free_all(void *first_ptr, ...);

/*
 * free_array:
 * Releases memory taken by a null-terminated array and by its arguments.
 * The first argument should be a pointer to the first element of an array.
 * The second argument should be a function that will release the memory of
 * its argument item. This function will be called for every item in array.
 */
static void free_array(void **array, void (*free_item)(void *item));

void *talloc(void *(new_type)(void)) {
  return (*new_type)();
}

void *new_album(void) {
  Album album = malloc(sizeof(struct album));
  RETURN_IF_NULL(album);
  album->album_type = album->id = album->name = album->release_date = NULL;
  album->restrictions = NULL;
  album->artists = NULL;
  album->tracks = NULL;
  return album;
}

void *new_simplified_album(void) {
  SimplifiedAlbum simplified_album = malloc(sizeof(struct simplified_album));
  RETURN_IF_NULL(simplified_album);
  simplified_album->album_type = simplified_album->href = 
  simplified_album->id = simplified_album->name =
  simplified_album->release_date = NULL;
  simplified_album->restrictions = NULL;
  simplified_album->artists = NULL;
  return simplified_album;
}

void *new_saved_album(void) {
  SavedAlbum saved_album = malloc(sizeof(struct saved_album));
  RETURN_IF_NULL(saved_album);
  saved_album->album = NULL;
  return saved_album;
}

void *new_artist(void) {
  Artist artist = malloc(sizeof(struct artist));
  RETURN_IF_NULL(artist);
  artist->followers = NULL;
  artist->genres = NULL;
  artist->id = artist->name = NULL;
  return artist;
}

void *new_simplified_artist(void) {
  SimplifiedArtist simplified_artist =
    malloc(sizeof(struct simplified_artist));
  RETURN_IF_NULL(simplified_artist);
  simplified_artist->href = simplified_artist->id = simplified_artist->name =
    NULL;
  return simplified_artist;
}

void *new_playlist(void) {
  Playlist playlist = malloc(sizeof(struct playlist));
  RETURN_IF_NULL(playlist);
  playlist->description = playlist->id = playlist->name = 
  playlist->snapshot_id = NULL;
  playlist->tracks = NULL;
  playlist->owner = NULL;
  return playlist;
}

void *new_simplified_playlist(void) {
  SimplifiedPlaylist simplified_playlist = 
    malloc(sizeof(struct simplified_playlist));
  RETURN_IF_NULL(simplified_playlist);
  simplified_playlist->description = simplified_playlist->href =
  simplified_playlist->id = simplified_playlist->name = 
  simplified_playlist->snapshot_id = simplified_playlist->tracks.href = NULL;
  simplified_playlist->owner = NULL;
  return simplified_playlist;
}

void *new_playlist_track(void) {
  PlaylistTrack playlist_track = malloc(sizeof(struct playlist_track));
  RETURN_IF_NULL(playlist_track);
  playlist_track->added_at = playlist_track->added_by.href =
  playlist_track->added_by.id = NULL;
  playlist_track->track = NULL;
  return playlist_track;
}

void *new_track(void) {
  Track track = malloc(sizeof(struct track));
  RETURN_IF_NULL(track);
  track->album = NULL;
  track->artists = NULL;
  track->id = track->name = NULL;
  track->restrictions = NULL;
  return track;
}

void *new_simplified_track(void) {
  SimplifiedTrack simplified_track = malloc(sizeof(struct simplified_track));
  RETURN_IF_NULL(simplified_track);
  simplified_track->artists = NULL;
  simplified_track->href = simplified_track->id = simplified_track->name =
    NULL;
  simplified_track->restrictions = NULL;
  return simplified_track;
}

void *new_saved_track(void) {
  SavedTrack saved_track = malloc(sizeof(struct saved_track));
  RETURN_IF_NULL(saved_track);
  saved_track->added_at = NULL;
  saved_track->track = NULL;
  return saved_track;
}

void *new_user(void) {
  User user = malloc(sizeof(struct user));
  RETURN_IF_NULL(user);
  user->display_name = user->id = NULL;
  user->followers = NULL;
  return user;
}

void *new_simplified_user(void) {
  SimplifiedUser simplified_user = malloc(sizeof(struct simplified_user));
  RETURN_IF_NULL(simplified_user);
  simplified_user->href = simplified_user->id = simplified_user->display_name =
    NULL;
  return simplified_user;
}

void *new_followers(void) {
  return malloc(sizeof(struct followers));
}

void *new_page(void) {
  Page page = malloc(sizeof(struct page));
  RETURN_IF_NULL(page);
  page->href = page->next = page->items = NULL;
  return page;
}

void *new_restrictions(void) {
  Restrictions restrictions = malloc(sizeof(struct restrictions));
  RETURN_IF_NULL(restrictions);
  restrictions->reason = NULL;
  return restrictions;
}

void *new_search(void) {
  Search search = malloc(sizeof(struct search));
  RETURN_IF_NULL(search);
  search->tracks = search->artists = search->albums = search->playlists = NULL;
  return search;
}

void tfree(void (*free_type)(void *type_struct_ptr), void *type_struct_ptr) {
  (*free_type)(type_struct_ptr);
}

void free_album(void *album_ptr) {
  RETURN_VOID_IF_NULL(album_ptr);
  Album album = album_ptr;
  free_restrictions(album->restrictions);
  free_array((void **) album->artists, free_simplified_artist);
  IF_NOT_NULL(album->tracks) {
    free_array(album->tracks->items, free_track);
    free_page(album->tracks);
  }
  free_all(album->album_type, album->id, album->name, 
           album->release_date, album, NULL);
}

void free_simplified_album(void *simplified_album_ptr) {
  RETURN_VOID_IF_NULL(simplified_album_ptr);
  SimplifiedAlbum simplified_album = simplified_album_ptr;
  free_restrictions(simplified_album->restrictions);
  free_array((void **) simplified_album->artists, free_simplified_artist);
  free_all(simplified_album->album_type, simplified_album->href,
           simplified_album->id, simplified_album->name,
           simplified_album->release_date, simplified_album, NULL);
}

void free_saved_album(void *saved_album_ptr) {
  RETURN_VOID_IF_NULL(saved_album_ptr);
  SavedAlbum saved_album = saved_album_ptr;
  free_album(saved_album->album);
  free_all(saved_album->added_at, saved_album, NULL);
}

void free_artist(void *artist_ptr) {
  RETURN_VOID_IF_NULL(artist_ptr);
  Artist artist = artist_ptr;
  free_followers(artist->followers);
  free_array((void **) artist->genres, free);
  free_all(artist->id, artist->name, artist, NULL);
}

void free_simplified_artist(void *simplified_artist_ptr) {
  RETURN_VOID_IF_NULL(simplified_artist_ptr);
  SimplifiedArtist simplified_artist = simplified_artist_ptr;
  free_all(simplified_artist->href, simplified_artist->id,
           simplified_artist->name, simplified_artist, NULL);
}

void free_playlist(void *playlist_ptr) {
  RETURN_VOID_IF_NULL(playlist_ptr);
  Playlist playlist = playlist_ptr;
  free_simplified_user(playlist->owner);
  IF_NOT_NULL(playlist->tracks) {
    free_array(playlist->tracks->items, free_track);
    free_page(playlist->tracks);
  }
  free_all(playlist->description, playlist->id, playlist->name,
           playlist->snapshot_id, playlist, NULL);
}

void free_simplified_playlist(void *simplified_playlist_ptr) {
  RETURN_VOID_IF_NULL(simplified_playlist_ptr);
  SimplifiedPlaylist simplified_playlist = simplified_playlist_ptr;
  free_simplified_user(simplified_playlist->owner);
  free_all(simplified_playlist->description, simplified_playlist->href,
           simplified_playlist->id, simplified_playlist->name,
           simplified_playlist->snapshot_id,
           simplified_playlist->tracks.href, simplified_playlist,
           NULL);
}

void free_playlist_track(void *playlist_track_ptr) {
  RETURN_VOID_IF_NULL(playlist_track_ptr);
  PlaylistTrack playlist_track = playlist_track_ptr;
  free_track(playlist_track->track);
  free_all(playlist_track->added_at, playlist_track->added_by.href,
           playlist_track->added_by.id, playlist_track, NULL);
}

void free_track(void *track_ptr) {
  RETURN_VOID_IF_NULL(track_ptr);
  Track track = track_ptr;
  free_simplified_album(track->album);
  free_array((void **) track->artists, free_simplified_artist);
  free_restrictions(track->restrictions);
  free_all(track->id, track->name, track, NULL);
}

void free_simplified_track(void *simplified_track_ptr) {
  RETURN_VOID_IF_NULL(simplified_track_ptr);
  SimplifiedTrack simplified_track = simplified_track_ptr;
  free_array((void **) simplified_track->artists, free_simplified_artist);
  free_restrictions(simplified_track->restrictions);
  free_all(simplified_track->href, simplified_track->id,
           simplified_track->name, simplified_track, NULL);
}

void free_saved_track(void *saved_track_ptr) {
  RETURN_VOID_IF_NULL(saved_track_ptr);
  SavedTrack saved_track = saved_track_ptr;
  free_track(saved_track->track);
  free_all(saved_track->added_at, saved_track, NULL);
}

void free_user(void *user_ptr) {
  RETURN_VOID_IF_NULL(user_ptr);
  User user = user_ptr;
  free_followers(user->followers);
  free_all(user->display_name, user->id, user, NULL);
}

void free_simplified_user(void *simplified_user_ptr) {
  RETURN_VOID_IF_NULL(simplified_user_ptr);
  SimplifiedUser simplified_user = simplified_user_ptr;
  free_all(simplified_user->href, simplified_user->id,
           simplified_user->display_name, simplified_user, NULL);
}

void free_followers(void *followers_ptr) {
  free(followers_ptr);
}

void free_page(void *page_ptr) {
  RETURN_VOID_IF_NULL(page_ptr);
  Page page = page_ptr;
  free_all(page->href, page->next, page, NULL);
}

void free_restrictions(void *restrictions_ptr) {
  RETURN_VOID_IF_NULL(restrictions_ptr);
  Restrictions restrictions = restrictions_ptr;
  free_all(restrictions->reason, restrictions, NULL);
}

void free_search(void *search_ptr) {
  RETURN_VOID_IF_NULL(search_ptr);
  Search search = search_ptr;
  IF_NOT_NULL(search->tracks) {
    free_array(search->tracks->items, free_track);
    free_page(search->tracks);
  }
  IF_NOT_NULL(search->artists) {
    free_array(search->artists->items, free_artist);
    free_page(search->artists);
  }
  IF_NOT_NULL(search->albums) {
    free_array(search->albums->items, free_album);
    free_page(search->albums);
  }
  IF_NOT_NULL(search->playlists) {
    free_array(search->playlists->items, free_playlist);
    free_page(search->playlists);
  }
  free(search);
}


static void free_all(void *first_ptr, ...) {
  free(first_ptr);

  va_list ap;
  va_start(ap, first_ptr);
  for (void *ptr = va_arg(ap, void *); ptr != NULL; ptr = va_arg(ap, void *)) {
    free(ptr);
  }
  va_end(ap);
}

static void free_array(void **array, void (*free_item)(void *item)) {
  if (array != NULL) {
    for (int i = 0; array[i] != NULL; i++) {
      (*free_item)(array[i]);
    }
    free(array);
  }
}
