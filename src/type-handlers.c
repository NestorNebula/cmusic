#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "query.h"
#include "ptrarray.h"
#include "tmem.h"
#include "tprint.h"
#include "readers.h"
#include "helpers.h"
#include "type-handlers.h"

#define IS_NULL(ptr) (ptr == NULL)

extern SimplifiedPlaylist *owned_playlists,
                          *followed_playlists;
extern Artist *followed_artists;

void handle_album(Album album) {
  if (IS_NULL(album)) return;
  if (!IS_NULL(album->restrictions)) {
    print_to_stream("\nThe album is restricted for the following reason: %s\n",
                    album->restrictions->reason);
    print_to_stream("Some options might be unavailable due to the restriction."
                    "Do you still want to access to the album ? (y/n) ");
    bool answer = read_bool(stdin);
    if (!answer) return;
  }

  print_album_details(album);

  int option = handle_option_choice(3, "Save album in library",
                             "Learn more about one of the artists",
                             "Learn more about one of the tracks");
  if (option == 0) {
    PtrArray ptr_array = new_ptr_array();
    add_item(ptr_array, album);

    query_put_user_saved_albums((Album *) get_array(ptr_array));
    print_to_stream("\nAlbum saved in library\n");

    free_ptr_array(ptr_array, false, NULL);
  } else if (option == 1) {
    print_array(album->artists, print_simplified_artist_essentials);
    print_to_stream("Enter artist's number: ");
    bool success = false;
    int choice = read_integer(stdin, &success);
    if (!success) return;
    int len = 0;
    while (!IS_NULL(album->artists[len])) len++;
    if (choice < 1 || choice > len) return;

    Artist artist = query_get_artist(album->artists[choice - 1]->id);
    handle_artist(artist);
    tfree(free_artist, artist);
  } else if (option == 2) {
    size_t offset = 0;
    bool is_last_page = false;
    do {
      Page page = !offset
        ? album->tracks
        : query_get_album_tracks(album->id, offset);
      is_last_page = page->limit + offset >= page->total;
      SimplifiedTrack *simplified_tracks = page->items; 

      print_array(simplified_tracks,
                  print_simplified_track_essentials);
      print_to_stream("Enter track's number%s ",
                      is_last_page
                        ? ":"
                        : " (0 to get album's other tracks):");
      bool success = false;
      int choice = read_integer(stdin, &success);
      if (success) {
        if (choice == 0) {
          if (offset) {
            free_array((void **) simplified_tracks, free_simplified_track);
            offset += page->limit;
            tfree(free_page, page);
          } else offset += page->limit;
          continue;
        }
        int tracks_count = 0;
        while (!IS_NULL(simplified_tracks[tracks_count])) tracks_count++;
        if (choice >= 1 && choice <= tracks_count) {
          SimplifiedTrack simplified_track = simplified_tracks[choice - 1];
          Track track = query_get_track(simplified_track->id);
          handle_track(track);
          tfree(free_track, track);
        }
      }
      if (offset) {
        free_array((void **) simplified_tracks, free_simplified_track);
        tfree(free_page, page);
      }
      break;
    } while (!is_last_page);
  }
}

void handle_artist(Artist artist) {
  if (IS_NULL(artist)) return;
  print_artist_details(artist);

  int option = 
    handle_option_choice(3, "Follow Artist", 
                         "Learn more about one of their albums",
                         "Learn more about their top tracks");

  if (option == 0) {
    PtrArray ptr_array = new_ptr_array();
    add_item(ptr_array, artist);

    query_put_follow_artists((Artist *) get_array(ptr_array));
    update_followed_artists();

    free_ptr_array(ptr_array, false, NULL);
  } else if (option == 1) {
    bool is_last_page = false;
    size_t offset = 0;
    do {
      Page page = query_get_artist_albums(artist->id, offset);
      SimplifiedAlbum *simplified_albums = page->items;
      print_array(simplified_albums, print_simplified_album_essentials);
      is_last_page = page->limit + offset >= page->total;
      print_to_stream("Enter albums's number%s ",
                      is_last_page 
                        ? ":" 
                        : " (0 to get artist's other albums):");
      bool success = false; 
      int choice = read_integer(stdin, &success);
      if (success) {
        if (choice == 0) {
          offset += page->limit;
          tfree(free_page, page);
          continue;
        }
        int count_albums = 0;
        while (!IS_NULL(simplified_albums[count_albums])) count_albums++;
        if (choice >= 1 && choice <= count_albums) {
          Album album = query_get_album(simplified_albums[choice - 1]->id);
          handle_album(album);
          tfree(free_album, album);
        } 
      }
      tfree(free_page, page);
      break;
    } while (!is_last_page);
  } else if (option == 2) {
    Track *tracks = query_get_artist_top_tracks(artist->id);
    print_array(tracks, print_track_essentials);
    print_to_stream("Enter track's number: ");
    bool success = false;
    int choice = read_integer(stdin, &success);
    if (success) {
      int count_tracks = 0;
      while (!IS_NULL(tracks[count_tracks])) count_tracks++;
      if (choice >= 1 && choice <= count_tracks) {
        handle_track(tracks[choice - 1]);
      }
    }
    free_array((void **) tracks, free_track);
  }
}


