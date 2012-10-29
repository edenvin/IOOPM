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

struct _chunk {
  void *start;
  chunk_size size;
  struct _chunk *next;
  Boolean mark;
};
typedef struct _chunk *Chunk;

struct _lists {
  Chunk freelist;
  Chunk alloclist;
  unsigned int sort_style;
};
typedef struct _lists *Lists;


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
 * Returns a pointer to the start of the memory represented by chunk.
 */
void *memory_start(Chunk chunk);

/*
 * Searches for a chunk in the memory. If strict is TRUE the needle must match
 * the chunks' start pointer. If FALSE the needle may point to anywhere in the chunks'
 * memory.
 */
Chunk search_memory(void *needle, Chunk haystack, Boolean strict);

/*
 * Claims a part of the memory. Finds a suitable chunk in the free-list,
 * splits it if neccessary and pushes the chunk on to the allocated-list.
 * Keeps the free-list sorted.
 */
Chunk claim_memory(chunk_size size, Lists lists);

/*
 * Removes the chunk from the allocated-list and adds it to the free-list.
 * Combines the new chunk on the free-list with surrounding chunks if possible.
 * Keeps the free-list sorted.
 */
void free_memory(Chunk chunk, Lists lists);

/*
 * Creates a new lists struct with a freelist and an alloclist.
 */
Lists create_lists(void *start, chunk_size size, unsigned int sort_style);

/*
 * Frees a whole chunklist.
 */
void free_chunklist(Chunk list);

/*
 * Returns the next chunk after chunk
 */
Chunk next_chunk(Chunk chunk);

/*
 * Returns the freelist in lists.
 */
Chunk memory_freelist(Lists lists);

/*
 * Returns the alloclist in lists.
 */
Chunk memory_alloclist(Lists lists);

#endif
