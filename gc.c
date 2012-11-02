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
Boolean in_address_space(void *ptr, Priv_mem mem) {
  void *start = as_start(mem);
  void *end = as_end(mem);
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

void traverse_heap(void *ptr, void *mem) {
  void *heap_ptr = *(void **) ptr;
  
  // Search for a chunk that contains this pointer.
  Chunk chunk = search_memory(heap_ptr, alloclist(mem), FALSE);
  
  // We found a chunk! Mark it and check it's pointers.
  if (chunk) {
    // Only traverse if this chunk hasn't been processed already.
    if (memory_is_marked(chunk) == FALSE) {
      if (in_address_space(ptr, mem))
        printf("HEAP: %p found at %p resulted in chunk with start at %p. Traversing pointers in chunk.\n", heap_ptr, ptr, chunk->start);
      else
        printf("STACK: %p found at %p resulted in chunk with start at %p. Traversing pointers in chunk.\n", heap_ptr, ptr, chunk->start);
      set_memory_mark(chunk, TRUE);
      // Go through each possible pointer in the chunk and see if it points
      // to another chunk.
      for (void **cursor = chunk->start; (void *)cursor < (void *)(chunk->start + chunk->size); cursor++) {
        if (in_address_space(*cursor, mem)) {
          // The pointer points to the stack!
          traverse_heap(cursor, mem);
        }
      }
    }
    else {
      if (in_address_space(ptr, mem))
        printf("HEAP: %p found at %p resulted in chunk with start at %p, but this chunk has already been traversed!\n", heap_ptr, ptr, chunk->start);
      else
        printf("STACK: %p found at %p resulted in chunk with start at %p, but this chunk has already been traversed!\n", heap_ptr, ptr, chunk->start);
    }
  }
}

void print_stack_pointers_to_chunks(void *ptr, void *mem) {
  void *heap_ptr = *(void **) ptr;
  
  Chunk chunk = search_memory(heap_ptr, alloclist(mem), FALSE);
  if (chunk) {
    printf("STACKDUMP: %p found at %p resulted in chunk with start at %p.\n", heap_ptr, ptr, chunk->start);
  }
}

//! Frees the chunks that are no longer being used.
/*!
 * The sweep-stage of the mark & sweep algorithm.
 * Frees the chunks that is no longer being used, the ones with
 * the mark bit set to false.
 */
int sweep(Priv_mem mem){
  int i = 0;
  Chunk current_chunk = alloclist(mem); 
  Chunk temp = NULL;
  //Initiate iterator
  while (current_chunk){
    if (memory_is_marked(current_chunk) == FALSE){
      i++;
      temp = current_chunk;
      current_chunk = next_chunk(current_chunk);
      priv_free(priv_to_style(mem), memory_start(temp));
    } else {
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
void mark_unused(Priv_mem mem){
  Chunk current_chunk = alloclist(mem);
  while (current_chunk){
    set_memory_mark(current_chunk ,FALSE);
    current_chunk = next_chunk(current_chunk);
  }
}

unsigned int collect_help(Priv_mem mem) {
  mark_unused(mem);
  printf("\n\n traversing stack and heap\n\n");
  traverseStack(mem->as, &traverse_heap, mem);
  printf("\n\n traversing complete.\n\n");
  unsigned int i = sweep(mem);
  return i;
}

//! Performs a garbage collection according to the mark and sweep algorithm.
/*!
 * Performs a garbage collection according to the mark and sweep algorithm.
 * Returns a positive integer if the sweep stage was successful and memory 
 * was freed.
 * The integer returned is corresponding to the number of memory blocks freed.
 */
unsigned int collect(Memory memory){
  return collect_help(style_to_priv(memory));
}
