#include <stdlib.h>
#include "priv_imalloc.h"


/* 
 * Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
Memory priv_imalloc(chunk_size memsiz, unsigned int flags) {
  Priv_mem new_mem = malloc(sizeof(priv_mem));
  new_mem->as = malloc(sizeof(addressspace));
  new_mem->as->start = malloc(memsiz);
  new_mem->as->end = new_mem->as->start + memsiz - 1;
  
  // MANUAL
  if (flags < 13) {
    new_mem->functions.manual.alloc        = &manual_alloc;
    new_mem->functions.manual.avail        = &avail;
    new_mem->functions.manual.free         = &priv_free;
    new_mem->lists = lists_based_on_flags(flags, 9, 10, 12, new_mem->as->start, memsiz);
    return priv_to_style((Priv_mem) new_mem);
  } else {
    new_mem->functions.managed.alloc = &managed_alloc;
    // MANAGED + REFCOUNT
    if (flags < 21) {
      set_priv_mem_managed_functions(new_mem, &retain, &release, &count, NULL, NULL);
      new_mem->lists = lists_based_on_flags(flags, 17, 18, 20, new_mem->as->start, memsiz);
  // MANAGED + GCD
    } else if (flags < 37) {
      set_priv_mem_managed_functions(new_mem, NULL, NULL, NULL, &typed_alloc, &collect);
      new_mem->lists = lists_based_on_flags(flags, 33, 34, 36, new_mem->as->start, memsiz);
  // MANAGED + REFCOUNT + GCD
    } else {
      set_priv_mem_managed_functions(new_mem, &retain, &release, &count, &typed_alloc, &collect);
      new_mem->lists = lists_based_on_flags(flags, 49, 50, 52, new_mem->as->start, memsiz);
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
  
  Boolean using_refcount = temp->functions.managed.rc.retain != NULL;
  Boolean using_gc = temp->functions.managed.gc.alloc != NULL;
  
  Chunk new_chunk = claim_memory(size, temp->lists);
  
  // Allocation failed.
  if (new_chunk == NULL) {
    // If we're using gc, we need to run garbage the collection and retry the allocation.
    if (using_gc) {
      if (collect(mem) == 0)
        return NULL;
      else
        return managed_alloc(mem, size);
    }
    else {
      // We're not using gc, so no garbage collection can be done. The allocation failed.
      return NULL;
    }
  }
  
  if (using_refcount) {
    object = refcount_to_object(memory_start(new_chunk));
    retain(object);
  }
  else {
    object = memory_start(new_chunk);
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
  return (Memory) ((void**) mem + 2);
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
Priv_mem style_to_priv(Memory mem) {
  return (Priv_mem) ((void**) mem - 2);
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
    return mem->as->start;
}

/*
 * Returns pointer to the end of the allocated address space of memory mem
 */
void* as_end(Priv_mem mem) {
    return mem->as->end;
}
