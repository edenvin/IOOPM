#ifndef __refcount_h
#define __refcount_h

/*
 * refcount.h
 *
 * This file contains private specifications for using the imalloc
 * allocator used as a course project for the 2012 IOOP/M course.
 *
 */

 #include <imalloc.h>
 #include <priv_imalloc.h>

/*
 * Increment adress's refcount by 1
 */
void retain(void *block);

/*
 * Decrement adress's refcount by 1
 */
void release(void *block);

/*
 * Return adress's refcount
 */
unsigned int count(void *block);


 #endif