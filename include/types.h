#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdbool.h>

typedef char *string;

// API Types

typedef struct album *Album;
typedef struct simplified_album *SimplifiedAlbum;
typedef struct saved_album *SavedAlbum;

typedef struct artist *Artist;
typedef struct simplified_artist *SimplifiedArtist;

typedef struct playlist *Playlist;
typedef struct simplified_playlist *SimplifiedPlaylist;
typedef struct playlist_track *PlaylistTrack;

typedef struct track *Track;
typedef struct simplified_track *SimplifiedTrack;
typedef struct saved_track *SavedTrack;

typedef struct user *User;
typedef struct simplified_user *SimplifiedUser;

typedef struct followers {
  size_t total;
} *Followers;

typedef struct page {
  string href;
  size_t limit;
  string next;
  size_t total;
  void *items;
} *Page;

typedef struct restrictions {
  string reason;
} *Restrictions;

typedef struct search {
  Page tracks;
  Page artists;
  Page albums;
  Page playlists;
} *Search;


// Albums 

struct album {
  string album_type;
  size_t total_tracks;
  string id;
  string name;
  string release_date;
  struct restrictions restrictions;
  SimplifiedArtist *artists;
  struct page tracks; 
  size_t popularity;
};

struct simplified_album {
  string album_type;
  size_t total_tracks;
  string href;
  string id;
  string name;
  string release_date;
  struct restrictions restrictions;
  SimplifiedArtist *artists;
};

struct saved_album {
  string added_at;
  Album album;
};


// Artists 

struct artist {
  struct followers followers;
  string *genres;
  string id;
  string name;
  size_t popularity;
};

struct simplified_artist {
  string href;
  string id;
  string name;
};


// Playlists

struct playlist {
  string description;
  string id;
  string name;
  SimplifiedUser owner; 
  bool public;
  string snapshot_id;
  struct page tracks;
};

struct simplified_playlist {
  string description;
  string href;
  string id;
  string name;
  SimplifiedUser owner;
  bool public;
  string snapshot_id;
  struct {
    string href;
    size_t total;
  } tracks;
};

struct playlist_track {
  string added_at;
  struct {
    string href;
    string id;
  } added_by;
  Track track;
};


// Tracks

struct track {
  SimplifiedAlbum album;
  SimplifiedArtist *artists;
  size_t duration_ms;
  string id;
  struct restrictions restrictions;
  string name;
  size_t popularity;
};

struct simplified_track {
  SimplifiedArtist *artists;
  size_t duration_ms;
  string href;
  string id;
  struct restrictions restrictions;
  string name;
};

struct saved_track {
  string added_at;
  Track track;
};
  

// Users

struct user {
  string display_name;
  struct followers followers;
  string id;
};

struct simplified_user {
  string href;
  string id;
  string display_name;
};


#endif
