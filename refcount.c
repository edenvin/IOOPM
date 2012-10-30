#include "refcount.h"

unsigned int retain(void* object) {
  unsigned int* t = object_to_refcount(object);
  *t = *t + 1;
  return *t;
}

unsigned int release(Memory mem, void* object) {
  unsigned int* t = object_to_refcount(object);
  *t = *t - 1;
  if (*t == 0) {
    priv_free(mem, refcount_to_object(object));
    return *t;
  }
  return *t;
}

unsigned int count(void* object) {
  return *object_to_refcount(object);
}

/*
 * Returns an object pointer from a refcount pointer.
 */
unsigned int* object_to_refcount(void *object) {
  return (unsigned int*) object - 1;
}

/*
 * Returns a refcount pointer from an object pointer.
 */
void* refcount_to_object(unsigned int *refcount) {
  return (void*) (refcount + 1);
}
