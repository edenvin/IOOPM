#include "memory_tests.h"

void testTEST(void) {
  CU_ASSERT(1);
}

void testMORETEST(void) {
  CU_FAIL("HAHA YOU SUCK!");
}


/*
 * Add tests to suites.
 */
int memory_tests(int (*init_suite)(void), int (*clean_suite)(void)) {
  // Add suites
  CU_pSuite memory_suite = CU_add_suite("Memory Suite", init_suite, clean_suite);
  if (NULL == memory_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  // Add tests
  if (
    (NULL == CU_add_test(memory_suite, "test of test()", testTEST)) ||
    (NULL == CU_add_test(memory_suite, "test of more_tests()", testMORETEST))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
