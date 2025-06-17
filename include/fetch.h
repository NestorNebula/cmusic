#ifndef FETCH_H
#define FETCH_H

#include <cjson/cJSON.h>
#include "types.h"

/*
 * fetch:
 * Fetches data using the provided URL and method.
 * If body isn't null, includes it as the body of the request.
 * The method argument must be one of "GET", "POST", "PUT" and "DELETE".
 * Once the data has been fetched, parses it using cJSON module.
 * Returns the parsed data if no error occurred, else returns a null pointer.
 */
cJSON *fetch(string url, string method, string body);

#endif
