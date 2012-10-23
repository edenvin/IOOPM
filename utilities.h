#ifndef __utilities_h
#define __utilities_h
#define REFCOUNT(p) ((unsigned int*) (p) - 1)
#define OBJECT(p) ((void*) ((p) + 1))
#define IMALLOC_TO_FREE ((Chunk) (p) - 2)
#define IMALLOC_TO_ALLOC ((Chunk) (p) - 1)
#define FREE_TO_ALLOC ((p) + 1)
#define FREE_TO_IMALLOC ((style) ((p) + 2))
#define ALLOC_TO_FREE ((p) - 1)
#define ALLOC_TO_IMALLOC ((style) ((p) + 1))

#include "imalloc.h"
#include "memory.h"

/*
 * utilities.h
 *
 * This file contains some utility functions used in the imalloc allocator
 * used as a course project for the 2012 IOOP/M course.
 */

typedef enum { FALSE, TRUE } Boolean;

#endif
