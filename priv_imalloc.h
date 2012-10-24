#ifndef __priv_malloc_h
#define __priv_malloc_h

#include "imalloc.h"
#include "format.h"
#include "memory.h"
#include "gc.h"
#include "refcount.h"

/*
 * priv_imalloc.h
 *
 * This file contains private specifications for using the imalloc
 * allocator used as a course project for the 2012 IOOP/M course.
 *
 * Note that this specification might evolve as the project is
 * running, f.ex. due to technical as well as educational bugs and
 * insights.
 *
 * You MAY ADD and ALTER the definitions in this file.
 *
 */

/* Actual return type specifications for iMalloc */
 
struct priv_mem {
  void* start;
  void* end;
  Chunk freelist;
  Chunk alloclist;
  style functions;
};
typedef struct priv_mem priv_mem;

/* Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
struct style *priv_imalloc(chunk_size memsiz, unsigned int flags);

/* 
 * Frees an object. 
 */
void ifree(void *object);

/* 
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* ialloc(chunk_size size);

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* manual_alloc(chunk_size size);

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* managed_alloc(chunk_size size);

/*
 * Allocates memory for the given chunk size as a string
 * and returns a pointer for the allocated memory 
 */
void* typed_alloc(format_string size);

/*
 * Converts a priv_mem pointer to a style pointer.
 */
style* priv_to_style(priv_mem* mem);

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
priv_mem* style_to_priv(style* mem);

/*
 * Returns the freelist in mem.
 */
Chunk freelist(priv_mem* mem);

/*
 * Returns the alloclist in mem.
 */
Chunk alloclist(priv_mem* mem);

#endif