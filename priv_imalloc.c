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
    new_mem->functions.free         = &manual_free;
    new_mem->lists = lists_based_on_flags(flags, 9, 10, 12, new_mem->start, memsiz);
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
      new_mem->functions.gc.alloc   = NULL;
      new_mem->functions.gc.collect = NULL;
      new_mem->lists = lists_based_on_flags(flags, 17, 18, 20, new_mem->start, memsiz);
  // MANAGED + GC
    } else if (flags < 37) {
      new_mem->functions.rc.retain  = NULL;
      new_mem->functions.rc.release = NULL;
      new_mem->functions.rc.count   = NULL;
      new_mem->functions.gc.alloc   = &typed_alloc;
      new_mem->functions.gc.collect = &collect;
      new_mem->lists = lists_based_on_flags(flags, 33, 34, 36, new_mem->start, memsiz);
  // MANAGED + REFCOUNT + GC
    } else {
      new_mem->functions.rc.retain  = &retain;
      new_mem->functions.rc.release = &release;
      new_mem->functions.rc.count   = &count;
      new_mem->functions.gc.alloc   = &typed_alloc;
      new_mem->functions.gc.collect = &collect;
      new_mem->lists = lists_based_on_flags(flags, 49, 50, 52, new_mem->start, memsiz);
    }
    return priv_to_style((Priv_mem) new_mem);
  }
}

/*
 * Returns a Lists object based on flags and x, y, z.
 */
Lists lists_based_on_flags(unsigned int flags, unsigned int x, unsigned int y, unsigned int z, void *start, chunk_size memsiz) {
  if (flags == x)
    return create_lists(start, memsiz, ASCENDING_SIZE);
  else if (flags == y)
    return create_lists(start, memsiz, DESCENDING_SIZE);
  else if (flags == z)
    return create_lists(start, memsiz, ADDRESS);
}

/* 
 * Frees object in manual memory mem, returns the amount of memory freed
 */
unsigned int manual_free(Memory mem, void *object) {
  Priv_manual temp = (Priv_manual) style_to_priv(mem);
  Chunk temp_list  = manual_alloclist(temp);
  Chunk needle     = search_memory(object, temp_list, FALSE);
  chunk_size size  = memory_size(needle);
  free_memory(needle, temp->lists);
  return size;
}

/* 
 * Frees object in managed memory mem, returns the amount of memory freed
 */
unsigned int managed_free(Memory mem, void *object) {
  Priv_managed temp = (Priv_managed) style_to_priv(mem);
  Chunk temp_list  = managed_alloclist(temp);
  Chunk needle     = search_memory(object, temp_list, FALSE);
  chunk_size size  = memory_size(needle);
  free_memory(needle, temp->lists);
  return size;
}

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* manual_alloc(Memory mem, chunk_size size) {
  Priv_manual temp = (Priv_manual) style_to_priv(mem);
  Chunk new_chunk = claim_memory(size, temp->lists);
  return memory_start(new_chunk);
}

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* managed_alloc(Memory mem, chunk_size size) {
  Priv_managed temp = (Priv_managed) style_to_priv(mem);
  void* object;
  if (temp->functions.rc.retain == NULL){
    Chunk new_chunk = claim_memory(size, temp->lists);
    if (new_chunk == NULL) {
      if (collect(mem) == 0) {
        return NULL;
      }
      new_chunk = claim_memory(size, temp->lists);
    }
    object = memory_start(new_chunk);
  } else {
    Chunk new_chunk = claim_memory(size+sizeof(int), temp->lists);
    if (temp->functions.gc.alloc == NULL) {
      if (new_chunk == NULL) {
        if (collect(mem) == 0) {
          object = NULL;
        }
        new_chunk = claim_memory(size+sizeof(int), temp->lists);
      }
    }
    object = OBJECT(memory_start(new_chunk));
    retain(object);
  }
  return object;
}

/*
 * Allocates memory for the given chunk size as a string
 * and returns a pointer for the allocated memory 
 */
void* typed_alloc(Memory mem, format_string size) {
  chunk_size decoded_size = format_string_to_size(size);
  return managed_alloc(mem, decoded_size);
}

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
  return (Memory) ((void**) mem + 3);
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
Priv_mem style_to_priv(Memory mem) {
  return (Priv_mem) ((void**) mem - 3);
}

/*
 * Returns the freelist in manual memory mem.
 */
Chunk manual_freelist(Priv_manual mem) {
    return memory_freelist(mem->lists);
}

/*
 * Returns the freelist in managed memory mem.
 */
Chunk managed_freelist(Priv_managed mem) {
    return memory_freelist(mem->lists);
}

/*
 * Returns the alloclist in manual memory mem.
 */
Chunk manual_alloclist(Priv_manual mem) {
    return memory_alloclist(mem->lists);
}

/*
 * Returns the alloclist in managed memory mem.
 */
Chunk managed_alloclist(Priv_managed mem) {
    return memory_alloclist (mem->lists);
}

/*
 * Returns pointer to the start of the allocated address space of manual memory mem
 */
void* manual_as_start(Priv_manual mem) {
    return mem->start;
}

/*
 * Returns pointer to the start of the allocated address space of managed memory mem
 */
void* managed_as_start(Priv_managed mem) {
    return mem->start;
}

/*
 * Returns pointer to the end of the allocated address space of manual memory mem
 */
void* manual_as_end(Priv_manual mem) {
    return mem->end;
}

/*
 * Returns pointer to the end of the allocated address space of managed memory mem
 */
void* managed_as_end(Priv_managed mem) {
    return mem->end;
}
