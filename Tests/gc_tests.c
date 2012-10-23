#include "gc_tests.h"

void testTEST_gc(void) {
  CU_ASSERT(1);
}

void testMORETEST_gc(void) {
  CU_ASSERT(/*ÄR UTTRYCKET SANT*/)
  CU_FAIL("HAHA YOU SUCK!");
}


/*
 * Add tests to suites.
 */
int memory_tests(int (*init_suite)(void), int (*clean_suite)(void)) {
  // Add suites
  CU_pSuite gc_suite = CU_add_suite("gc Suite", init_suite, clean_suite);
  if (NULL == gc_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  // Add tests
  if (
    (NULL == CU_add_test(gc_suite, "test of test()", testTEST_gc)) ||
    (NULL == CU_add_test(gc_suite, "test of more_tests()", testMORETEST_gc))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
