#ifndef TPRINT_H
#define TPRINT_H

#include "types.h"


/*
 * Type Structures Printers functions:
 * Each type has two printers functions, a "details" printer that will
 * print many informations about the data and an "essentials" printer
 * that will only print a few informations about it.
 */

/*
 * print_array:
 * Prints array as a list with a number as index (one-indexed) for each item.
 * Prints each array item by calling print_item.
 * Expects a null terminated array as first argument. The array's items must have a
 * type expected by the print_item function.
 */
#define print_array(array, print_item) \


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
