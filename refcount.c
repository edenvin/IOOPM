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
    //managed_free(mem, object);
    return *t;
  } 
  return *t;
}

unsigned int count(void* object) {
  return *REFCOUNT(object);
}