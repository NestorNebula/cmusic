#ifndef CJSON_CONVERTERS_H
#define CJSON_CONVERTERS_H

#include <cjson/cJSON.h>
#include "types.h"

/*
 * cJSON converters:
 * Each "converter" function converts the cJSON tree (cJSON item pointer)
 * argument to the expected structure type.
 * If all the required informations to create the structure could be found,
 * the function will return a pointer to the created structure.
 * If any required information  couldn't be found,
 * the function will end the program prematurely.
 */

/*
 * cjson_to_array:
 * Takes a cJSON array and calls cJSON_to_item_type for each of its items.
 * cJSON_to_item_type is a function pointer taking a cJSON pointer as argument
 * and returning a pointer to the structure created.
 * cJSON_to_item_type should point to a function declared in this module.
 * Returns a pointer to the first element of a null-terminated array of 
 * pointers pointing to structures created by cJSON_to_item_type.
 */
void **cJSON_to_array(cJSON *array, void *(*cJSON_to_item_type)(cJSON *item));

/*
 * cJSON_to_album:
 * Converts cJSON_album to an album structure and returns a pointer to it.
 */
void *cJSON_to_album(cJSON *cJSON_album);

/*
 * cJSON_to_simplified_album:
 * Converts cJSON_simplified_album to a simplified_album structure and 
 * returns a pointer to it.
 */
void *cJSON_to_simplified_album(cJSON *cJSON_simplified_album);

/*
 * cJSON_to_saved_album:
 * Converts cJSON_saved_album to a saved_album structure and 
 * returns a pointer to it.
 */
void *cJSON_to_saved_album(cJSON *cJSON_saved_album);

/*
 * cJSON_to_artist:
 * Converts cJSON_artist to an artist structure and returns a pointer to it.
 */
void *cJSON_to_artist(cJSON *cJSON_artist);

/*
 * cJSON_to_simplified_artist:
 * Converts cJSON_simplified_artist to a simplified_artist structure and 
 * returns a pointer to it.
 */
void *cJSON_to_simplified_artist(cJSON *cJSON_simplified_artist);

/*
 * cJSON_to_playlist:
 * Converts cJSON_playlist to a playlist structure and returns a pointer to it.
 */
void *cJSON_to_playlist(cJSON *cJSON_playlist);

/*
 * cJSON_to_simplified_playlist:
 * Converts cJSON_simplified_playlist to a simplified_playlist structure 
 * and returns a pointer to it.
 */
void *cJSON_to_simplified_playlist(cJSON *cJSON_simplified_playlist);

/*
 * cJSON_to_playlist_track:
 * Converts cJSON_playlist_track to a playlist_track structure and 
 * returns a pointer to it.
 */
void *cJSON_to_playlist_track(cJSON *cJSON_playlist_track);

/*
 * cJSON_to_track:
 * Converts cJSON_track to a track structure and returns a pointer to it.
 */
void *cJSON_to_track(cJSON *cJSON_track);

/*
 * cJSON_to_simplified_track:
 * Converts cJSON_simplified_track to a simplified_track structure and 
 * returns a pointer to it.
 */
void *cJSON_to_simplified_track(cJSON *cJSON_simplified_track);

/*
 * cJSON_to_saved_track:
 * Converts cJSON_saved_track to a saved_track structure and
 * returns a pointer to it.
 */
void *cJSON_to_saved_track(cJSON *cJSON_saved_track);

/*
 * cJSON_to_user:
 * Converts cJSON_user to a user structure and returns a pointer to it.
 */
void *cJSON_to_user(cJSON *cJSON_user);

/*
 * cJSON_to_simplified_user:
 * Converts cJSON_simplified_user to a simplified_user structure and 
 * returns a pointer to it.
 */
void *cJSON_to_simplified_user(cJSON *cJSON_simplified_user);

/*
 * cJSON_to_followers:
 * Converts cJSON_followers to a followers structure and 
 * returns a pointer to it.
 */
void *cJSON_to_followers(cJSON *cJSON_followers);

/*
 * cJSON_to_page:
 * Converts cJSON_page to a page structure and returns a pointer to it.
 * Calls function pointed by cJSON_to_item_type for each of its items.
 */
void *cJSON_to_page(cJSON *cJSON_page, 
                    void *(*cJSON_to_item_type)(cJSON *item));

/*
 * cJSON_to_restrictions:
 * Converts cJSON_restrictions to a restrictions structure and 
 * returns a pointer to it.
 */
void *cJSON_to_restrictions(cJSON *cJSON_restrictions);

/*
 * cJSON_to_search:
 * Converts cJSON_search to a search structure and returns a pointer to it.
 */
void *cJSON_to_search(cJSON *cJSON_search);

#endif
