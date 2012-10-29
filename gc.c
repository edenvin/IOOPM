/*!
 * \file gc.c
 *
 * \author Celine Dion
 * \date 2012-10-26
 *
 * Functions needed to perform garbage collection
 */
#include "gc.h"
#include "priv_imalloc.h"

//! Checks if the pointer is pointing within the adress space.
/*!
 * Returns true if pointer ptr is within the addresspace on the heap that
 * was allocated using iMalloc.
 */
Boolean in_address_space(void *ptr, Priv_managed mem) {
  void *start = managed_as_start(mem);
  void *end = managed_as_end(mem);
  if (ptr < end && ptr > start){
    return TRUE;
  }
  else{
    return FALSE;
  }
}

//! Traverses the heap and marks all objects with pointers as alive.
/*!
 * Function to traverse the heap and mark all the chunk's whose 
 * corresponding object were found with pointers from the stack as alive.
 * Also marks a chunk as alive if you can "backtrace" a number of pointers
 * from an object back to the stack.
 * Objects on the heap must be "connected" to the stack via
 * one (or more) pointers to be considered alive.
 */
void traverse_heap(void *ptr, void *mem){
  chunk_size size = 0;
  Chunk chunk = managed_alloclist (mem);
  while (size < memory_size(search_memory(ptr,chunk,FALSE))){
    /* If the first intpointer points to something within our adress space,
     * mark the current chunk as used and find pointers from the new chunk. 
     */
     if (in_address_space(ptr + 1, mem) == TRUE){
      traverse_heap((Chunk)(ptr + 1), mem);
      set_memory_mark((Chunk)(ptr + 1), TRUE);
      size++;
    }
    // If no pointer was found, continue to read the rest of the chunk as int pointers
    else{
      size = size+sizeof(int);
    }
  }
  return;
}

//! Frees the chunks that are no longer being used.
/*!
 * The sweep-stage of the mark & sweep algorithm.
 * Frees the chunks that is no longer being used, the ones with
 * the mark bit set to false.
 */
int sweep(Priv_managed mem){
  int i = 0;
  Chunk current_chunk = managed_alloclist(mem); 
  //Initiate iterator
  while (current_chunk){
    if (memory_is_marked(current_chunk) == FALSE){
      free_memory(current_chunk);
      i++;
      current_chunk = next_chunk(current_chunk);
    }
    else{
      current_chunk = next_chunk(current_chunk);
    }
  }
  return i;
}

//! Marks all the objects on the heap as dead.
 /*!
  * The first stage of the mark & sweep algorithm.
  * Traverses the alloclist and sets all mark_bits to false.
  */
void mark_unused(Priv_managed mem){
  Chunk current_chunk = managed_alloclist(mem);
  while (current_chunk){
    set_memory_mark(current_chunk ,FALSE);
    current_chunk = next_chunk(current_chunk);       
  } 
}

//! Performs a garbage collection according to the mark and sweep algorithm.
/*!
 * Performs a garbage collection according to the mark and sweep algorithm.
 * Returns a positive integer if the sweep stage was successful and memory 
 * was freed.
 * The integer returned is corresponding to the number of memory blocks freed.
 */
unsigned int collect(Memory memory){
  SET_STACK_BOTTOM
  Priv_managed mem = (Priv_managed) style_to_priv(memory);
  mark_unused(mem);
  AddressSpace as;
  as->start = managed_as_start(mem);
  as->end = managed_as_end(mem);
  traverseStack(as, &traverse_heap, mem);
  unsigned int i = sweep(mem);
  return i;
}