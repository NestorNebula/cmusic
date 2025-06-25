#ifndef TPRINT_H
#define TPRINT_H

#include <stdio.h>
#include "types.h"

/*
 * Type Structures Printers functions:
 * The printers are implemented as function-like macros to allow types
 * that have many properties in common to call the same function.
 * Each type has a "print details" and a "print essentials" function.
 * If a type is incompatible with the details function, it will be redirected
 * toward the essentials function. If it is incompatible with the essentials
 * function, nothing will be printed.
 * A more general function is also provided for each type. Depending of its
 * argument, it will decide to call the details or the essentials function.
 * As functions expect particular types as arguments but are implemented
 * as function-like macros, calling these functions with unexpected types
 * will result in undefined behavior.
 */

/*
 * print_array:
 * Prints array as a list with a number as index (one-indexed) for each item.
 * Prints each array item using item_printer.
 * Expects a null terminated array as argument. The array's items must have a
 * type expected by the item_printer function.
 */
#define print_array(array, item_printer)


/*
 * print_album:
 * Calls print_album_details if album has type Album and has no missing
 * information. Else, calls print_album_essentials.
 * Expects an Album or SimplifiedAlbum argument.
 */
#define print_album(album)

/*
 * print_album_details:
 * Prints album with detailed informations.
 * Expects an Album or SimplifiedAlbum argument.
 */
#define print_album_details(album)

/*
 * print_album_essentials:
 * Prints album's essential informations.
 * Expects an Album or SimplifiedAlbum argument.
 */
#define print_album_essentials(album)


/*
 * print_artist:
 * Calls print_artist_details if artist has type Artist and has no missing
 * information. Else, calls print_artist_essentials.
 * Expects an Artist or SimplifiedArtist argument.
 */
#define print_artist(artist)

/*
 * print_artist_details:
 * Prints artist with detailed informations.
 * Expects an Artist argument.
 */
#define print_artist_details(artist)

/*
 * print_artist_essentials:
 * Prints artist's essential informations.
 * Expects an Artist or SimplifiedArtist argument.
 */
#define print_artist_essentials(artist)


/*
 * print_playlist:
 * Calls print_playlist_details if playlist has type Playlist and has no
 * missing information. Else, calls print_playlist_essentials.
 * Expects a Playlist or SimplifiedPlaylist argument.
 */
#define print_playlist(playlist)

/*
 * print_playlist_details:
 * Prints playlist with detailed informations.
 * Expects a Playlist or SimplifiedPlaylist argument.
 */
#define print_playlist_details(playlist)

/*
 * print_playlist_essentials:
 * Prints playlist's essential informations.
 * Expects a Playlist or SimplifiedPlaylist argument.
 */
#define print_playlist_essentials(playlist)


/*
 * print_track:
 * Calls print_track_details if track has type Track and has no missing
 * information. Else, calls print_track_essentials.
 * Expects a Track or SimplifiedTrack argument.
 */
#define print_track(track)

/*
 * print_track_details:
 * Prints track with detailed informations.
 * Expects a Track or SimplifiedTrack argument.
 */
#define print_track_details(track)

/*
 * print_track_essentials:
 * Prints track's essential informations.
 * Expects a Track or SimplifiedTrack argument.
 */
#define print_track_essentials(track)


/*
 * print_user:
 * Calls print_user_details if user has type User and has no missing 
 * information. Else, calls print_user_essentials.
 * Expects a User or SimplifiedUser argument.
 */
#define print_user(user)

/*
 * print_user_details:
 * Prints user with detailed informations.
 * Expects a User argument.
 */
#define print_user_details(user)

/*
 * print_user_essentials:
 * Prints user's essential informations.
 * Expect a User or SimplifiedUser argument.
 */
#define print_user_essentials(user)

FILE *print_stream;


#endif
