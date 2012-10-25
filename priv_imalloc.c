#include "priv_imalloc.h"


/* Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
struct style *priv_imalloc(chunk_size memsiz, unsigned int flags) { return NULL; }


/* 
 * Frees an object. 
 */
void ifree(void *object) {}

/* 
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* ialloc(chunk_size size) { return NULL; }

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
 * Converts a priv_mem pointer to a style pointer.
 */
style* priv_to_style(priv_mem* mem) {
  return (style*) mem+(2 * sizeof(void*) + 2 * sizeof(Chunk));
}

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
priv_mem* style_to_priv(style* mem) {
  return (priv_mem*) mem-(2 * sizeof(void*) + 2 * sizeof(Chunk));
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
