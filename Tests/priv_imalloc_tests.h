#ifndef __priv_imalloc_tests_h
#define __priv_imalloc_tests_h
/*
 * refcount_tests.h
 *
 * This file contains unit tests for the memory part of the imalloc allocator.
 *
 */

#include "CUnit/Basic.h"
#include "../priv_imalloc.h"

/*
 * Add tests.
 */
int priv_imalloc_tests(int (*init_suite)(void), int (*clean_suite)(void));

#endif
