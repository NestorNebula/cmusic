#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "type-handlers.h"
#include "tmem.h"
#include "query.h"
#include "tprint.h"
#include "readers.h"
#include "ptrarray.h"
#include "helpers.h"

#define IS_NULL(ptr) (ptr == NULL)
#define IS_EMPTY(str) (str[0] == '\0')

extern string token;

extern User user;
extern SimplifiedPlaylist *owned_playlists,
                          *followed_playlists;
extern Artist *followed_artists;

/*
 * handle_user_connection:
 * Queries the API to get user's informations.
 * Terminates program if the query fails, else stores
 * user informations in user.
 */
void handle_user_connection(void);

/*
 * search:
 * Prompts the user for a search query and parameters.
 * Queries the API with the query and parameters and displays the results.
 * Offers the possibility to the user to learn more about one of the results.
 */
void search(void);

/*
 * handle_user_playlists:
 * Offers the possibility to update playlists' details, learn more about their
 * tracks and remove them.
 */
void handle_user_playlists(void);

/*
 * handle_followed:
 * Offers the possibility to unfollow followed artists/playlist or to learn
 * more about them.
 */
void handle_followed(void);

/*
 * handle_favorites:
 * Display the user's favorites artists and/or tracks and offers
 * the possibility to learn more about them.
 */
void handle_favorites(Artist *favorite_artists, Track *favorite_tracks);

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

  update_playlists();
  update_followed_artists();

  Page favorite_artists_page = query_get_user_top_artists(0);
  Page favorite_tracks_page = query_get_user_top_tracks(0);

  for (;;) {
    int option = handle_option_choice(4, "Search in catalog", 
                                      "Manage Playlists",
                                      "Manage followed Artists/Playlists",
                                      "Learn about your favorite "
                                      "artists/tracks");

    int owned_playlists_count = 0;
    while (!IS_NULL(owned_playlists[owned_playlists_count]))
      owned_playlists_count++;
    if (option == 0) {
      search();
    } else if (option == 1) {
      if (owned_playlists_count) {
        handle_user_playlists();
      } else {
        print_to_stream("\nNo playlist to manage\n");
      }
    } else if (option == 2) {
      handle_followed();
    } else if (option == 3) {
      handle_favorites((Artist *) favorite_artists_page->items, 
                       (Track *) favorite_tracks_page->items);
    } else break;
  }
    
  free_array(favorite_artists_page->items, free_artist);
  tfree(free_page, favorite_artists_page);
  free_array(favorite_tracks_page->items, free_track);
  tfree(free_page, favorite_tracks_page);
  tfree(free_user, user);
}


void handle_user_connection(void) {
  user = query_get_user();
  if (IS_NULL(user) || IS_NULL(user->display_name)) {
    print_to_stream("\nInvalid token. You can get a valid token by following "
                    "the steps provided in the docs.\n");
    exit(EXIT_FAILURE);
  }
}

