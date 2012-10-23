#ifndef __memory_h
#define __memory_h
/*
 * memory.h
 *
 * This file contains specifications for the free-list and the allocated-list used in the
 * imalloc allocator used as a course project for the 2012 IOOP/M couse.
 *
 */

#include "imalloc.h"
#include "utilities.h"

typedef struct chunk {
  void *start;
  chunk_size size;
  struct chunk *next;
  Boolean free;
  unsigned short refcount;
} *Chunk;


/*
 * Returns TRUE if the chunk is free, FALSE if not.
 */
Boolean memory_is_free(Chunk chunk);

/*
 * Marks a chunk as free or taken.
 */
Boolean set_memory_status(Chunk chunk, Boolean free);

/*
 * Returns the refcount for the chunk.
 */
unsigned short memory_refcount(Chunk chunk);

/*
 * Increase or decrease the refcount.
 */
unsigned short increase_memory_refcount(Chunk chunk);
unsigned short decrease_memory_refcount(Chunk chunk);

/*
 * Searches for a chunk in the memory.
 */
Chunk search_memory(void *needle, Chunk stack);

/*
 * Claims a part of the memory. Finds a suitable chunk in the free-list,
 * splits it if neccessary and pushes the chunk on to the allocated-list.
 * Keeps the free-list sorted.
 */
Chunk claim_memory(chunk_size size);

/*
 * Removes the chunk from the allocated-list and adds it to the free-list.
 * Combines the new chunk on the free-list with surrounding chunks if possible.
 * Keeps the free-list sorted.
 */
void free_memory(Chunk chunk);


/* PRIVATE FUNCTIONS. MOVE INTO .C */

/*
 * Splits a chunk into two smaller chunks, with the first chunk having the given size,
 * and the second chunk having any remaining space.
 */
void split_memory(Chunk chunk, chunk_size size);

/*
 * Combines two chunks into one larger chunk.
 */
void combine_memory(Chunk first, Chunk last);

#endif
