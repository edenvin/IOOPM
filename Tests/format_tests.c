#include "format_tests.h"

void test_format_string_to_size(void) {
  CU_ASSERT(format_string_to_size("***d") == (chunk_size) (3 * sizeof(void*) + sizeof(int)));
  CU_ASSERT(format_string_to_size("3i4f") == (chunk_size) (5 * sizeof(int) + 4 * sizeof(float)));
}

void testMORETEST_format(void) {
  CU_FAIL("HAHA YOU SUCK!");
}

void 


/*
 * Add tests to suites.
 */
int format_tests(int (*init_suite)(void), int (*clean_suite)(void)) {
  // Add suites
  CU_pSuite format_suite = CU_add_suite("Format Suite", init_suite, clean_suite);
  if (NULL == format_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  // Add tests
  if (
    (NULL == CU_add_test(format_suite, "test of test()", testTEST_format)) ||
    (NULL == CU_add_test(format_suite, "test of more_tests()", testMORETEST_format))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