void search(void) {
  for (;;) {
    int option = handle_option_choice(4, "Search album", "Search artist", 
                                "Search playlist", "Search track");

    if (option == 0) {
      print_to_stream("Enter album's name: ");
      string name = read_string(stdin);
      if (IS_EMPTY(name)) {
        free(name);
        continue;
      }
      print_to_stream("Enter album's artist (optional): ");
      string artist = read_string(stdin);
      print_to_stream("Enter album's year (optional, "
                      "format: yyyy OR yyyy-yyyy): ");
      string year = read_string(stdin);
      print_to_stream("Only include new albums? (y/n) ");
      bool new = read_bool(stdin);
      print_to_stream("Only include \"hipster\" albums (albums with the lowest "
                      "10%% popularity)? (y/n) ");
      bool hipster = read_bool(stdin);

      size_t offset = 0;
      bool is_last_page = false;
      do {
        Search search = query_get_albums(name, IS_EMPTY(artist) ? NULL : artist,
                                         IS_EMPTY(year) ? NULL : year, new,
                                         hipster, offset);
        Page albums_page = search->albums;
        is_last_page = albums_page->limit + offset >= albums_page->total;
        SimplifiedAlbum *albums = albums_page->items;

        if (IS_NULL(albums[0])) {
          print_to_stream("\nNo album found\n");
          tfree(free_search, search);
          break;
        }
        print_array(albums, print_simplified_album_essentials);
        print_to_stream("Enter album's number%s ",
                        is_last_page
                          ? ":"
                          : " (0 to get other results):");

        bool success = false;
        int choice = read_integer(stdin, &success);
        if (success) {
          if (choice == 0) {
            offset += albums_page->limit;
            tfree(free_search, search);
            continue;
          }
          int albums_count = 0;
          while (!IS_NULL(albums[albums_count])) albums_count++;
          if (choice >= 1 && choice <= albums_count) {
            Album album = query_get_album(albums[choice - 1]->id);
            handle_album(album);
            tfree(free_album, album);
          }
        }
        tfree(free_search, search);
        break;
      } while (!is_last_page);
      free(name);
      free(artist);
      free(year);
    } else if (option == 1) {
      print_to_stream("Enter artist's name: ");
      string name = read_string(stdin);
      if (IS_EMPTY(name)) {
        free(name);
        continue;
      }
      print_to_stream("Enter artist's active year (optional, "
                      "format: yyyy OR yyyy-yyyy): ");
      string year = read_string(stdin);
      print_to_stream("Enter artist's genre (optional): ");
      string genre = read_string(stdin);

      size_t offset = 0;
      bool is_last_page = false;
      do {
        Search search = query_get_artists(name, IS_EMPTY(year) ? NULL : year,
                                          IS_EMPTY(genre) ? NULL : genre,
                                          offset);
        Page artists_page = search->artists;
        is_last_page = artists_page->limit + offset >= artists_page->total;
        Artist *artists = artists_page->items;

        if (IS_NULL(artists[0])) {
          print_to_stream("\nNo artist found\n");
          tfree(free_search, search);
          break;
        }

        print_array(artists, print_artist_essentials);
        print_to_stream("Enter artist's number%s ",
                        is_last_page
                          ? ":"
                          : " (0 to get other results):");
        bool success = false;
        int choice = read_integer(stdin, &success);
        
        if (success) {
          if (choice == 0) {
            offset += artists_page->limit;
            tfree(free_search, search);
            continue;
          }
          int artists_count = 0;
          while (!IS_NULL(artists[artists_count])) artists_count++;
          if (choice >= 1 && choice <= artists_count) {
            handle_artist(artists[choice - 1]);
          }
        }
        tfree(free_search, search);
        break;
      } while (!is_last_page);
      free(name);
      free(year);
      free(genre);
    } else if (option == 2) {
      print_to_stream("Enter playlist's name: ");
      string name = read_string(stdin);
      if (IS_EMPTY(name)) {
        free(name);
        continue;
      }

      size_t offset = 0;
      bool is_last_page = false;
      do {
        Search search = query_get_playlists(name, offset);
        Page playlists_page = search->playlists;
        is_last_page = playlists_page->limit + offset >= playlists_page->total;
        SimplifiedPlaylist *playlists = playlists_page->items;

        if (IS_NULL(playlists[0])) {
          print_to_stream("\nNo playlist found\n");
          tfree(free_search, search);
          break;
        }

        print_array(playlists, print_simplified_playlist_essentials);
        print_to_stream("Enter playlist's number%s ",
                        is_last_page
                          ? ":"
                          : " (0 to get other results):");
        bool success = false;
        int choice = read_integer(stdin, &success);

        if (success) {
          if (choice == 0) {
            offset += playlists_page->limit;
            tfree(free_search, search);
            continue;
          }
          int playlists_count = 0;
          while (!IS_NULL(playlists[playlists_count])) playlists_count++;
          if (choice >= 1 && choice <= playlists_count) {
            Playlist playlist = query_get_playlist(playlists[choice - 1]->id);
            handle_playlist(playlist);
            tfree(free_playlist, playlist);
          }
        }
        tfree(free_search, search);
        break;
      } while (!is_last_page);
      free(name);
    } else if (option == 3) {
      print_to_stream("Enter track's name: ");
      string name = read_string(stdin);
      if (IS_EMPTY(name)) {
        free(name);
        continue;
      }
      print_to_stream("Enter track's artist (optional): ");
      string artist = read_string(stdin);
      print_to_stream("Enter track's year (optional, format: "
                      "yyyy OR yyyy-yyyy): ");
      string year = read_string(stdin);
      print_to_stream("Enter track's album (optional): ");
      string album = read_string(stdin);
      print_to_stream("Enter track's genre (optional): ");
      string genre = read_string(stdin);

      size_t offset = 0;
      bool is_last_page = false;
      do {
        Search search = query_get_tracks(name,  IS_EMPTY(artist) ? NULL : artist,
                                         IS_EMPTY(year) ? NULL : year,
                                         IS_EMPTY(album) ? NULL : album,
                                         IS_EMPTY(genre) ? NULL : genre, offset);
        Page tracks_page = search->tracks;
        is_last_page = tracks_page->limit + offset >= tracks_page->total;
        Track *tracks = tracks_page->items;

        if (IS_NULL(tracks[0])) {
          print_to_stream("\nNo track found\n");
          tfree(free_search, search);
          break;
        }

        print_array(tracks, print_track_essentials);
        print_to_stream("Enter track's number%s ",
                        is_last_page
                          ? ":"
                          : " (0 to get other results):");
        bool success = false;
        int choice = read_integer(stdin, &success);

        if (success) {
          if (choice == 0) {
            offset += tracks_page->limit;
            tfree(free_search, search);
            continue;
          }
          int tracks_count = 0;
          while (!IS_NULL(tracks[tracks_count])) tracks_count++;
          if (choice >= 1 && choice <= tracks_count) {
            handle_track(tracks[choice - 1]);
          }
        }
        tfree(free_search, search);
        break;
      } while (!is_last_page);
      free(name);
      free(artist);
      free(year);
      free(album);
      free(genre);
    } else break;
  }
}

