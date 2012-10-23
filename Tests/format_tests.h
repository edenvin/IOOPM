#ifndef __format_tests_h
#define __format_tests_h
/*
 * format_tests.h
 *
 * This file contains unit tests for the memory part of the imalloc allocator.
 *
 */

#include "CUnit/Basic.h"
#include "../format.h"

/*
 * Add tests.
 */
int format_tests(int (*init_suite)(void), int (*clean_suite)(void));

#endif
