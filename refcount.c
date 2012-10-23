#include "refcount.h"

unsigned int retain(void* object) {
	unsigned int* t = REFCOUNT(object);
	*t = *t + 1;
	return *t;
}

unsigned int release(Memory mem, void* object) {
	unsigned int* t = REFCOUNT(object);
	*t = *t - 1;
  if (*t == 0) {
    // PLACEHOLDER free the object with the help of ifree PLACEHOLDER
    return 0;
  } 
  return *t;
}

unsigned int count(void* object) {
  return *REFCOUNT(object);
}