void handle_user_playlists(void) {
  for (;;) {
    SimplifiedPlaylist *playlists = owned_playlists;
    int playlists_count = 0;
    while (!IS_NULL(playlists[playlists_count])) playlists_count++;
    print_array(playlists, print_simplified_playlist_essentials);
    print_to_stream("Enter the number of the playlist you want to manage: ");
    bool success = false;
    int choice = read_integer(stdin, &success);

    if (!success || choice < 1 || choice > playlists_count) break;
    Playlist playlist = query_get_playlist(playlists[choice - 1]->id);

    int option = handle_option_choice(3, "Update playlist's details",
                                      "Remove tracks from playlist",
                                      "Learn more about tracks");

    if (option == 0) {
      print_to_stream("Enter playlist's name: ");
      if (!IS_EMPTY(playlist->name)) {
        print_to_stream("(%s) ", playlist->name);
      }
      string new_name = read_string(stdin);
      if (!IS_EMPTY(new_name)) {
        free(playlist->name);
        playlist->name = new_name;
      } else free(new_name);
      print_to_stream("Enter playlist's description: ");
      if (!IS_EMPTY(playlist->description)) {
        print_to_stream("(%s) ", playlist->description);
      }
      string new_description = read_string(stdin);
      if (!IS_EMPTY(new_description)) {
        free(playlist->description);
        playlist->description = new_description;
      } else free(new_description);
      
      query_put_playlist_details(playlist);
      update_playlists();
      print_to_stream("\nPlaylist updated\n");
    } else if (option == 1 || option == 2) {
      PtrArray ptr_array = new_ptr_array();
      size_t offset = 0;
      int tracks_count = 0;
      for (;;) {
        Page page = !offset
          ? playlist->tracks
          : query_get_playlist_tracks(playlist->id, offset);
        PlaylistTrack *playlist_tracks = page->items;
        for (int i = 0; !IS_NULL(playlist_tracks[i]); i++) {
          add_item(ptr_array, playlist_tracks[i]->track);
          tracks_count++;
        }
        if (offset) {
          free_array((void **) playlist_tracks, free_playlist_track);
          tfree(free_page, page);
        }
        offset = tracks_count;
        if (tracks_count >= page->total) break;
      }
      Track *tracks = (Track *) get_array(ptr_array);

      if (IS_NULL(tracks[0])) {
        print_to_stream("\nNo track in playlist\n");
      } else if (option == 1) {
        PtrArray tracks_to_delete_ptr_array = new_ptr_array();
        for (;;) {
          print_array(tracks, print_track_essentials);
          print_to_stream("Enter track to remove's number: ");
          bool succcess = false;
          int choice = read_integer(stdin, &success);
          if (choice < 1 || choice > tracks_count) break;
          add_item(tracks_to_delete_ptr_array, tracks[choice - 1]);
          print_to_stream("Remove another track ? (y/n) ");
          if (!read_bool(stdin)) break;
        }
        size_t tracks_to_delete_count = get_size(tracks_to_delete_ptr_array);
        if (tracks_to_delete_count) {
          print_to_stream("You are about to remove %zu track%s from \"%s\" "
                          "playlist. Confirm ? (y/n) ", tracks_to_delete_count,
                          tracks_to_delete_count > 1 ? "s" : "", 
                          playlist->name);
          if (read_bool(stdin)) {
            query_delete_playlist_tracks(
              playlist, 
              (Track *) get_array(tracks_to_delete_ptr_array));
            update_playlists();
            print_to_stream("\nTrack%s removed\n", 
                            tracks_to_delete_count > 1
                              ? "s"
                              : "");
          }
        }
        free_ptr_array(tracks_to_delete_ptr_array, false, NULL);
      } else {
        print_array(tracks, print_track_essentials);
        print_to_stream("Enter track's number: ");
        bool success = false;
        int choice = read_integer(stdin, &success);
        if (choice >= 1 && choice <= tracks_count) {
          handle_track(tracks[choice - 1]);
        }
      }

      if (playlist->tracks->total > playlist->tracks->limit) {
        free_array(get_array(ptr_array) + playlist->tracks->limit, free_track);
      }
      free_ptr_array(ptr_array, false, NULL);
    } 

    tfree(free_playlist, playlist);
  }
}

