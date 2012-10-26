#include "priv_imalloc_tests.h"

void test_priv_to_style(void) {
  chunk_size memsiz = 1 Mb;
  Priv_manual new_mem = malloc(sizeof(priv_manual));
  new_mem->start = malloc(memsiz);
  new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
  new_mem->functions.alloc        = &manual_alloc;
  new_mem->functions.avail        = &avail;
  new_mem->functions.free         = &manual_free;
  new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);

  Manual new_manual = priv_to_style((Priv_mem) new_mem);

  Manual compare_manual = malloc(sizeof(manual));
  compare_manual->alloc               = &manual_alloc;
  compare_manual->avail               = &avail;
  compare_manual->free                = &manual_free;

  CU_ASSERT(new_manual->alloc == compare_manual->alloc);
  CU_ASSERT(new_manual->avail == compare_manual->avail);
  CU_ASSERT(new_manual->free == compare_manual->free);
  
}

/*
 * Add tests to suites.
 */
int priv_imalloc_tests(int (*init_suite)(void), int (*clean_suite)(void)) {
  // Add suites
  CU_pSuite priv_imalloc_suite = CU_add_suite("priv_imalloc Suite", init_suite, clean_suite);
  if (NULL == priv_imalloc_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  // Add tests
  if (
    (NULL == CU_add_test(priv_imalloc_suite, "test of test()", test_priv_to_style))
    ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
