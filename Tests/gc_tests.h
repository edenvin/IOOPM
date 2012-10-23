#ifndef __gc_tests_h
#define __gc_tests_h
/*
 * gc_tests.h
 *
 * This file contains unit tests for the gc part of the imalloc allocator.
 *
 */

#include "CUnit/Basic.h"
#include "../imalloc.h"
#include "../priv_imalloc.h"

/*
 * Add tests.
 */
int gc_tests(int (*init_suite)(void), int (*clean_suite)(void));

#endif