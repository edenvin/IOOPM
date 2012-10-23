#include "gc_tests.h"

void test_collect(void) {
  managed mem = (managed) iMalloc(sizeOf(int)*4, GC + ASCENDING_SIZE);
  
  CU_ASSERT(collect (6) == 1);
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
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect)) ||
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
