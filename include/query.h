#ifndef QUERY_H
#define QUERY_H

#include "types.h"

/*
 * Query:
 * This module's functions are wrappers that will fetch the API using
 * the fetch function from the "fetch" header.
 * If no error occurred when fetching the API and the response isn't empty,
 * the function will continue by calling a "cJSON_to_" function from the 
 * "cjson-converters" header.
 * If a "cJSON_to_" function was called, its return value will be returned,
 * else the function doesn't return anything.
 * Any error will cause the program to terminate.
 */


// Album queries

/*
 * query_get_album:
 * Queries the API for an album with and id of id.
 * Returns the album if found, else returns a null pointer.
 */
Album query_get_album(string id);

/*
 * query_get_user_saved_albums:
 * Queries the API for the albums saved by the user,
 * skipping the first offset elements.
 * Returns the albums as a page structure.
 */
Page query_get_user_saved_albums(size_t offset);

/*
 * query_put_user_saved_albums:
 * Queries the API to save albums in user's saved albums.
 */
void query_put_user_saved_albums(Album *albums);

/*
 * query_delete_user_saved_albums:
 * Queries the API to delete albums from user's saved albums.
 */
void query_delete_user_saved_albums(Album *albums);

/*
 * Queries the API for recently released albums, skipping the first
 * offset elements.
 * Returns the albums as a page structure.
 */
Page query_get_new_albums(size_t offset);

// Artist queries

/*
 * query_get_artist:
 * Queries the API for an artist with and id of id.
 * Returns the artist if found, else returns a null pointer.
 */
Artist query_get_artist(string id);

/*
 * query_get_artist_albums:
 * Queries the API for albums of the artist having an id of id, 
 * skipping the first offset albums.  
 * Returns the albums as a page structure.
 */
Page query_get_artist_albums(string id, size_t offset);

/*
 * query_get_artist_top_tracks:
 * Queries the API for the top tracks of the artist having an id of id.
 * Returns a null-terminated array of tracks.
 */
Track *query_get_artist_top_tracks(string id);

// Playlist queries

/*
 * query_get_playlist:
 * Queries the API for a playlist with an id of id.
 * Returns the playlist if found, else returns a null pointer.
 */
Playlist query_get_playlist(string id);

/*
 * query_put_playlist_details:
 * Queries the API to update playlist's details.
 */
void query_put_playlist_details(Playlist playlist);

/*
 * query_get_playlist_tracks:
 * Queries the API to get tracks from a playlist having an id of id,
 * skipping the first offset tracks.
 * Returns the tracks as a page structure.
 */
Page query_get_playlist_tracks(string id, size_t offset);

/*
 * query_put_playlist_tracks:
 * Queries the API to add tracks to playlist.
 */
void query_put_playlist_tracks(Playlist playlist, Track *tracks);

/*
 * query_delete_playlist_tracks:
 * Queries the API to delete tracks from playlist.
 */
void query_delete_playlist_tracks(Playlist playlist, Track *tracks);

/*
 * query_get_user_playlists:
 * Queries the API for user's owned of followed playlists, skipping the first
 * offset playlists.
 * Returns the playlists as a page structure.
 */
Page query_get_user_playlists(size_t offset);

/*
 * query_post_playlist:
 * Queries the API to create a new playlist as user.
 * Use the informations in playlist to create it.
 * Returns the newly created playlist.
 */
Playlist query_post_playlist(User user, Playlist playlist);

// Search queries

/*
 * query_get_all:
 * Queries the API for albums, artists, playlists and tracks
 * using every possible parameter, skipping the first offset results 
 * for each category.
 * Returns the search result.
 * Equivalent to calling query_get_albums, query_get_artists,
 * query_get_playlists and query_get_tracks with the same parameters,
 * but only querying the API once.
 */
Search query_get_all(string album, string artist, string playlist,
                     string track, string year, string genre,
                     bool new, bool hipster, size_t offset);

/*
 * query_get_albums:
 * Queries the API for albums using album, artist, year, new and hipster
 * parameters, skipping the first offset albums.
 * Returns the search result.
 */
Search query_get_albums(string album, string artist, string year,
                        bool new, bool hipster, size_t offset);

/*
 * query_get_artists:
 * Queries the API for artists using artist, year and genre parameters,
 * skipping the first offset artists.
 * Returns the search result.
 */
Search query_get_artists(string artist, string year, string genre,
                         size_t offset);

/*
 * query_get_playlists:
 * Queries the API for playlists using playlist parameter, skipping the
 * first offset playlists.
 * Returns the search result.
 */
Search query_get_playlists(string playlist, size_t offset);

/*
 * query_get_tracks:
 * Queries the API for tracks using track, artist, year, album and genre
 * parameters, skipping the first offset tracks.
 * Returns the search result.
 */
Search query_get_tracks(string track, string artist, string year,
                        string album, string genre, size_t offset);

// Track queries

/*
 * query_get_track:
 * Queries the API for a track having an id of id.
 * Returns the track if found, else returns a null pointer.
 */
Track query_get_track(string id);

/*
 * query_get_user_saved_tracks:
 * Queries the API for tracks saved by user, skipping the first offset tracks.
 * Returns the tracks as a page structure.
 */
Page query_get_user_saved_tracks(size_t offset);

/*
 * query_put_user_saved_tracks:
 * Queries the API to save tracks in user's saved tracks.
 */
void query_put_user_saved_tracks(Track *tracks);

/*
 * query_delete_user_saved_tracks:
 * Queries the API to delete tracks from user's saved tracks.
 */
void query_delete_user_saved_tracks(Track *tracks);

// User queries

/*
 * query_get_user:
 * Queries the API to get user's informations.
 * Returns user if found, else returns null.
 */
User query_get_user(void);

/*
 * query_get_user_top_artists:
 * Queries the API for the "top" artists of the user, skipping the
 * first offset artists.
 * Returns the artists as a page structure.
 */
Page query_get_user_top_artists(size_t offset);

/*
 * query_get_user_top_tracks:
 * Queries the API for the "top" tracks of the user, skipping the
 * first offset tracks.
 * Returns the tracks as a page structure.
 */
Page query_get_user_top_tracks(size_t offset);

/*
 * query_put_follow_playlist:
 * Queries the API to follow playlist.
 */
void query_put_follow_playlist(Playlist playlist);

/*
 * query_delete_unfollow_playlist:
 * Queties the API to unfollow playlist.
 */
void query_delete_unfollow_playlist(Playlist playlist);

/*
 * query_get_followed_artists:
 * Queries the API for user's followed artists.
 * Returns the artists as a page structure.
 */
Page query_get_followed_artists(void);

/*
 * query_put_follow_artists:
 * Queries the API to follow artists.
 */
void query_put_follow_artists(Artist *artists);

/*
 * query_delete_unfollow_artists:
 * Queries the API to unfollow artists.
 */
void query_delete_unfollow_artists(Artist *artists);

#endif
