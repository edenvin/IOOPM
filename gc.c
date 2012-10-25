#include "gc.h"

/*
 * Typedefs from the specification
 */
typedef char *RawPtr;
typedef struct {
RawPtr start;
RawPtr end;
} address_space;

typedef void (*MarkFun)(void *ptr, void *data);


void traverse_stack (address_space h, mark_fun f, void *p);
/*
 * Our functions
 */


/*
 * Returns true if pointer ptr is within the addresspace of the heap that was allocated using iMalloc
 */
Boolean in_address_space (void *ptr, address_space h) {
  if (ptr < h->end && ptr > h->start)
    return TRUE;
  else
    return FALSE;
}

/*
 * Function to traverse the heap and mark items that are linked within
 * the adress space as used.
 */
void traverse_heap(void *ptr, style mem, address_space h){
  chunk_size size = 0;
  while(size < memory_size(ptr)){
    /* If the first intpointer points to something within our adress space,
     * mark the current chunk as used and find pointers from the new chunk. 
     */
    if(in_address_space((int)*ptr + size, h) == TRUE){
      traverse_heap((int)*ptr + size), h, mem);
      set_memory_mark(((int)*ptr + size), TRUE);
      size = size+sizeof(int);
    }
    // When no pointer was found, continue to read the rest of the chunk as int pointers
    else
      size = size+sizeof(int);
  }
  return NULL;
}


 /*
  * The first stage of the mark & sweep algorithm.
  * Traverses the alloclist and sets all mark_bits to false.
  */
void stage_one (style mem) {
  while (current_chunk) {           // be om next_chunk
    set_memory_status (current_chunk ,false);
    current_chunk = next_chunk;       
  } 
}

/*
 * The sweep-stage of the mark & sweep algorithm.
 * Frees the chunks that is no longer being used, the ones with the mark bit set to false.
 */
int sweep (style mem) {
  int i = 0;
  while (!next_chunk) {
    if (memory_is_marked (current_chunk) == FALSE) {
      free_memory(current_chunk);
      i++;
      current_chunk = next_chunk;
    }
    else {
      current_chunk = next_chunk;
    }
  }
  return i;
}

/*
 * Performs a garbage collection according to the mark and sweep algorithm.
 * Returns a positive integer if the sweep stage was successful and memory was freed.
 */
unsigned int collect (style mem) {
  stage_one (mem);
  address_space as;
  as->start = //first object in the heap's addresspace
  as->end = //last object in the heap's addresspace
  traverse_stack (&as, traverse_heap(*ptr, as, mem), NULL);
  return sweep (mem);
}
