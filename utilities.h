#ifndef __utilities_h
#define __utilities_h

#define REFCOUNT(p) ((unsigned int*) (p) - 1)
#define OBJECT(p) ((void*) ((p) + 1))

/*
 * utilities.h
 *
 * This file contains some utility functions used in the imalloc allocator
 * used as a course project for the 2012 IOOP/M course.
 */

typedef enum { FALSE, TRUE } Boolean;

enum { F_MANUAL = 0, F_MANAGED = 1 } Type;

#endif
