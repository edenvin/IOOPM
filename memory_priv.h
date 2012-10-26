#ifndef __memory_priv_h
#define __memory_priv_h

#include "imalloc.h"
#include "memory.h"

/*
 * Creates a new chunk with given size and start at given pointer.
 * The chunk will not be marked.
 */
Chunk new_chunk(void *start, chunk_size size, Chunk next);

/*
 * Frees a chunk struct from memory.
 */
void free_chunk(Chunk chunk);

/*
 * Frees a whole lists struct.
 */
void free_lists(Lists lists);

/*
 * Inserts chunk into the freelist in lists.
 */
void insert_chunk_to_freelist(Lists lists, Chunk chunk);

#endif
