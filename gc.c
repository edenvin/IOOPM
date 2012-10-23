#include "gc.h"


/*
 * Typedefs from the specification
 */
typedef char *RawPtr;
typedef struct {
RawPtr start;
RawPtr end;
} addressspace;

typedef void (*MarkFun)(void *ptr, void *data);

traverse_stack (adress_space h, mark_fun f, void *p);

void ptr_mark(void *ptr, void *ignore, style mem) {
  set_memory_status(memory_search(*ptr, mem), true);
}


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
  *addressspace as;
  as->start = //första objektet i adressrymden
  as->end = //sista objektet i adressrymden
  traverse_stack (&as, ptr_mark (*ptr, *ignore, mem), NULL);
  
}