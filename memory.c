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
  chunk->mark = FALSE;
  
  return chunk;
}

/* PUBLIC FUNCTIONS */

/*
 * Marks a chunk as marked or not marked
 */
Boolean set_memory_mark(Chunk chunk, Boolean mark) {
  if (chunk == NULL)
    return FALSE;
  
  chunk->mark = mark;
  return memory_is_marked(chunk);
}

/*
 * Returns TRUE if the chunk is free, FALSE if not.
 */
Boolean memory_is_marked(Chunk chunk) {
  return (chunk && chunk->mark);
}

/*
 * Returns the size of the chunk.
 */
chunk_size memory_size(Chunk chunk) {
  if (chunk == NULL)
    return 0;
  return chunk->size;
}

Chunk new_chunklist(void *start, chunk_size size) {
  return new_chunk(start, size, NULL, 0);
}

/*
 * Returns the first chunk in free_list
 */
Chunk first_free_chunk(style mem) {
  return STYLE_TO_FREE(mem);
}

/*
 * Returns the first chunk in alloc_list
 */
Chunk first_alloc_chunk(style mem) {
  return STYLE_TO_ALLOC(mem);
}

/*
 * Returns the next chunk after chunk
 */
Chunk next_chunk(Chunk chunk) {
  if (chunk == NULL)
    return NULL;
  return chunk->next;
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
