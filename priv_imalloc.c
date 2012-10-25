#include <stdlib.h>
#include "priv_imalloc.h"


/* 
 * Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
struct style *priv_imalloc(chunk_size memsiz, unsigned int flags) {
  priv_mem *new_mem = (priv_mem) malloc(sizeof(priv_mem));
  new_mem->start = (void*) malloc(memsiz);
  new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
  new_mem->freelist = new_chunklist(new_mem->start, memsiz);
  new_mem->alloclist = NULL;
  // MANUAL
  if (flags < 13) {
    (manual) (new_mem->functions)->alloc        = &manual_alloc;
    (manual) (new_mem->functions)->avail        = &priv_avail;
    (manual) (new_mem->functions)->free         = &priv_free;
    switch (flags) {
      case 9:  new_mem->sortstyle = ASCENDING_SIZE;
      break;
      case 10: new_mem->sortstyle = DESCENDING_SIZE;
      break;
      case 12: new_mem->sortstyle = ADRESS;
      break;
    }
  // MANAGED + REFCOUNT
  } else if (flags < 21) {
    (managed) (new_mem->functions)->alloc       = &managed_alloc;
    (managed) (new_mem->functions)->rc->retain  = &retain;
    (managed) (new_mem->functions)->rc->release = &release;
    (managed) (new_mem->functions)->rc->count   = &count;
    (managed) (new_mem->functions)->gc          = NULL;
    switch (flags) {
      case 17: new_mem->sortstyle = ASCENDING_SIZE;
      break;
      case 18: new_mem->sortstyle = DESCENDING_SIZE;
      break;
      case 20: new_mem->sortstyle = ADRESS;
      break;
    }
  // MANAGED + GC
  } else if (flags < 37) {
    (managed) (new_mem->functions)->alloc       = &managed_alloc;
    (managed) (new_mem->functions)->rc          = NULL;
    (managed) (new_mem->functions)->gc->alloc   = &typed_alloc;
    (managed) (new_mem->functions)->gc->collect = &collect;
    switch (flags) {
      case 33: new_mem->sortstyle = ASCENDING_SIZE;
      break;
      case 34: new_mem->sortstyle = DESCENDING_SIZE;
      break;
      case 36: new_mem->sortstyle = ADRESS;
      break;
    }
  // MANAGED + REFCOUNT + GC
  } else {
    (managed) (new_mem->functions)->alloc       = &managed_alloc;
    (managed) (new_mem->functions)->rc->retain  = &retain;
    (managed) (new_mem->functions)->rc->release = &release;
    (managed) (new_mem->functions)->rc->count   = &count;
    (managed) (new_mem->functions)->gc->alloc   = &typed_alloc;
    (managed) (new_mem->functions)->gc->collect = &collect;
    switch (flags) {
      case 49: new_mem->sortstyle = ASCENDING_SIZE;
      break;
      case 50: new_mem->sortstyle = DESCENDING_SIZE;
      break;
      case 52: new_mem->sortstyle = ADRESS;
      break;
    }
  }
  return priv_to_style(new_mem);
}

/* 
 * Frees an object. 
 */
void priv_free(void *object) {}

/* 
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* priv_alloc(chunk_size size) { return NULL; }

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* manual_alloc(chunk_size size) { return NULL; }

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* managed_alloc(chunk_size size) { return NULL; }

/*
 * Allocates memory for the given chunk size as a string
 * and returns a pointer for the allocated memory 
 */
void* typed_alloc(format_string size) { return NULL; }

/*
 * Returns the total size of the free space in the address space.
 */
unsigned int avail(Memory mem) {
  return 0;
}

/*
 * Converts a priv_mem pointer to a style pointer.
 */
Memory priv_to_style(priv_mem* mem) {
  return (Memory) ((unsigned int) ((void*) ((Chunk) mem + 2) + 2) + 1);
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
priv_mem* style_to_priv(Memory mem) {
  return (priv_mem*) ((unsigned int) ((void*) ((Chunk) mem - 2) - 2) - 1);
}

/*
 * Returns the freelist in mem.
 */
Chunk freelist(priv_mem* mem) {
  return mem->freelist;
}

/*
 * Returns the alloclist in mem.
 */
Chunk alloclist(priv_mem* mem) {
  return mem->alloclist;
}

/*
 * Returns pointer to the start of the allocated address space
 */
void* as_start(priv_mem *mem) {
  return mem->start;
}

/*
 * Returns pointer to the end of the allocated address space
 */
void* as_end(priv_mem *mem) {
  return mem->end;
}
