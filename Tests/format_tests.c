#include "format_tests.h"

void test_format_string_to_size(void) {
  chunk_size test1 = format_string_to_size("***d");
  chunk_size test2 = format_string_to_size("3i4f");
  chunk_size test3 = format_string_to_size("6f4i3l1*d9c");
  chunk_size test4 = format_string_to_size("55");
  chunk_size test5 = format_string_to_size("");
  CU_ASSERT(test1 == (chunk_size) (3 * sizeof(void*) + sizeof(double)));
  CU_ASSERT(test2 == (chunk_size) (3 * sizeof(int) + 4 * sizeof(float)));
  CU_ASSERT(test3 == (chunk_size) (6 * sizeof(float) + 4 * sizeof(int) + 3 * sizeof(long)
   + sizeof(void*) + sizeof(double) + 9 * sizeof(char)));
  CU_ASSERT(test4 == (chunk_size) (55 * sizeof(char)));
  CU_ASSERT(test5 == 0);
}

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
    (NULL == CU_add_test(format_suite, "test of test()", test_format_string_to_size))
    ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
