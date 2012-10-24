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
}

void test_memory_size(void) {
  Chunk chunk = new_chunklist(NULL, 2 Kb);
  
  CU_ASSERT(memory_size(chunk) == 2*1024);
}

void test_set_memory_mark(void) {
  Chunk chunk = NULL;
  CU_ASSERT(memory_is_marked(chunk) == FALSE);
  
  chunk = new_chunklist(NULL, 10);
  CU_ASSERT(!memory_is_marked(chunk));
  
  set_memory_mark(chunk, FALSE);
  CU_ASSERT(!memory_is_marked(chunk));
  
  set_memory_mark(chunk, TRUE);
  CU_ASSERT(memory_is_marked(chunk));
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
    (NULL == CU_add_test(memory_suite, "test of new_chunklist()", test_new_chunklist)) ||
    (NULL == CU_add_test(memory_suite, "test of memory_size()", test_memory_size)) ||
    (NULL == CU_add_test(memory_suite, "test of set_memory_mark()", test_set_memory_mark))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
