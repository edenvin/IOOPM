#include "priv_imalloc_tests.h"

void test_conversion_functions(void) {
  chunk_size memsiz = 1 Mb;
  Priv_mem new_mem = malloc(sizeof(priv_mem));
  new_mem->start = malloc(memsiz);
  new_mem->end = new_mem->start + memsiz - 1;
  new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
  new_mem->functions.manual.alloc        = &manual_alloc;
  new_mem->functions.manual.avail        = &avail;
  new_mem->functions.manual.free         = &priv_free;

  Manual new_manual = (Manual) priv_to_style((Priv_mem) new_mem);

  CU_ASSERT(new_manual->alloc == &manual_alloc);
  CU_ASSERT(new_manual->avail == &avail);
  CU_ASSERT(new_manual->free  == &priv_free);

  Priv_mem compare_priv = (Priv_mem) style_to_priv((Memory) new_manual);

  CU_ASSERT(new_mem->start == compare_priv->start);
  CU_ASSERT(new_mem->end   == compare_priv->end);
  CU_ASSERT(new_mem->lists == compare_priv->lists);
  CU_ASSERT(new_mem->functions.manual.alloc == compare_priv->functions.manual.alloc);
  CU_ASSERT(new_mem->functions.manual.avail == compare_priv->functions.manual.avail);
  CU_ASSERT(new_mem->functions.manual.free  == compare_priv->functions.manual.free);

  free_lists(new_mem->lists);
  free(new_mem->start);
  free(new_mem);
}

void test_priv_imalloc(void) {
  Priv_mem new_mem = style_to_priv(priv_imalloc(1 Mb, REFCOUNT + GCD + ADDRESS));
  CU_ASSERT(new_mem->end == new_mem->start + 1048575);
  CU_ASSERT(new_mem->lists->freelist->size == 1048576);
  CU_ASSERT(new_mem->lists->freelist->next == NULL);
  CU_ASSERT(new_mem->lists->alloclist == NULL);
  CU_ASSERT(new_mem->lists->sort_style == 4);
  CU_ASSERT(new_mem->functions.managed.rc.retain == &retain);
  CU_ASSERT(new_mem->functions.managed.rc.release == &release);
  CU_ASSERT(new_mem->functions.managed.rc.count == &count);
  CU_ASSERT(new_mem->functions.managed.gc.alloc == &typed_alloc);
  CU_ASSERT(new_mem->functions.managed.gc.collect == &collect);

  free_lists(new_mem->lists);
  free(new_mem->start);
  free(new_mem);
}

/*void test_manual_alloc(void) {
  Priv_mem new_mem = style_to_priv(priv_imalloc(1 Mb, REFCOUNT + GCD + ADDRESS));

}*/

void test_managed_alloc(void) {
  // Test refcount.
  Managed mem = (Managed) iMalloc(1 Mb, REFCOUNT + ASCENDING_SIZE);
  void *tmp = mem->alloc((Memory) mem, 1 Kb);
  
  Priv_mem priv_mem  = style_to_priv((Memory) mem);
  CU_ASSERT(alloclist(priv_mem) != NULL);
  CU_ASSERT(alloclist(priv_mem)->size == 1 Kb + sizeof(int));
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
    (NULL == CU_add_test(priv_imalloc_suite, "test of conversion functions: priv_to_style and style_to_priv", test_conversion_functions)) ||
    (NULL == CU_add_test(priv_imalloc_suite, "test of test_managed_alloc()", test_managed_alloc)) ||
    (NULL == CU_add_test(priv_imalloc_suite, "test of priv_imalloc()", test_priv_imalloc))
    ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
