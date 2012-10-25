#include "gc.h"
#include "priv_imalloc.h"
/*
 * Typedefs from the specification
 */
typedef char *RawPtr;
typedef struct {
  RawPtr start;
  RawPtr end;
} address_space;

typedef void (*MarkFun)(void *ptr, void *data);

/* Scans the stack, CPU registers, and static data to build a
* root set, R s.t. for all r in R, h->start <= r && r <= h->end.
* Then, for all r in R, calls f(r, p), where p is just some
* additional user-provided payload.
*/
void traverse_stack (address_space h, MarkFun f, void *p);

/*
 * Returns true if pointer ptr is within the addresspace on the heap that was allocated using iMalloc
 */
Boolean in_address_space(void *ptr, address_space h) {
  if (ptr < h.end && ptr > h.start){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

/*
 * Funciton to traverse the heap and mark all the chunk's whose 
 * corresponding object were found with pointers from the stack as alive.
 * Also marks a chunk as alive if you can "backtrace" a number of pointers
 * from an object back to the stack.
 * Objects on the heap must be "connected" to the stack via
 * one (or more) pointers to be considered alive.
 */
void traverse_heap(void *ptr, address_space h, priv_mem *mem){
  chunk_size size = 0;
  Chunk chunk = alloclist (mem);
  while (size < memory_size(search_memory(ptr,chunk))){
    /* If the first intpointer points to something within our adress space,
     * mark the current chunk as used and find pointers from the new chunk. 
     */
     if (in_address_space((int)*ptr + size, h) == TRUE){
      traverse_heap(((int)*ptr + size), h, mem);
      set_memory_mark(((int)*ptr + size), TRUE);
      size = size+sizeof(int);
    }
    // If no pointer was found, continue to read the rest of the chunk as int pointers
    else{
      size = size+sizeof(int);
    }
  }
  return NULL;
}
 /*
  * The first stage of the mark & sweep algorithm.
  * Traverses the alloclist and sets all mark_bits to false.
  */
void mark_unused(priv_mem *mem){
  Chunk current_chunk = alloclist(mem);
  while (current_chunk){
    set_memory_mark(current_chunk ,FALSE);
    current_chunk = next_chunk(current_chunk);       
  } 
}

/*
 * The sweep-stage of the mark & sweep algorithm.
 * Frees the chunks that is no longer being used, the ones with
 * the mark bit set to false.
 */
int sweep(priv_mem *mem){
  int i = 0;
  Chunk current_chunk = alloclist(mem); 
  //Initiate iterator
  while (current_chunk){
    if (memory_is_marked(current_chunk) == FALSE){
      free_memory(current_chunk);
      i++;
      current_chunk = next_chunk(current_chunk);
    }
    else
      current_chunk = next_chunk(current_chunk);
  }
  return i;
}

/*
 * Performs a garbage collection according to the mark and sweep algorithm.
 * Returns a positive integer if the sweep stage was successful and memory was freed.
 * The integer returned is corresponding to the number of memory blocks freed.
 */
unsigned int collect(Memory mem){
  priv_mem *memory_private = style_to_priv(mem);
  mark_unused(memory_private);
  address_space as;
  as.start = as_start(memory_private);
  as.end = as_end(memory_private);
  traverse_stack(as, traverse_heap(*ptr, as, memory_private), NULL);
  unsigned int i = sweep(memory_private);
  return i;
}
