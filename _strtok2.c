#include "main.h"

char* _strtok(char* str, const char* delim)
{
  static char* last;
  char* token;

  if (str != NULL) {
    last = str;
  }

  token = last;//Hello World....{" ?,"}

  while (*last != '\0') {
    for (const char* d = delim; *d != '\0'; ++d) {
      if (*last == *d) {
        *last++ = '\0';
        return token;
      }
    }
    ++last;
  }

  return NULL;
}


