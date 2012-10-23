#include "memory.h"

/* PRIVATE FUNCITONS */

/*
 * Creates a new chunk with given size and start at given pointer.
 * The chunk will be set as free, and it's refcount will be 0.
 */
Chunk new_chunk(void *start, chunk_size size, Chunk next, unsigned short refcount) {
  // Allocate memory for a new chunk.
  Chunk chunk = malloc(sizeof(Chunk));
  
  chunk->start = start;
  chunk->size = size;
  chunk->next = next;
  chunk->refcount = refcount;
  chunk->mark = FALSE;
  
  return chunk;
}

/* PUBLIC FUNCTIONS */

/*
 * Returns TRUE if the chunk is free, FALSE if not.
 */
Boolean memory_is_marked(Chunk chunk) {
  return chunk->mark;
}

Chunk new_chunklist(void *start, chunk_size size) {
  return new_chunk(start, size, NULL, 0);
}



/*
 * Splits a chunk into two smaller chunks, with the first chunk having the given size,
 * and the second chunk having any remaining space.
 */
void split_memory(Chunk chunk, chunk_size size) {}

/*
 * Combines two chunks into one larger chunk.
 */
void combine_memory(Chunk first, Chunk last) {}
