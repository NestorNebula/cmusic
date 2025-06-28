#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "readers.h"

#define IS_END_CHAR(ch) ((ch) == '\n' || (ch) == EOF)

string read_string(FILE *stream) {
  string str = malloc(sizeof(char));
  if (str == NULL) return str;
  size_t size = 1;

  int ch;
  for (ch = getc(stream); isspace(ch) && !IS_END_CHAR(ch); ch = getc(stream));

  if (!IS_END_CHAR(ch)) {
    do {
      string resized = realloc(str, size + 1);
      if (resized == NULL) {
        free(str);
        return NULL;
      }
      str = resized;
      str[size - 1] = ch;
      size++;
      ch = getc(stream);
    } while (!IS_END_CHAR(ch));
  }

  str[size - 1] = '\0';

  return str;
}

int read_integer(FILE *stream, bool *success) {
  int i;

  if (success) *success = fscanf(stream, "%d", &i) == 1;
  else fscanf(stream, "%d", &i);
  int ch;
  do {
    ch = getc(stream);
  } while (!IS_END_CHAR(ch));

  return i;
}

bool read_bool(FILE *stream) {
  int ch;
  for (ch = getc(stream); isspace(ch) && !IS_END_CHAR(ch); ch = getc(stream));
  if (!IS_END_CHAR(ch)) {
    int c;
    do {
      c = getc(stream);
    } while (!IS_END_CHAR(c));
  }
  
  return tolower(ch) == 'y';
}
