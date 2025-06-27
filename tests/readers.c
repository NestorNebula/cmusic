#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>
#include "readers.h"

#define TEST_STR "A test string"
#define TEST_INT 10
#define TRUE_STR "YES"
#define FALSE_STR "NO"

Test(read_string, returns_string_read) {
  FILE *stream = tmpfile();
  fputs(TEST_STR, stream);
  rewind(stream);

  string str_read = read_string(stream);
  cr_expect(eq(str, str_read, TEST_STR),
            "Expected returned string (%s) to match %s",
            str_read, TEST_STR);

  free(str_read);
  fclose(stream);
}

Test(read_string, returns_empty_string_on_empty_input) {
  FILE *stream = tmpfile();
  fputs("   ", stream);
  rewind(stream);

  string str_read = read_string(stream);
  cr_expect(eq(str, str_read, ""),
            "Expected returned string (%s) to be empty",
            str_read);

  free(str_read);
  fclose(stream);
}

Test(read_integer, returns_integer_read) {
  FILE *stream = tmpfile();
  fprintf(stream, "%d", TEST_INT);
  rewind(stream);

  int int_read = read_integer(stream, NULL);
  cr_expect(eq(int, int_read, TEST_INT),
            "Expected returned int (%d) to be %d",
            int_read, TEST_INT);

  fclose(stream);
}

Test(read_integer, sets_success_to_true_after_reading_integer_successfully) {
  FILE *stream = tmpfile();
  fprintf(stream, " %d ", TEST_INT);
  rewind(stream);

  bool success = false;
  int int_read = read_integer(stream, &success);
  cr_expect(eq(int, int_read, TEST_INT),
            "Expected returned int (%d) to be %d",
            int_read, TEST_INT);
  cr_expect(eq(int, success, true),
            "Expected success to be true");

  fclose(stream);
}

Test(read_integer, sets_success_to_false_when_integer_could_not_be_read) {
  FILE *stream = tmpfile();
  fputs(TEST_STR, stream);
  rewind(stream);

  bool success = true;
  read_integer(stream, &success);
  cr_expect(eq(int, success, false),
            "Expected success to be false");

  fclose(stream);
}

Test(read_bool, returns_true_when_input_starts_with_y) {
  FILE *stream = tmpfile();
  fputs(TRUE_STR, stream);
  rewind(stream);

  bool bool_read = read_bool(stream);
  cr_expect(eq(int, bool_read, true),
            "Expected read_bool to return true");

  fclose(stream);
}

Test(read_bool, returns_false_when_input_starts_with_n) {
  FILE *stream = tmpfile();
  fputs(FALSE_STR, stream);
  rewind(stream);

  bool bool_read = read_bool(stream);
  cr_expect(eq(int, bool_read, false,),
            "Expected read_bool to return false");

  fclose(stream);
}

Test(read_bool, returns_false_when_input_starts_with_non_expected_character) {
  FILE *stream = tmpfile();
  fputs(TEST_STR, stream);
  rewind(stream);

  bool bool_read = read_bool(stream);
  cr_expect(eq(int, bool_read, false),
            "Expected read_bool to return false");

  fclose(stream);
}
