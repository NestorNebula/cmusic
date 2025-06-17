#include <stdlib.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <fff/fff.h>
#include <curl/curl.h>
#include "fetch.h"

DEFINE_FFF_GLOBALS;

#define OBJ_PROPERTY "property"
#define OBJ_VALUE "value"

FAKE_VALUE_FUNC(CURLcode, curl_easy_perform, CURL *);
FAKE_VALUE_FUNC(cJSON *, cJSON_Parse, const char *);

cJSON *cjson_object = NULL;

void setup(void) {
  RESET_FAKE(cJSON_Parse);
  RESET_FAKE(curl_easy_perform);

  if (cjson_object == NULL) {
    cjson_object = cJSON_CreateObject();
    cJSON_AddStringToObject(cjson_object, OBJ_PROPERTY, OBJ_VALUE);
    cJSON_Parse_fake.return_val = cjson_object;
  }

  curl_easy_perform_fake.return_val = CURLE_OK;
}

TestSuite(fetch, .init = setup);

Test(fetch, fetches_api_using_curl_easy_perform) {
  fetch("https://test.com", "GET", NULL);
  cr_expect(eq(int, curl_easy_perform_fake.call_count, 1),
            "Expected curl_easy_perform to have been called");
}

Test(fetch, returns_json_parsed_with_cjson) {
  cJSON *cjson_returned = fetch("https://test.com", "GET", NULL);
  cr_assert(cjson_returned != NULL, 
            "Expected fetch to return non-null content");
  cJSON *str_item = cJSON_GetObjectItemCaseSensitive(cjson_returned, OBJ_PROPERTY);
  cr_assert(cJSON_IsString(str_item), "Expected str_item to be a string");
  cr_expect(eq(str, str_item->string, OBJ_PROPERTY),
            "Expected str_item property to be %s", OBJ_PROPERTY);
  cr_expect(eq(str, str_item->valuestring, OBJ_VALUE),
            "Expected str_item value to be %s", OBJ_VALUE);
}

Test(fetch, terminate_program_on_curl_error, .exit_code = EXIT_FAILURE, 
     .description = "Test") {
  curl_easy_perform_fake.return_val = (CURLcode) CURLE_OK + 1;
  fetch("https://test.com", "GET", NULL);
}

Test(fetch, terminate_program_when_invalid_method_is_passed, 
     .exit_code = EXIT_FAILURE) {
  fetch("https://test.com", "METHOD", NULL);
}
