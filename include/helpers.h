#ifndef HELPERS_H
#define HELPERS_H

#include "types.h"

/*
 * Helpers:
 * This file contains helpers for the main file and the helper file dedicated
 * to types handling (type-handlers).
 * The helpers are either variables to save data or functions that interact
 * with this data or with the user.
 */

/*
 * Prints options_count options provided as arguments.
 * Prompts the user to choose an option. Returns the index of the selected
 * option if the index is valid, else returns -1.
 */
int handle_option_choice(size_t options_count, ...);

/*
 * update_playlists:
 * Queries the API for user's owned and followed playlists and uses
 * the response to update owned_playlists and followed_playlists.
 */
void update_playlists(void);

/*
 * update_followed_artists:
 * Queries the API for the user's followed artists and uses the response 
 * to update followed_artists.
 */
void update_followed_artists(void);

#endif
