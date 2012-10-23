#include <string.h>
#include <ctype.h>
#include "format.h"

chunk_size format_string_to_size(format_string str) {
  int i = 0;
  int m = 0;
  int result = 0;
  while (str[i] != '\0') {
    // If digit, adds it with possible former digit.
    if (isdigit(str[i])) {
      m = m*10;
      m = m + (str[i] - 48);
      i++;
    } else {
      // If not digit, check which type and multiply it with m.
      if (m == 0) {
        m = 1;
      }
      switch (str[i]) {
        case '*': result = result + m * sizeof(void*);
        break;
        case 'i': result = result + m * sizeof(int);
        break;
        case 'f': result = result + m * sizeof(float);
        break;
        case 'c': result = result + m * sizeof(char);
        break;
        case 'l': result = result + m * sizeof(long);
        break;
        case 'd': result = result + m * sizeof(double);
        break;
      }
      i++;
      m = 0;
    }
  }
  // If the string only contained numbers, return m. Otherwise return result.
  if (m) {
    return m;
  } else {
    return result;
  }
}