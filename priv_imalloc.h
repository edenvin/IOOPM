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

typedef struct {
  void *start;
  void *end;
  Lists lists;
  style functions;
} priv_mem, *Priv_mem;

/* 
 * Initiates the malloc library to be used. memsiz defines the
 * maximum amount of memory that can be used. flags specifies kind
 * of memory manager and allows fine-tunes some options.
 */
struct style *priv_imalloc(chunk_size memsiz, unsigned int flags);

/*
 * Set function pointers to a priv_mem struct.
 */
void set_priv_mem_managed_functions(Priv_mem mem, unsigned int (*retain)(void *object), unsigned int (*release)(Memory mem, void *object), unsigned int (*count)(void *object), void* (*typed_alloc)(Memory mem, format_string size), unsigned int (*collect)(Memory mem));

/*
 * Returns a Lists object based on flags and x, y, z.
 */
Lists lists_based_on_flags(unsigned int flags, unsigned int x, unsigned int y, unsigned int z, void *start, chunk_size memsiz);

/*
 * Frees object in memory mem, returns the amount of memory freed
 */
unsigned int priv_free(Memory mem, void *object);

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* manual_alloc(Memory mem, chunk_size size);

/*
 * Allocates memory for the given chunk size 
 * and returns a pointer for the allocated memory 
 */
void* managed_alloc(Memory mem, chunk_size size);

/*
 * Allocates memory for the given chunk size as a string
 * and returns a pointer for the allocated memory 
 */
void* typed_alloc(Memory mem, format_string size);

/*
 * Returns the total size of the free space in the address space.
 */
unsigned int avail(Memory mem);

/*
 * Converts a priv_mem pointer to a style pointer.
 */
Memory priv_to_style(Priv_mem mem);

/*
 * Converts a style style pointer to a priv_mem pointer.
 */
Priv_mem style_to_priv(Memory mem);

/*
 * Returns the freelist in memory mem.
 */
Chunk freelist(Priv_mem mem);

/*
 * Returns the alloclist in memory mem.
 */
Chunk alloclist(Priv_mem mem);

/*
 * Returns pointer to the start of the allocated address space of managed memory mem
 */
void* as_start(Priv_mem mem);

/*
 * Returns pointer to the start of the allocated address space of manual memory mem
 */
void* as_end(Priv_mem mem);

#endif
