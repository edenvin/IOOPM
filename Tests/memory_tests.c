#include "memory_tests.h"

void test_memory_is_marked(void) {
  Chunk chunk = new_chunklist(NULL, 10 Kb);
  
  CU_ASSERT(!memory_is_marked(chunk));
}

void test_new_chunklist(void) {
  Chunk chunk = new_chunklist(NULL, 24 Kb);
  
  CU_ASSERT(chunk->start == NULL);
  CU_ASSERT(chunk->size == 24 Kb);
  CU_ASSERT(chunk->next == NULL);
  CU_ASSERT(chunk->mark == FALSE);
  CU_ASSERT(chunk->refcount == 0);
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
    (NULL == CU_add_test(memory_suite, "test of memory_is_marked()", test_memory_is_marked)) ||
    (NULL == CU_add_test(memory_suite, "test of new_chunklist()", test_new_chunklist))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
