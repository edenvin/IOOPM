#ifndef __gc_h
#define __gc_h
#include "imalloc.h"
#include "memory.h"
/*
 * gc.h
 * This file contains the public specifications for using the
 * Managed, automatic garbage collection allocation.
 */

/* 
 * Performs a garbage collection according to the mark and sweep algorithm.
 */
chunk collect(style mem);


#endif
