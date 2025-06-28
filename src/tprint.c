#include <stdio.h>
#include "tprint.h"

#define BILLION 1000000000
#define MILLION 1000000
#define THOUSAND 1000


static void _print_album_details(string name, string release_date,
                                 SimplifiedArtist *artists, 
                                 size_t total_tracks);
static void _print_album_essentials(string name, size_t total_tracks);

static void _print_artist_details(string name, string *genres,
                                  Followers followers);
static void _print_artist_essentials(string name);

static void _print_playlist_details(string name, string description, 
                                    SimplifiedUser owner);
static void _print_playlist_essentials(string name);

static void _print_track_details(string name, size_t duration_ms,
                                 SimplifiedArtist *artists);
static void _print_track_essentials(string name);

static void _print_user_details(string display_name, Followers followers);
static void _print_user_essentials(string display_name);

static void print_followers(Followers followers);

void print_album_details(Album album) {
  _print_album_details(album->name, album->release_date, album->artists,
                       album->total_tracks);
  print_empty_line();
}

void print_album_essentials(Album album) {
  _print_album_essentials(album->name, album->total_tracks);
  print_empty_line();
}

void print_simplified_album_details(SimplifiedAlbum simplified_album) {
  _print_album_details(simplified_album->name, simplified_album->release_date, 
                       simplified_album->artists, 
                       simplified_album->total_tracks);
  print_empty_line();
}

void print_simplified_album_essentials(SimplifiedAlbum simplified_album) {
  _print_album_essentials(simplified_album->name, 
                          simplified_album->total_tracks);
  print_empty_line();
}

void print_artist_details(Artist artist) {
  _print_artist_details(artist->name, artist->genres, artist->followers);
  print_empty_line();
}

void print_artist_essentials(Artist artist) {
  _print_artist_essentials(artist->name);
  print_empty_line();
}

void print_simplified_artist_details(SimplifiedArtist simplified_artist) {
  _print_artist_details(simplified_artist->name, NULL, NULL);
  print_empty_line();
}

void print_simplified_artist_essentials(SimplifiedArtist simplified_artist) {
  _print_artist_essentials(simplified_artist->name);
  print_empty_line();
}

void print_playlist_details(Playlist playlist) {
  _print_playlist_details(playlist->name, playlist->description, 
                          playlist->owner);
  if (playlist->tracks != NULL) {
    print_to_stream("Number of tracks: %zu\n", playlist->tracks->total);
  } 
  print_empty_line();
}

void print_playlist_essentials(Playlist playlist) {
  _print_playlist_essentials(playlist->name);
  if (playlist->tracks != NULL) {
    print_to_stream("(%zu tracks)\n", playlist->tracks->total);
  }
  print_empty_line();
}

void print_simplified_playlist_details(SimplifiedPlaylist 
                                        simplified_playlist) {
  _print_playlist_details(simplified_playlist->name, 
                          simplified_playlist->description,
                          simplified_playlist->owner);
  print_to_stream("Number of tracks: %zu\n",
                  simplified_playlist->tracks.total);
  print_empty_line();
}

void print_simplified_playlist_essentials(SimplifiedPlaylist
                                            simplified_playlist) {
  _print_playlist_essentials(simplified_playlist->name);
  print_to_stream("(%zu tracks)\n", simplified_playlist->tracks.total);
  print_empty_line();
}

void print_track_details(Track track) {
  _print_track_details(track->name, track->duration_ms, track->artists);
  print_empty_line();
}

void print_track_essentials(Track track) {
  _print_track_essentials(track->name);
  print_empty_line();
}

void print_simplified_track_details(SimplifiedTrack simplified_track) {
  _print_track_details(simplified_track->name, simplified_track->duration_ms, 
                       simplified_track->artists);
  print_empty_line();
}
void print_simplified_track_essentials(SimplifiedTrack simplified_track) {
  _print_track_essentials(simplified_track->name);
  print_empty_line();
}

void print_user_details(User user) {
  _print_user_details(user->display_name, user->followers);
  print_empty_line();
}

void print_user_essentials(User user) {
  _print_user_essentials(user->display_name);
  print_empty_line();
}

void print_simplified_user_details(SimplifiedUser simplified_user) {
  _print_user_details(simplified_user->display_name, NULL);
  print_empty_line();
}
void print_simplified_user_essentials(SimplifiedUser simplified_user) {
  _print_user_essentials(simplified_user->display_name);
  print_empty_line();
}

static void _print_album_details(string name, string release_date,
                                 SimplifiedArtist *artists, 
                                 size_t total_tracks) {
  print_to_stream("%s\n", name);
  print_to_stream("Release date: %s\n", release_date);
  if (artists != NULL && artists != NULL) {
    print_to_stream("%s:", artists[1] != NULL ? "Artists" : "Artist");
    for (int i = 0; artists[i] != NULL; i++) {
      print_to_stream(" %s%s", artists[i]->name, 
                      artists[i + 1] != NULL ? "," : "");
    }
    printf("\n");
  }
  print_to_stream("Number of tracks: %zu\n", total_tracks);
}

static void _print_album_essentials(string name, size_t total_tracks) {
  print_to_stream("%s (%zu tracks)\n", name, total_tracks);
}

static void _print_artist_details(string name, string *genres,
                                  Followers followers) {
  print_to_stream("%s\n", name);
  if (genres != NULL && *genres != NULL) {
    print_to_stream("Music genres:");
    for (int i = 0; genres[i] != NULL; i++) {
      print_to_stream(" %s%s", genres[i], genres[i + 1] != NULL ? "," : "");
    }
    print_empty_line();
  }
  print_followers(followers);
}

static void _print_artist_essentials(string name) {
  print_to_stream("%s\n", name);
}

static void _print_playlist_details(string name, string description, 
                                    SimplifiedUser owner) {
  print_to_stream("%s\n", name);
  print_to_stream("%s\n", description);
  if (owner != NULL && owner->display_name != NULL) {
    print_to_stream("Owned by %s\n", owner->display_name);
  }
}

static void _print_playlist_essentials(string name) {
  print_to_stream("%s\n", name);
}

static void _print_track_details(string name, size_t duration_ms,
                                 SimplifiedArtist *artists) {
  print_to_stream("%s\n", name);
  print_to_stream("Duration: %2zu:%.2zu\n",
                  duration_ms / 1000 / 60, duration_ms / 1000 % 60);
  if (artists != NULL && *artists != NULL) {
    print_to_stream("%s:", artists[1] != NULL ? "Artists" : "Artist");
    for (int i = 0; artists[i] != NULL; i++) {
      print_to_stream(" %s%s", artists[i]->name,
                      artists[i + 1] != NULL ? "," : "");
    }
    print_empty_line();
  }
}

static void _print_track_essentials(string name) {
  print_to_stream("%s\n", name);
}

static void _print_user_details(string display_name, Followers followers) {
  print_to_stream("%s\n", display_name);
  print_followers(followers);
}

static void _print_user_essentials(string display_name) {
  print_to_stream("%s\n", display_name);
}

static void print_followers(Followers followers) {
  if (followers != NULL) {
    if (followers->total >= BILLION) {
      print_to_stream("Followers: %.2fBn\n", 
                      followers->total / (double) BILLION);
    } else if (followers->total >= MILLION) {
      print_to_stream("Followers: %.2fM\n",
                      followers->total / (double) MILLION);
    } else if (followers->total >= THOUSAND) {
      print_to_stream("Followers: %.2fk\n",
                      followers->total / (double) THOUSAND);
    } else {
      print_to_stream("Followers: %zu\n", followers->total);
    }
  }
}
