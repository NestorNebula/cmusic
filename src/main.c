#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "type-handlers.h"
#include "tmem.h"
#include "query.h"
#include "tprint.h"
#include "readers.h"

#define IS_NULL(ptr) (ptr == NULL)
#define IS_EMPTY(str) (str[0] == '\0')

extern string token;

User user = NULL;

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

  for (;;) {
    int option = handle_option_choice(1, "Search in catalog");

    switch (option) {
      case 0:
        search();
        break;
      default:
        exit(EXIT_SUCCESS);
    }
  }
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
  int option = handle_option_choice(4, "Search album", "Search artist", 
                              "Search playlist", "Search track");

  if (option == 0) {
    print_to_stream("Enter album's name: ");
    string name = read_string(stdin);
    if (IS_EMPTY(name)) return;
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
    if (IS_EMPTY(name)) return;
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
    if (IS_EMPTY(name)) return;

    size_t offset = 0;
    bool is_last_page = false;
    do {
      Search search = query_get_playlists(name, offset);
      Page playlists_page = search->playlists;
      is_last_page = playlists_page->limit + offset >= playlists_page->total;
      SimplifiedPlaylist *playlists = playlists_page->items;

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
    if (IS_EMPTY(name)) return;
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
  }
}
