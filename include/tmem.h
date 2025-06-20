#ifndef TMEM_H
#define TMEM_H

/*
 * Type Structures Memory Allocation functions
 * Each function allocates the memory needed for the specified type structure.
 * If not enough memory is available, the function will return a null pointer.
 * To free the memory allocated by one of the functions, the corresponding
 * "free" deallocation function should be called with the returned pointer 
 * as argument.
 */

/*
 * talloc:
 * Wrapper for calling any of the memory allocation functions.
 * The argument should be a pointer to a memory allocation function.
 * The return value will be a pointer to the new structure if enough memory
 * was available, else it will be a null pointer.
 */
void *talloc(void *(*new_type)(void));

/*
 * new_album:
 * Allocates memory for an album structure and returns a pointer to it.
 */
void *new_album(void);

/*
 * new_simplified_album:
 * Allocates memory for a simplified_album structure and 
 * returns a pointer to it.
 */
void *new_simplified_album(void);

/*
 * new_saved_album:
 * Allocates memory for a saved_album structure and  returns a pointer to it.
 */
void *new_saved_album(void);

/*
 * new_artist:
 * Allocates memory for an artist structure and returns a pointer to it.
 */
void *new_artist(void);

/*
 * new_simplified_artist:
 * Allocates memory for a simplified_artist structure and 
 * returns a pointer to it.
 */
void *new_simplified_artist(void);

/*
 * new_playlist:
 * Allocates memory for a playlist structure and returns a pointer to it.
 */
void *new_playlist(void);

/*
 * new_simplified_playlist:
 * Allocates memory for a simplified_playlist structure and 
 * returns a pointer to it.
 */
void *new_simplified_playlist(void);

/*
 * new_playlist_track:
 * Allocates memory for a playlist_track structure and returns a pointer to it.
 */
void *new_playlist_track(void);

/*
 * new_track:
 * Allocates memory for a track structure and returns a pointer to it.
 */
void *new_track(void);

/*
 * new_simplified_track:
 * Allocates memory for a simplified_track structure and 
 * returns a pointer to it.
 */
void *new_simplified_track(void);

/*
 * new_saved_track:
 * Allocates memory for a saved_track structure and returns a pointer to it.
 */
void *new_saved_track(void);

/*
 * new_user:
 * Allocates memory for a user structure and returns a pointer to it.
 */
void *new_user(void);

/*
 * new_simplified_user:
 * Allocates memory for a simplified_user structure and 
 * returns a pointer to it.
 */
void *new_simplified_user(void);

/*
 * new_followers:
 * Allocates memory for a followers structure and returns a pointer to it.
 */
void *new_followers(void);

/*
 * new_page:
 * Allocates memory for a page structure and returns a pointer to it.
 */
void *new_page(void);

/*
 * new_restrictions:
 * Allocates memory for a restrictions structure and returns a pointer to it.
 */
void *new_restrictions(void);

/*
 * new_search:
 * Allocates memory for a search structure and returns a pointer to it.
 */
void *new_search(void);


/*
 * Type Structures Memory Deallocation functions:
 * Each function deallocates the memory taken by the structure pointed to
 * by its argument.
 * The argument must be a pointer obtained by the call of the corresponding
 * "new" allocation function, otherwise, the behavior of the function is
 * undefined.
 * It is important to note that, considering the nested structures were also
 * allocated using "new" allocation functions,
 * they will also be deallocated by the deallocation function.
 */

/*
 * tfree:
 * Wrapper for calling any of the memory deallocation functions. 
 * The first argument must be a pointer to a memory deallocation function.
 * The second argument must be a pointer to the structure that must be
 * deallocated. That pointer will be passed as an argument when calling
 * the function pointed by the first argument.
 * Calling this function with a pointer to a structure that doesn't correspond 
 * to what the deallocation function expects will result in undefined behavior.
 */
void tfree(void (*free_type)(void *type_struct_ptr), void *type_struct_ptr);

/*
 * free_array:
 * Releases memory taken by a null-terminated array and by its arguments.
 * The first argument should be a pointer to the first element of an array.
 * The second argument should be a function that will release the memory of
 * its argument item. This function will be called for every item in array.
 */
void free_array(void **array, void (*free_item)(void *item));

/*
 * free_album:
 * Releases memory taken by the album structure pointed by album_ptr.
 */
void free_album(void *album_ptr);

/*
 * free_simplified_album: 
 * Releases memory taken by the simplfied_album structure 
 * pointed by simplified_album_ptr.
 */
void free_simplified_album(void *simplified_album_ptr);

/*
 * free_saved_album: 
 * Releases memory taken by the saved_album structure 
 * pointed by saved_album_ptr.
 */
void free_saved_album(void *saved_album_ptr);

/*
 * free_artist: 
 * Releases memory taken by the artist structure pointed by artist_ptr.
 */
void free_artist(void *artist_ptr);

/*
 * free_simplified_artist: 
 * Releases memory taken by the simplified_artist structure 
 * pointed by simplified_artist_ptr.
 */
void free_simplified_artist(void *simplified_artist_ptr);

/*
 * free_playlist: 
 * Releases memory taken by the playlist structure pointed by playlist_ptr.
 */
void free_playlist(void *playlist_ptr);

/*
 * free_simplified_playlist: 
 * Releases memory taken by the simplified_playlist structure 
 * pointed by simplified_playlist_ptr.
 */
void free_simplified_playlist(void *simplified_playlist_ptr);

/*
 * free_playlist_track: 
 * Releases memory taken by the playlist_track structure 
 * pointed by playlist_track_ptr.
 */
void free_playlist_track(void *playlist_track_ptr);

/*
 * free_track: 
 * Releases memory taken by the track structure pointed by track_ptr.
 */
void free_track(void *track_ptr);

/*
 * free_simplified_track: 
 * Releases memory taken by the simplified_track structure 
 * pointed by simplified_track_ptr.
 */
void free_simplified_track(void *simplified_track_ptr);

/*
 * free_saved_track: 
 * Releases memory taken by the saved_track structure 
 * pointed by saved_track_ptr.
 */
void free_saved_track(void *saved_track_ptr);

/*
 * free_user: 
 * Releases memory taken by the user structure pointed by user_ptr.
 */
void free_user(void *user_ptr);

/*
 * free_simplified_user:
 * Releases memory taken by the simplified_user structure 
 * pointed by simplified_user_ptr.
 */
void free_simplified_user(void *simplified_user_ptr);

/*
 * free_followers: 
 * Releases memory taken by the followers structure pointed by followers_ptr.
 */
void free_followers(void *followers_ptr);

/*
 * free_page: 
 * Releases memory taken by the page structure pointed by page_ptr.
 */
void free_page(void *page_ptr);

/*
 * free_restrictions:
 * Releases memory taken by the restrictions structure 
 * pointed by restrictions_ptr.
 */
void free_restrictions(void *restrictions_ptr);

/*
 * free_search: 
 * Releases memory taken by the search structure pointed by search_ptr.
 */
void free_search(void *search_ptr);

#endif
