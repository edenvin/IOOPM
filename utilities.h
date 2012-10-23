#ifndef __utilities_h
#define __utilities_h

#include "imalloc.h"

#define REFCOUNT(p) ((unsigned int*) (p) - 1)
#define OBJECT(p) ((void*) ((p) + 1))
#define STYLE_TO_FREE(p) ((Chunk) (p) - 2)
#define STYLE_TO_ALLOC(p) ((Chunk) (p) - 1)
#define FREE_TO_ALLOC(p) ((p) + 1)
#define FREE_TO_STYLE(p) ((style) ((p) + 2))
#define ALLOC_TO_FREE(p) ((p) - 1)
#define ALLOC_TO_STYLE(p) ((style) ((p) + 1))

/*
 * utilities.h
 *
 * This file contains some utility functions used in the imalloc allocator
 * used as a course project for the 2012 IOOP/M course.
 */

typedef enum { FALSE, TRUE } Boolean;

#endif
