#ifndef __refcount_h
#define __refcount_h

/*
 * refcount.h
 *
 * This file contains private specifications for using the imalloc
 * allocator used as a course project for the 2012 IOOP/M course.
 *
 */

 #include "imalloc.h"
 #include "utilities.h"
 #include <stdlib.h>

/*
 * Increment adress's refcount by 1
 */
unsigned int retain(void *object);

/*
 * Decrement adress's refcount by 1
 */
unsigned int release(Memory mem, void *object);

/*
 * Return adress's refcount
 */
unsigned int count(void *object);

/*
 * Returns an object pointer from a refcount pointer.
 */
unsigned int* object_to_refcount(void *object);

/*
 * Returns a refcount pointer from an object pointer.
 */
void* refcount_to_object(unsigned int *refcount);

#endif
