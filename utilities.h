#ifndef __utilities_h
#define __utilities_h

#include "priv_imalloc.h"

#define REFCOUNT(p) ((unsigned int*) (p) - 1)
#define OBJECT(p) ((void*) ((p) + 1))

/*
 * utilities.h
 *
 * This file contains some utility functions used in the imalloc allocator
 * used as a course project for the 2012 IOOP/M course.
 */

style* priv_to_style(priv_mem* mem);

priv_mem* style_to_priv(style* mem);

Chunk freelist(priv_mem* mem);

Chunk alloclist(priv_mem* mem);

typedef enum { FALSE, TRUE } Boolean;

#endif
