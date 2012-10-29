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

/*
 * Finds any adjecent chunks to chunk, removes them from the freelist and returns them combined into one chunk.
 * If no adjecent chunks are found, return chunk.
 */
Chunk combine_adjecent(Lists lists, Chunk chunk);

/*
 * Unlinks the chunk from the freelist.
 */
void unlink_chunk_from_freelist(Chunk chunk, Chunk prev, Lists lists);

/*
 * Unlinks the chunk from the alloclist..
 */
void unlink_chunk_from_alloclist(Chunk chunk, Chunk prev, Lists lists);


#endif
