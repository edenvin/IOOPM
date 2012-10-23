#include "gc.h"

/*
 *
 * Performs a garbage collection according to the mark and sweep algorithm.
 *
 */
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

void print_ptr(void *ptr, void *ignore) {
  printf("%p\n", ptr);
}


void stage_one (style mem){
  while (!next_chunk){           // be om next_chunk
    set_memory_status (chunk,false);
    current_chunk = next_chunk;       
  } 
}


unsigned int collect (style mem){
  stage_one (mem);
  *addressspace as;
  as->start = //första objektet i adressrymden
  as->end = //sista objektet i adressrymden
  traverse_stack (&as, print_ptr, NULL);
  
}