#include "types.h"

/*
 * read_string:
 * Reads string from stream. Starts at the first non-space character and.
 * stops when meeting a new-line character or end-of-file.
 * Returns the string read as a dynamically allocated string, or a null pointer
 * if an error occurred.
 */
string read_string(FILE *stream);

/*
 * read_integer:
 * Reads an integer from stream and returns it.
 * If an integer could be read, sets variable pointed by success to true,
 * else sets it to false.
 * If no number could be read, the returned value is undefined, thus, it it
 * important to check that a number was read, using the variable pointed 
 * by success.
 */
int read_integer(FILE *stream, bool *success);

/*
 * read_bool:
 * Reads stream and returns true if the first non-space character is a y,
 * else returns false.
 */
bool read_bool(FILE *stream);
