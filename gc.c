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
 * Funciton to mark all the chunk's whose corresponding object were found with pointers from the stack as alive.
 * Also marks a chunk as alive if you can "backtrace" a number of pointer from an object back to the stack.
 * Objects on the heap must be "connected" to the stack via one (or more) pointers to be considered alive.
 */
void ptr_mark(void *ptr,/* void *ignore,*/ style mem) {
  set_memory_status(memory_search(*ptr, mem), true);
}
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
 * Goes through every sizeof(int) in the destination on the heap of *ptr and 
 * tries to read it as a pointer to see if there is any pointers to anything else within
 * the adress space.
 */
void *find_pointers(void *ptr, address_space h, style mem){
  chunk_size size = 0;
  while(size < memory_size(ptr)){
    if(in_address_space((int)*ptr + size, h) == TRUE)
      find_pointers((int)*ptr + size), h, mem);
      set_memory_mark(((int)*ptr + size), free)
      size = size+sizeof(int);
    else
      size = size+sizeof(int);
  }
  return NULL;
}

/*
 * Function to traverse the heap and mark items that are linked within
 * the adress space as used.
 */
void traverse_heap (void *ptr, style mem, address_space h) {
  if (in_address_space (ptr, h) == TRUE) {// if the pointer is within the address space (should be, but it depends on traverse_stack)

    void *potential_ptr = *ptr; // bit unsure about the loop-part
    while (in_address_space(potential_ptr,h) == TRUE) {
      ptr_mark(potential_ptr, mem);
      potential_ptr = *potential_ptr;
    }
  }
  else
    return;
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
  traverse_stack (&as, ptr_mark (*ptr, *ignore, mem), NULL);
  return sweep (mem);
}
