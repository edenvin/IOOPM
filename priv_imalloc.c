#include <stdlib.h>
#include "priv_imalloc.h"


/* 
 * Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
struct style *priv_imalloc(chunk_size memsiz, unsigned int flags) {
  // MANUAL
  if (flags < 13) {
    Priv_manual new_mem = malloc(sizeof(priv_manual));
    new_mem->start = malloc(memsiz);
    new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
    new_mem->functions.alloc        = &manual_alloc;
    new_mem->functions.avail        = &avail;
    new_mem->functions.free         = &priv_free;
    switch (flags) {
      case 9:  new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
      break;
      case 10: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
      break;
      case 12: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
      break;
    }
    return priv_to_style((Priv_mem) new_mem);
  } else {
    Priv_managed new_mem = malloc(sizeof(priv_managed));
    new_mem->start = malloc(memsiz);
    new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
    new_mem->functions.alloc = &managed_alloc;
    // MANAGED + REFCOUNT
    if (flags < 21) {
      new_mem->functions.rc.retain  = &retain;
      new_mem->functions.rc.release = &release;
      new_mem->functions.rc.count   = &count;
      switch (flags) {
        case 17: new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
        break;
        case 18: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
        break;
        case 20: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
        break;
      }
  // MANAGED + GC
    } else if (flags < 37) {
      new_mem->functions.gc.alloc   = &typed_alloc;
      new_mem->functions.gc.collect = &collect;
      switch (flags) {
        case 33: new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
        break;
        case 34: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
        break;
        case 36: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
        break;
      }
  // MANAGED + REFCOUNT + GC
    } else {
      new_mem->functions.rc.retain  = &retain;
      new_mem->functions.rc.release = &release;
      new_mem->functions.rc.count   = &count;
      new_mem->functions.gc.alloc   = &typed_alloc;
      new_mem->functions.gc.collect = &collect;
      switch (flags) {
        case 49: new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
        break;
        case 50: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
        break;
        case 52: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
        break;
      }
    }
    return priv_to_style((Priv_mem) new_mem);
  }
}

/* 
 * Frees an object. 
 */
unsigned int priv_free(Memory mem, void *object) { return 0; }

/* 
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* priv_alloc(Memory mem, chunk_size size) { return NULL; }

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* manual_alloc(Memory mem, chunk_size size) { return NULL; }

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* managed_alloc(Memory mem, chunk_size size) { return NULL; }

/*
 * Allocates memory for the given chunk size as a string
 * and returns a pointer for the allocated memory 
 */
void* typed_alloc(Memory mem, format_string size) { return NULL; }

/*
 * Returns the total size of the free space in the address space.
 */
unsigned int avail(Memory mem) {
  return 0;
}

/*
 * Converts a priv_mem pointer to a style pointer.
 */
Memory priv_to_style(Priv_mem mem) {
  return (Memory) ((void*) ((Lists) mem + 1) + 2);
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
Priv_mem style_to_priv(Memory mem) {
  return (Priv_mem) ((void*) ((Lists) mem - 1) - 2);
}

/*
 * Returns the freelist in mem.
 */
Chunk freelist(Priv_mem mem, unsigned int flag) {
  if (flag)
    return memory_freelist(((Priv_managed) mem)->lists);
  else
    return memory_freelist(((Priv_manual) mem)->lists);
}

/*
 * Returns the alloclist in mem.
 */
Chunk alloclist(Priv_mem mem, unsigned int flag) {
  if (flag)
    return memory_alloclist(((Priv_managed) mem)->lists);
  else
    return memory_alloclist(((Priv_manual) mem)->lists);
}

/*
 * Returns pointer to the start of the allocated address space
 */
void* as_start(Priv_mem mem, unsigned int flag) {
  if (flag)
    return ((Priv_managed) mem)->start;
  else
    return ((Priv_manual) mem)->start;
}

/*
 * Returns pointer to the end of the allocated address space
 */
void* as_end(Priv_mem mem, unsigned int flag) {
  if (flag)
    return ((Priv_managed) mem)->end;
  else
    return ((Priv_manual) mem)->end;
}
