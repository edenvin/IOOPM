#include "memory.h"

/* PRIVATE FUNCITONS */

/*
 * Creates a new chunk with given size and start at given pointer.
 * The chunk will not be marked.
 */
Chunk new_chunk(void *start, chunk_size size, Chunk next) {
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
 * Returns TRUE if the chunk is free, FALSE if not.
 */
Boolean memory_is_marked(Chunk chunk) {
  return (chunk && chunk->mark);
}

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
 * Returns the size of the chunk.
 */
 chunk_size memory_size(Chunk chunk) {
   if (chunk == NULL)
     return 0;
   return chunk->size;
 }
 
/*
 * Searches for a chunk in the memory.
 */
Chunk search_memory(void *needle, Chunk haystack) { return NULL; }

/*
 * Claims a part of the memory. Finds a suitable chunk in the free-list,
 * splits it if neccessary and pushes the chunk on to the allocated-list.
 * Keeps the free-list sorted.
 */
Chunk claim_memory(chunk_size size, Chunk free_list, Chunk alloc_list) { return NULL; }

/*
 * Removes the chunk from the allocated-list and adds it to the free-list.
 * Combines the new chunk on the free-list with surrounding chunks if possible.
 * Keeps the free-list sorted.
 */
void free_memory(Chunk chunk) {}

/*
 * Creates a new chunklist with a first chunk of given size and start at given pointer.
 */
Chunk new_chunklist(void *start, chunk_size size) {
  return new_chunk(start, size, NULL);
}

/*
 * Returns the next chunk after chunk
 */
 
Chunk next_chunk(Chunk chunk) {
  if (chunk == NULL)
    return NULL;
  return chunk->next;
}
