#ifndef TPRINT_H
#define TPRINT_H

#include "types.h"


/*
 * Type Structures Printers functions:
 * The printers wrappers are implemented as function-like macros to allow types
 * that have many properties in common to call the same function-like macro.
 * Each type has "reals" function "print details" and "print essentials",
 * called by the function-like macros depending on the values of their
 * arguments.
 */

/*
 * print_array:
 * Prints array as a list with a number as index (one-indexed) for each item.
 * Prints each array item by calling item_printer with with_details and
 * is_simplified as arguments.
 * Expects a null terminated array as first argument. The array's items must have a
 * type expected by the item_printer function.
 */
#define print_array(array, item_printer, with_details, is_simplified) \


/*
 * print_album:
 * Calls print_album_details, print_album_essentials,
 * print_simplified_album_details or print_simplified_album_essentials
 * depending on the values of with_details and is_simplified.
 * Expects an Album or SimplifiedAlbum argument.
 */
#define print_album(album, with_details, is_simplified) \

/*
 * print_album_details:
 * Prints album with detailed informations.
 */
void print_album_details(Album album);

/*
 * print_album_essentials:
 * Prints album's essential informations.
 */
void print_album_essentials(Album album);

/*
 * print_simplified_album_details:
 * Prints simplified album with detailed informations.
 */
void print_simplified_album_details(SimplifiedAlbum simplified_album);

/*
 * print_simplified_album_essentials:
 * Prints simplified album's essential informations.
 */
void print_simplified_album_essentials(SimplifiedAlbum simplified_album);


/*
 * print_artist:
 * Calls print_artist_details, print_artist_essentials,
 * print_simplified_artist_details or print_simplified_artist_essentials
 * depending on the values of with_details and is_simplified.
 * Expects an Artist or SimplifiedArtist argument.
 */
#define print_artist(artist, with_details, is_simplified) \

/*
 * print_artist_details:
 * Prints artist with detailed informations.
 */
void print_artist_details(Artist artist);

/*
 * print_artist_essentials:
 * Prints artist's essential informations.
 */
void print_artist_essentials(Artist artist);

/*
 * print_simplified_artist_details:
 * Prints simplified artist with detailed informations.
 */
void print_simplified_artist_details(SimplifiedArtist simplified_artist);

/*
 * print_simplified_artist_essentials:
 * Prints simplified artist's essential informations.
 */
void print_simplified_artist_essentials(SimplifiedArtist simplified_artist);


/*
 * print_playlist:
 * Calls print_playlist_details, print_playlist_essentials,
 * print_simplified_playlist_details or print_simplified_playlist_essentials
 * depending on the values of with_details and is_simplified.
 * Expects a Playlist or SimplifiedPlaylist argument.
 */
#define print_playlist(playlist, with_details, is_simplified) \

/*
 * print_playlist_details:
 * Prints playlist with detailed informations.
 */
void print_playlist_details(Playlist playlist);

/*
 * print_playlist_essentials:
 * Prints playlist's essential informations.
 */
void print_playlist_essentials(Playlist playlist);

/*
 * print_simplified_playlist_details:
 * Prints simplified playlist with detailed informations.
 */
void print_simplified_playlist_details(SimplifiedPlaylist simplified_playlist);

/*
 * print_simplified__playlist_essentials:
 * Prints simplified playlist's essential informations.
 */
void print_simplified_playlist_essentials(SimplifiedPlaylist
                                            simplified_playlist);


/*
 * print_track:
 * Calls print_track_details, print_track_essentials,
 * print_simplified_track_details or print_simplified_track_essentials
 * depending on the values of with_details and is_simplified.
 * Expects a Track or SimplifiedTrack argument.
 */
#define print_track(track) \

/*
 * print_track_details:
 * Prints track with detailed informations.
 */
void print_track_details(Track track);

/*
 * print_track_essentials:
 * Prints track's essential informations.
 */
void print_track_essentials(Track track);

/*
 * print_simplified_track_details:
 * Prints simplified track with detailed informations.
 */
void print_simplified_track_details(SimplifiedTrack simplified_track);

/*
 * print_simplified_track_essentials:
 * Prints simplified track's essential informations.
 */
void print_simplified_track_essentials(SimplifiedTrack simplified_track);


/*
 * print_user:
 * Calls print_user_details, print_user_essentials,
 * print_simplified_user_details or print_simplified_user_essentials depending
 * on tha values of with_details and is_simplified.
 * Expects a User or SimplifiedUser argument.
 */
#define print_user(user, with_details, is_simplified) \

/*
 * print_user_details:
 * Prints user with detailed informations.
 */
void print_user_details(User user);

/*
 * print_user_essentials:
 * Prints user's essential informations.
 */
void print_user_essentials(User user);

/*
 * print_simplified_user_details:
 * Prints user with detailed informations.
 */
void print_simplified_user_details(SimplifiedUser simplified_user);

/*
 * print_simplified_user_essentials:
 * Prints simplified user's essential informations.
 */
void print_simplified_user_essentials(SimplifiedUser simplified_user);


FILE *print_stream;


#endif