void handle_followed(void) {
  for (;;) {
    SimplifiedPlaylist *playlists = followed_playlists;
    Artist *artists = followed_artists;
    int artists_count = 0;
    while (!IS_NULL(artists[artists_count])) artists_count++;
    int playlists_count = 0;
    while (!IS_NULL(playlists[playlists_count])) playlists_count++;
    int option = handle_option_choice(4, "Unfollow artist", 
                                      "Unfollow playlist",
                                      "Learn more about followed artists",
                                      "Learn more about followed playlists");

    if (option == 0 || option == 2) {
      if (artists_count) {
        print_array(artists, print_artist_essentials);
        print_to_stream("Enter artist's number: ");
        bool success = false;
        int choice = read_integer(stdin, &success);
        if (success && choice >= 1 && choice <= artists_count) {
          if (option  == 0) {
            PtrArray artists_to_unfollow_ptr_array = new_ptr_array();
            add_item(artists_to_unfollow_ptr_array, artists[choice - 1]);
            query_delete_unfollow_artists(
              (Artist *) get_array(artists_to_unfollow_ptr_array));
            update_followed_artists();
            print_to_stream("\nArtist unfollowed\n");
            free_ptr_array(artists_to_unfollow_ptr_array, false, NULL);
          }
          else handle_artist(artists[choice - 1]);
        }
      } else print_to_stream("\nNo followed artist\n");
    } else if (option == 1 || option == 3) {
      if (playlists_count) {
        print_array(playlists, print_simplified_playlist_essentials);
        print_to_stream("Enter playlist's number: ");
        bool success = false;
        int choice = read_integer(stdin, &success);
        if (success && choice >= 1 && choice <= playlists_count) {
          Playlist playlist = query_get_playlist(playlists[choice - 1]->id);
          if (option == 1) {
            query_delete_unfollow_playlist(playlist);
            update_playlists();
            print_to_stream("\nPlaylist Unfollowed\n");
          } else handle_playlist(playlist);
          tfree(free_playlist, playlist);
        }
      } else print_to_stream("\nNo followed playlist\n");
    } else break;
  }
}

void handle_favorites(Artist *favorite_artists, Track *favorite_tracks) {
  int artists_count = 0;
  while (!IS_NULL(favorite_artists[artists_count])) artists_count++;
  int tracks_count = 0;
  while (!IS_NULL(favorite_tracks[tracks_count])) tracks_count++;
  for (;;) {
    int option = handle_option_choice(2, "Learn more about one of your "
                                      "favorite artists",
                                      "Learn more about one of your "
                                      "favorite tracks");

    if (option == 0) {
      if (artists_count) {
        print_array(favorite_artists, print_artist_essentials);
        print_to_stream("Enter artist's number: ");
        bool success = false;
        int choice = read_integer(stdin, &success);

        if (success && choice >= 1 && choice <= artists_count) {
          handle_artist(favorite_artists[choice - 1]);
        }
      } else print_to_stream("\nNo favorite artist\n");
    } else if (option == 1) {
      if (tracks_count) {
        print_array(favorite_tracks, print_track_essentials);
        print_to_stream("Enter tracks's number: ");
        bool success = false;
        int choice = read_integer(stdin, &success);

        if (success && choice >= 1 && choice <= tracks_count) {
          handle_track(favorite_tracks[choice - 1]);
        }
      } else print_to_stream("\nNo favorite track\n");
    } else break;
  }
}
