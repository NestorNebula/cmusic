#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include "fetch.h"

#define IS_METHOD(method) (IS_GET(method) || IS_POST(method) || \
                           IS_PUT(method) || IS_DELETE(method))
#define IS_GET(method) !strcmp(method, "GET")
#define IS_POST(method) !strcmp(method, "POST")
#define IS_PUT(method) !strcmp(method, "PUT")
#define IS_DELETE(method) !strcmp(method, "DELETE")

typedef struct response {
  string content;
  size_t size;
} *Response;

/*
 * token:
 * token is the authorization token that will be sent for each API request.
 */
string token = NULL;

/*
 * curl_cb:
 * Callback used by curl_easy_perform to write the response's content.
 * contents is a pointer to the response's content,
 * size is the size of a "member" of contents,
 * nmemb is the number of "members" of contents,
 * res_ptr is a pointer defined by the user on 
 * a previous call of CURLOPT_WRITEDATA.
 * Returns the total size of written content (size * nmemb) 
 * if no error occurred, else returns 0.
 */
static size_t curl_cb(void *contents, size_t size, size_t nmemb, 
                      void *res_ptr);

/*
 * call_api:
 * Uses curl to call an API using url, method and body.
 * method must be one of "GET", "POST", "PUT" and "DELETE".
 * If body is null, the request's body will be set to null.
 * Returns the API's response as a JSON string.
 * If url or method is null, if method isn't valid or if an error occurs
 * while calling the API, terminates the program.
 */
static string call_api(string url, string method, string body);

cJSON *fetch(string url, string method, string body) {
  string json_res = call_api(url, method, body);
  cJSON *res = cJSON_Parse(json_res);
  free(json_res);
  return res;
}

static size_t curl_cb(void *contents, size_t size, size_t nmemb, 
                      void *res_ptr) {
  Response res = (struct response *) res_ptr;

  size_t total_size = size * nmemb;
  string resized_content = realloc(res->content, res->size + total_size + 1);
  if (resized_content == NULL) return 0;
  res->content = resized_content;

  memcpy(res->content + res->size, contents, total_size);
  res->size += total_size;

  return total_size;
}

static string call_api(string url, string method, string body) {
  if (url == NULL || method == NULL || !IS_METHOD(method)) exit(EXIT_FAILURE);

  CURL *curl;
  CURLcode rc = (CURLcode) CURLE_OK - 1;

  struct response res;
  res.content = malloc(sizeof(char));
  if (res.content == NULL) return res.content;
  res.size = 0;
  res.content[res.size] = '\0';

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&res);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    struct curl_slist *list = NULL;
    if (token != NULL) {
      size_t authorization_size =
        strlen("Authorization: Bearer ") + strlen(token) + 1;
      char authorization_str[authorization_size];
      strcpy(authorization_str, "Authorization: Bearer ");
      strcat(authorization_str, token);
      list = curl_slist_append(list, authorization_str);
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    }

    if (body != NULL) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    }

    if (IS_GET(method)) {
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    } else if (IS_POST(method)) {
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
    } else if (IS_PUT(method)) {
      curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    } else {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    }

    rc = curl_easy_perform(curl);

    curl_slist_free_all(list);

    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  if (rc != CURLE_OK) {
    free(res.content);
    exit(EXIT_FAILURE);
  } else return res.content;
}
