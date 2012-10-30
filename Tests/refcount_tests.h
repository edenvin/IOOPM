#ifndef __refcount_tests_h
#define __refcount_tests_h
/*
 * refcount_tests.h
 *
 * This file contains unit tests for the memory part of the imalloc allocator.
 *
 */

#include "CUnit/Basic.h"
#include "../refcount.h"
#include "../priv_imalloc.h"
#include "../memory_priv.h"

/*
 * Add tests.
 */
int refcount_tests(int (*init_suite)(void), int (*clean_suite)(void));

#endif
