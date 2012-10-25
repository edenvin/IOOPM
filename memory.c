#include "memory.h"
#include "memory_priv.h"

/* PRIVATE FUNCITONS */

/*
 * Creates a new chunk with given size and start at given pointer.
 * The chunk will not be marked.
 */
Chunk new_chunk(void *start, chunk_size size, Chunk next) {
  // Allocate memory for a new chunk.
  Chunk chunk = malloc(sizeof(struct _chunk));
  
  chunk->start = start;
  chunk->size = size;
  chunk->next = next;
  chunk->mark = FALSE;
  
  return chunk;
}

/*
 * Frees a chunk struct from memory.
 */
void free_chunk(Chunk chunk) {
  free(chunk);
}

/*
 * Frees a whole lists struct.
 */
void free_lists(Lists lists) {
  free_chunklist(lists->freelist);
  free_chunklist(lists->alloclist);
  free(lists);
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
 * Searches for a chunk in the memory. If strict is TRUE the needle must match
 * the chunks' start pointer. If FALSE the needle may point to anywhere in the chunks'
 * memory.
 */
Chunk search_memory(void *needle, Chunk haystack, Boolean strict) {
  while (haystack) {
    if (needle == haystack->start)
      return haystack;
    else if (!strict && haystack->start < needle && needle < (haystack->start + haystack->size))
      return haystack;
    else
      haystack = next_chunk(haystack);
  }
  return NULL;
}

/*
 * Claims a part of the memory. Finds a suitable chunk in the free-list,
 * splits it if neccessary and pushes the chunk on to the allocated-list.
 * Keeps the free-list sorted.
 */
Chunk claim_memory(chunk_size size, Lists lists) {
  // Traverse free-list until we find a large enough chunk.
  // Split that chunk into two parts.
  // Move one of them to the end of the alloc_list.
  
  // If no splittable chunk was found, return null.
  return NULL;
}

/*
 * Removes the chunk from the allocated-list and adds it to the free-list.
 * Combines the new chunk on the free-list with surrounding chunks if possible.
 * Keeps the free-list sorted.
 */
void free_memory(Chunk chunk) {}

/*
 * Creates a new lists struct with a freelist and an alloclist.
 */
Lists create_lists(void *start, chunk_size size, unsigned int sort_style) {
  Lists lists = malloc(sizeof(struct _lists));
  lists->alloclist = NULL;
  lists->freelist = new_chunk(start, size, NULL);
  lists->sort_style = sort_style;
  
  return lists;
}

/*
 * Frees a whole chunklist.
 */
void free_chunklist(Chunk list) {
  Chunk temp;
  while (list) {
    temp = list;
    free(list);
    list = next_chunk(temp);
  }
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
 * Returns the freelist in lists.
 */
Chunk memory_freelist(Lists lists) {
  return lists->freelist;
}

/*
 * Returns the alloclist in lists.
 */
Chunk memory_alloclist(Lists lists) {
  return lists->alloclist;
}
