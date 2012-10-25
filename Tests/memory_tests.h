#ifndef __memory_tests_h
#define __memory_tests_h
/*
 * memory_tests.h
 *
 * This file contains unit tests for the memory part of the imalloc allocator.
 *
 */

#include "CUnit/Basic.h"
#include "../memory.h"
#include "../memory_priv.h"

/*
 * Add tests.
 */
int memory_tests(int (*init_suite)(void), int (*clean_suite)(void));

#endif
