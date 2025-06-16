#include <gtest/gtest.h>
#include <fff/fff.h>
#include <curl/curl.h>
DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(CURLcode, curl_easy_perform, CURL *);

int main(int argc, char *argv[]) {
  curl_easy_perform_fake.return_val = CURLE_OK;
  return RUN_ALL_TESTS();
}
