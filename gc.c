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
 * Funciton to mark all the chunk's (in alloclist) that were found in the stack as alive.
 * In other words, set the mark bit to true.
 */
void ptr_mark(void *ptr, void *ignore, style mem) {
  set_memory_status(memory_search(*ptr, mem), true);
}

/*
 * Our functions
 */

 /*
  * The first stage of the mark and sweep algorithm.
  * Traverses the alloclist and sets all mark_bits to false.
  */
void stage_one (style mem){
  while (!next_chunk){           // be om next_chunk
    set_memory_status (chunk,false);
    current_chunk = next_chunk;       
  } 
}

/*
 *
 * Performs a garbage collection according to the mark and sweep algorithm.
 *
 */
unsigned int collect (style mem){
  stage_one (mem);
  address_space as;
  as->start = //first object in the heap's addresspace
  as->end = //last object in the heap's addresspace
  traverse_stack (&as, ptr_mark (*ptr, *ignore, mem), NULL);

}