#ifndef __priv_malloc_h
#define __priv_malloc_h

#include "imalloc.h"
#include "format.h"
#include "memory.h"
#include "gc.h"
#include "refcount.h"
#include "utilities.h"

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
struct private_manual {
  void  *data; 
  manual functions;
};

struct private_managed {
  void   *data; 
  managed functions;
};

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

#endif