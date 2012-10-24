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

void traverse_stack (adress_space h, mark_fun f, void *p);
/*
 * Our functions
 */

Boolean in_address_space (void *ptr, adress_space h) {
  if (ptr < h->end && ptr > h->start) {
    return TRUE;
  }
    else {
      return FALSE;
    }
}

/*
 *
 */
void traverse_heap (void *ptr, style mem, address_space h) {
  Boolean ptr_in_adr_spc = in_address_space (ptr, h);
  if (ptr_in_adr_spc == TRUE) {                 // if the pointer is within the address space (should be, but it depends on traverse_stack)
    *ptr = potential_ptr; // bit unsure about the loop-part
    while (potential_ptr < h->end) {
      if (in_address_space (potential_ptr,address_space) == TRUE) {
        ptr_mark(potential_ptr, mem);
      }
      else {
        return;
      }
    potential_ptr = *potential_ptr;
    }
  }
}

/*
 * Funciton to mark all the chunk's whose corresponding object were found with pointers from the stack as alive.
 * Also marks a chunk as alive if you can "backtrace" a number of pointer from an object back to the stack.
 * Objects on the heap must be "connected" to the stack via one (or more) pointers to be considered alive.
 */
void ptr_mark(void *ptr,/* void *ignore,*/ style mem) {
  set_memory_status(memory_search(*ptr, mem), true);
}

 /*
  * The first stage of the mark & sweep algorithm.
  * Traverses the alloclist and sets all mark_bits to false.
  */
void stage_one (style mem) {
  while (!next_chunk) {           // be om next_chunk
    set_memory_status (chunk,false);
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
