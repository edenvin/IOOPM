#include <stdlib.h>
#include "priv_imalloc.h"


/* 
 * Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
Memory priv_imalloc(chunk_size memsiz, unsigned int flags) {
  // MANUAL
  if (flags < 13) {
    Priv_mem new_mem = malloc(sizeof(priv_mem));
    new_mem->start = malloc(memsiz);
    new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
    new_mem->functions.manual.alloc        = &manual_alloc;
    new_mem->functions.manual.avail        = &avail;
    new_mem->functions.manual.free         = &priv_free;
    new_mem->lists = lists_based_on_flags(flags, 9, 10, 12, new_mem->start, memsiz);
    return priv_to_style((Priv_mem) new_mem);
  } else {
    Priv_mem new_mem = malloc(sizeof(priv_mem));
    new_mem->start = malloc(memsiz);
    new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
    new_mem->functions.managed.alloc = &managed_alloc;
    // MANAGED + REFCOUNT
    if (flags < 21) {
      set_priv_mem_managed_functions(new_mem, &retain, &release, &count, NULL, NULL);
      new_mem->lists = lists_based_on_flags(flags, 17, 18, 20, new_mem->start, memsiz);
  // MANAGED + GCD
    } else if (flags < 37) {
      set_priv_mem_managed_functions(new_mem, NULL, NULL, NULL, &typed_alloc, &collect);
      new_mem->lists = lists_based_on_flags(flags, 33, 34, 36, new_mem->start, memsiz);
  // MANAGED + REFCOUNT + GCD
    } else {
      set_priv_mem_managed_functions(new_mem, &retain, &release, &count, &typed_alloc, &collect);
      new_mem->lists = lists_based_on_flags(flags, 49, 50, 52, new_mem->start, memsiz);
    }
    return priv_to_style((Priv_mem) new_mem);
  }
}

/*
 * Set function pointers to a priv_mem struct.
 */
void set_priv_mem_managed_functions(Priv_mem mem, unsigned int (*retain)(void *object), unsigned int (*release)(Memory mem, void *object), unsigned int (*count)(void *object), void* (*typed_alloc)(Memory mem, format_string size), unsigned int (*collect)(Memory mem)) {
  mem->functions.managed.rc.retain  = retain;
  mem->functions.managed.rc.release = release;
  mem->functions.managed.rc.count   = count;
  mem->functions.managed.gc.alloc   = typed_alloc;
  mem->functions.managed.gc.collect = collect;
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
  else
    return NULL;
}

/* 
 * Frees object in manual memory mem, returns the amount of memory freed
 */
unsigned int priv_free(Memory mem, void *object) {
  Priv_mem temp    = style_to_priv(mem);
  Chunk temp_list  = alloclist(temp);
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
  Priv_mem temp = style_to_priv(mem);
  Chunk new_chunk = claim_memory(size, temp->lists);
  return memory_start(new_chunk);
}

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* managed_alloc(Memory mem, chunk_size size) {
  Priv_mem temp = style_to_priv(mem);
  void* object;
  if (temp->functions.managed.rc.retain == NULL){
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
    if (temp->functions.managed.gc.alloc == NULL) {
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
 * Returns the largest possible size available for allocation.
 */
unsigned int avail(Memory mem) {
  Priv_mem new_mem = style_to_priv(mem);
  unsigned int avail = 0;
  Chunk freelist = new_mem->lists->freelist;
  while (freelist) {
    if (freelist->size > avail) {
      avail = freelist->size;
    }
    freelist = freelist->next;
  }
  return avail;
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
 * Returns the freelist in memory mem.
 */
Chunk freelist(Priv_mem mem) {
    return memory_freelist(mem->lists);
}

/*
 * Returns the alloclist in memory mem.
 */
Chunk alloclist(Priv_mem mem) {
    return memory_alloclist(mem->lists);
}

/*
 * Returns pointer to the start of the allocated address space of memory mem
 */
void* as_start(Priv_mem mem) {
    return mem->start;
}

/*
 * Returns pointer to the end of the allocated address space of memory mem
 */
void* as_end(Priv_mem mem) {
    return mem->end;
}
