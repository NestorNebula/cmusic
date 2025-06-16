#include <gtest/gtest.h>
#include <cjson/cJSON.h>
#include <fff/fff.h>

using testing::IsNull;
using testing::NotNull;

#define OBJ_PROPERTY "property"
#define OBJ_VALUE "value"

FAKE_VALUE_FUNC(cJSON *, cJSON_Parse, const char *);

cJSON *cjson_object = cJSON_CreateObject();
cJSON_AddStringToObject(test, OBJ_PROPERTY, OBJ_VALUE);

TEST(fetch, fetches_api_using_curl_easy_perform) {
  RESET_FAKE(curl_easy_peform);
  fetch("https://test.com", "GET", NULL);
  EXPECT_EQ(curl_easy_perform_fake.call_count, 1);
}

TEST(fetch, returns_json_parsed_with_cjson) {
  cJSON *cjson_returned = fetch("https://test.com", "GET", NULL);
  ASSERT_THAT(cjson_returned, NotNull());
  cJSON *str_item = cJSON_GetObjectItemCaseSensitive(cjson_returned, OBJ_PROPERTY);
  ASSERT_TRUE(cJSON_IsString(str_item));
  EXPECT_STREQ(str_item->string, OBJ_PROPERTY);
  EXPECT_STREQ(str_item->valuestring, OBJ_VALUE);
}

TEST(fetch, returns_null_on_curl_error) {
  curl_easy_perform_fake.return_val = CURLE_OK + 1;
  EXPECT_THAT(fetch("https://test.com", "GET", NULL), IsNull());
  curl_easy_perform_fake.return_val = CURLE_OK;
}

TEST(fetch, returns_null_when_invalid_method_is_passed) {
  EXPECT_THAT(fetch("https://test.com", "METHOD", NULL), IsNull());
}