void handle_playlist(Playlist playlist) {
  if (IS_NULL(playlist)) return;
  print_playlist_details(playlist);

  int option = handle_option_choice(2, "Follow Playlist",
                                    "Learn more about one of the tracks");

  if (option == 0) {
    query_put_follow_playlist(playlist);
    update_playlists();
    print_to_stream("\nPlaylist Followed\n");
  } else if (option == 1) {
    bool is_last_page = false;
    size_t offset = 0;
    do {
      Page page = !offset 
        ? playlist->tracks 
        : query_get_playlist_tracks(playlist->id, offset);
      is_last_page = page->limit + offset >= page->total;
      PtrArray ptr_array = new_ptr_array();
      PlaylistTrack *items = page->items;
      for (int i = 0; !IS_NULL(items[i]); i++) {
        PlaylistTrack playlist_track = items[i];
        add_item(ptr_array, playlist_track->track);
      }
      Track *tracks = (Track *) get_array(ptr_array);
      print_array(tracks, print_track_essentials);
      free_ptr_array(ptr_array, false, NULL);
      if (IS_NULL(tracks[0])) {
        print_to_stream("\nNo track in playlist\n");
        tfree(free_page, page);
        break;
      }
      print_to_stream("Enter track's number%s ",
                      is_last_page 
                        ? ":" 
                        : " (0 to get playlist's other tracks): ");
      bool success = false;
      int choice = read_integer(stdin, &success);
      if (success) {
        if (choice == 0) {
          if (offset) {
            free_array((void **) items, free_playlist_track);
            offset += page->limit;
            tfree(free_page, page);
          } else offset += page->limit;
          continue;
        }
        int count_tracks = 0;
        while (!IS_NULL(items[count_tracks])) count_tracks++;
        if (choice >= 1 && choice <= count_tracks) {
          PlaylistTrack playlist_track = items[choice - 1];
          handle_track(playlist_track->track);
        }
      }
      if (offset) {
        free_array((void **) items, free_playlist_track);
        tfree(free_page, page);
      }
      break;
    } while (!is_last_page);
  }
}


void handle_track(Track track) {
  if (IS_NULL(track)) return;
  if (!IS_NULL(track->restrictions)) {
    print_to_stream("\nThe track is restricted for the following reason: %s\n",
                    track->restrictions->reason);
    print_to_stream("Some options might be unavailable due to the restriction."
                    "Do you still want to access to the track? (y/n) ");
    bool answer = read_bool(stdin);
    if (!answer) return;
  }

  print_track_details(track);

  int option = handle_option_choice(3, "Add track to a playlist",
                                    "Learn more about track's album",
                                    "Learn more about one of the artists");

  if (option == 0) {
    print_array(owned_playlists, print_simplified_playlist_essentials);
    print_to_stream("Enter playlist's number: ");
    bool success;
    int choice = read_integer(stdin, &success);
    if (success) {
      int count_playlists = 0;
      while (owned_playlists[count_playlists] != NULL) count_playlists++;
      if (choice >= 1 && choice <= count_playlists) {
        Playlist playlist = 
          query_get_playlist(owned_playlists[choice - 1]->id);
        PtrArray ptr_array = new_ptr_array();
        add_item(ptr_array, track);
        query_post_playlist_tracks(playlist, (Track *) get_array(ptr_array));
        print_to_stream("\nTrack added to playlist\n");
        update_playlists();

        tfree(free_playlist, playlist);
        free_ptr_array(ptr_array, false, NULL);
      }
    }
  } else if (option == 1) {
    Album album = query_get_album(track->album->id);
    handle_album(album);
    tfree(free_album, album);
  } else if (option == 2) {
    print_array(track->artists, print_simplified_artist_essentials);
    
    print_to_stream("Enter artist's number: ");
    bool success = false;
    int choice = read_integer(stdin, &success);
    
    if (!success) return;
    int count_artists = 0;
    while (!IS_NULL(track->artists[count_artists])) count_artists++;
    if (choice >= 1 && choice <= count_artists) {
      Artist artist = query_get_artist(track->artists[choice - 1]->id);
      handle_artist(artist);
      tfree(free_artist, artist);
    }
  }
}
