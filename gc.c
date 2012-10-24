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

traverse_stack (adress_space h, mark_fun f, void *p);
/*
 * Our functions
 */

/*
 * Funciton to mark all the chunk's whose corresponding object were found with pointers from the stack as alive.
 * Also marks a chunk as alive if you can "backtrace" a number of pointer from an object back to the stack.
 * Objects on the heap must be "connected" to the stack via one (or more) pointers to be considered alive.
 */
void ptr_mark(void *ptr, void *ignore, style mem) {
  set_memory_status(memory_search(*ptr, mem), true);
}

 /*
  * The first stage of the mark & sweep algorithm.
  * Traverses the alloclist and sets all mark_bits to false.
  */
void stage_one (style mem){
  while (!next_chunk){           // be om next_chunk
    set_memory_status (chunk,false);
    current_chunk = next_chunk;       
  } 
}

/*
 * The sweep-stage of the mark & sweep algorithm.
 * Frees the chunks that is no longer being used, the ones with the mark bit set to false.
 */
int sweep (style mem){
  int i = 0;
  while (!next_chunk){
    free_memory(current_chunk);
    i++;
    current_chunk = next_chunk;
  }
  return i;
}

/*
 * Performs a garbage collection according to the mark and sweep algorithm.
 * Returns a positive integer if the sweep stage was successful and memory was freed.
 */
unsigned int collect (style mem){
  stage_one (mem);
  address_space as;
  as->start = //first object in the heap's addresspace
  as->end = //last object in the heap's addresspace
  traverse_stack (&as, ptr_mark (*ptr, *ignore, mem), NULL);
  return sweep (mem);
}