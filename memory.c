#include "memory.h"
#include "memory_priv.h"

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
 * Returns a pointer to the start of the memory represented by chunk.
 */
void *memory_start(Chunk chunk) {
  if (chunk == NULL)
    return NULL;
  return chunk->start;
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
  Chunk chunk = memory_freelist(lists);
  Chunk prev = NULL;
  
  while (chunk) {
    if (chunk->size >= size) {
      // We found a chunk large enough! Extract it.
      unlink_chunk_from_freelist(chunk, prev, lists);
      
      // Create two new chunks with smaller size.
      Chunk alloc = new_chunk(chunk->start, size, memory_alloclist(lists));
      lists->alloclist = alloc;
      
      chunk_size remaining = chunk->size - size;
      if (remaining > 0) {
        void *start = chunk->start+size;
        Chunk new = new_chunk(start, remaining, NULL);

        // Insert chunk to freelist
        insert_chunk_to_freelist(lists, new);
      }
      return alloc;
    }
    prev = chunk;
    chunk = chunk->next;
  }
  
  // If no splittable chunk was found, return null.

  return NULL;
}

/*
 * Removes the chunk from the allocated-list and adds it to the free-list.
 * Combines the new chunk on the free-list with surrounding chunks if possible.
 * Keeps the free-list sorted.
 */
void free_memory(Chunk chunk, Lists lists) {
  Chunk cursor = memory_alloclist(lists);
  Chunk prev = NULL;
  // Find the previous chunk.
  while (cursor) {
    if (chunk == cursor)
      break;
      
    prev = cursor;
    cursor = cursor->next;
  }
  
  // Unlink it.
  unlink_chunk_from_alloclist(chunk, prev, lists);
  
  // Combine with adjecent.
  chunk = combine_adjecent(lists, chunk);
  
  // Insert it.
  insert_chunk_to_freelist(lists, chunk);
}

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

/*
 * Inserts chunk into the freelist in lists.
 */
void insert_chunk_to_freelist(Lists lists, Chunk chunk) {
  chunk = combine_adjecent(lists, chunk);
  
  Chunk cursor = lists->freelist;
  
  // There is no freelist.
  if (cursor == NULL)
    lists->freelist = chunk;
  
  while (cursor) {
    
    // if chunk is going in as the first element in freelist
    if (cursor == lists->freelist &&
      ((lists->sort_style == ASCENDING_SIZE && chunk->size < cursor->size) ||
        (lists->sort_style == DESCENDING_SIZE && chunk->size > cursor->size) ||
        (lists->sort_style == ADDRESS && chunk->start < cursor->start))) {
      chunk->next = cursor;
      lists->freelist = chunk;
      break;
    }
    else if (cursor->next == NULL) {
      cursor->next = chunk;
      break;
    }
    else if ((lists->sort_style == ASCENDING_SIZE && chunk->size >= cursor->size && chunk->size < cursor->next->size) ||
        (lists->sort_style == DESCENDING_SIZE && chunk->size <= cursor->size && chunk->size > cursor->next->size) ||
        (lists->sort_style == ADDRESS && chunk->start > cursor->start && chunk->start < cursor->next->start)) {
      chunk->next = cursor->next;
      cursor->next = chunk;
      break;
    }
    
    cursor = cursor->next;
  }
}

/*
 * Finds any adjecent chunks to chunk, removes them from the freelist and returns them combined into one chunk.
 * If no adjecent chunks are found, return chunk.
 */
Chunk combine_adjecent(Lists lists, Chunk chunk) {
  Boolean before = FALSE;
  Boolean after = FALSE;
  
  Chunk cursor = memory_freelist(lists);
  Chunk prev = NULL;
  Chunk tmp;
  
  while (cursor) {
    if (before && after)
      // We found two adjecent cunks, no need to keep looking.
      break;
    else if (chunk->start == cursor->start + cursor->size) {
      // We found a chunk just before.
      before = TRUE;
      
      // Unlink the chunk from the freelist.
      unlink_chunk_from_freelist(cursor, prev, lists);
      
      // Combine them.
      chunk->size = cursor->size + chunk->size;
      chunk->start = cursor->start;
      
      tmp = cursor;
      cursor = cursor->next;
      free(tmp);
    }
    else if (chunk->start + chunk->size == cursor->start) {
      // We found a chunk just after.
      after = TRUE;
      
      // Unlink the chunk from the freelist.
      unlink_chunk_from_freelist(cursor, prev, lists);
      
      // Combine them.
      chunk->size = chunk->size + cursor->size;
      
      tmp = cursor;
      cursor = cursor->next;
      free_chunk(tmp);
    }
    else {
      prev = cursor;
      cursor = cursor->next;
    }
  }
  
  return chunk;
}

/*
 * Unlinks the chunk from the freelist.
 */
void unlink_chunk_from_freelist(Chunk chunk, Chunk prev, Lists lists) {
  if (prev == NULL)
    lists->freelist = chunk->next;
  else
    prev->next = chunk->next;
}

/*
 * Unlinks the chunk from the alloclist..
 */
void unlink_chunk_from_alloclist(Chunk chunk, Chunk prev, Lists lists) {
  if (prev == NULL)
    lists->alloclist = chunk->next;
  else
    prev->next = chunk->next;
}
