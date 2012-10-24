#ifndef __memory_h
#define __memory_h
/*
 * memory.h
 *
 * This file contains specifications for the free-list and the allocated-list used in the
 * imalloc allocator used as a course project for the 2012 IOOP/M couse.
 *
 */

#include <stdlib.h>
#include "imalloc.h"
#include "utilities.h"

typedef struct _chunk {
  void *start;
  chunk_size size;
  struct _chunk *next;
  Boolean mark;
} *Chunk;


/*
 * Returns TRUE if the chunk is marked, FALSE if not.
 */
Boolean memory_is_marked(Chunk chunk);

/*
 * Marks a chunk as marked or not marked
 */
Boolean set_memory_mark(Chunk chunk, Boolean mark);

/*
 * Returns the size of the chunk.
 */
chunk_size memory_size(Chunk chunk);

/*
 * Searches for a chunk in the memory.
 */
Chunk search_memory(void *needle, Chunk haystack);

/*
 * Claims a part of the memory. Finds a suitable chunk in the free-list,
 * splits it if neccessary and pushes the chunk on to the allocated-list.
 * Keeps the free-list sorted.
 */
Chunk claim_memory(chunk_size size, Chunk free_list, Chunk alloc_list);

/*
 * Removes the chunk from the allocated-list and adds it to the free-list.
 * Combines the new chunk on the free-list with surrounding chunks if possible.
 * Keeps the free-list sorted.
 */
void free_memory(Chunk chunk);

/*
 * Creates a new chunklist with a first chunk of given size and start at given pointer.
 */
Chunk new_chunklist(void *start, chunk_size size);

/*
 * Returns the next chunk after chunk
 */
Chunk next_chunk(Chunk chunk);

#endif
