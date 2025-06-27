#include "types.h"

/*
 * Type Handlers:
 * Each function behaves like a helper to the main function
 * for a certain type of data.
 * It prints information about the provided data,
 * offers multiple choices to interfer with it and its related
 * data via the API or via other handler functions.
 */

/*
 * handle_album:
 * Prints data about the album. Offers the possibility to save album in
 * user's library and to call handle_artist/handle_track for its 
 * artists/tracks.
 */
void handle_album(Album album);

/*
 * handle_artist:
 * Prints data about the artist. Offers the possibility to follow artist and
 * to call handle_album/handle_track for their albums/tracks.
 */
void handle_artist(Artist artist);

/*
 * handle_playlist:
 * Prints data about the playlist. Offers the possibility to follow playlist
 * and to call handle_track for its tracks.
 */
void handle_playlist(Playlist playlist);

/*
 * handle_track:
 * Prints data about the track. Offers the possibility to add the track to a
 * playlist and to call handle_album/handle_artist for its album/artists.
 */
void handle_track(Track track);
