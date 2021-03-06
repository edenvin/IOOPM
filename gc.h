#ifndef __gc_h
#define __gc_h

/*
 * gc.h
 * This file contains the public specifications for using the
 * Managed, automatic garbage collection allocation.
 */

#include "imalloc.h"
#include "utilities.h"
#include "rootset/rootset.h"

/* 
 * Performs a garbage collection according to the mark and sweep algorithm.
 */
unsigned int collect (Memory mem);

void print_stack_pointers_to_chunks(void *ptr, void *mem);

#endif
