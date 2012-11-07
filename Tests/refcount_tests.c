#include "refcount_tests.h"

void test_retain(void) {
  char *t = refcount_to_object(malloc(10 * sizeof(char) + sizeof(int)));
  strcpy(t,"Hejhopp");
  *object_to_refcount(t) = 1;
  CU_ASSERT(retain(t) == 2);
  *object_to_refcount(t) = 7;
  CU_ASSERT(retain(t) == 8);
}

void test_release(void) {
  Managed mem = (Managed) priv_imalloc(1 Mb, REFCOUNT + ASCENDING_SIZE);
  Priv_mem new_mem = style_to_priv((Memory) mem);
  char *t = (mem->alloc((Memory) mem, 10 * sizeof(char) + sizeof(int)));
  strcpy(t,"Jultomte");
  *object_to_refcount(t) = 3;
  CU_ASSERT(memory_start(search_memory(t, alloclist(new_mem), FALSE)) == object_to_refcount(t));
  CU_ASSERT(release((Memory) mem, t) == 2);
  CU_ASSERT(release((Memory) mem, t) == 1);
  CU_ASSERT(release((Memory) mem, t) == 0);
  CU_ASSERT(search_memory(t, alloclist(new_mem), FALSE) == NULL);

  free_lists(new_mem->lists);
  free(new_mem->as->start);
  free(new_mem);
}

void test_count(void) {
  Managed mem = (Managed) priv_imalloc(1 Mb, REFCOUNT + ASCENDING_SIZE);
  Priv_mem new_mem = style_to_priv((Memory) mem);
  char *t = mem->alloc((Memory) mem, 10 * sizeof(char)+ sizeof(int));
  strcpy(t,"tjenixen");
  *object_to_refcount(t) = 3;
  CU_ASSERT(count(t) == 3);
  retain(t);
  CU_ASSERT(count(t) == 4);
  release((Memory) mem, t);
  CU_ASSERT(count(t) == 3);

  free_lists(new_mem->lists);
  free(new_mem->as->start);
  free(new_mem);
}

/*
 * Add tests to suites.
 */
int refcount_tests(int (*init_suite)(void), int (*clean_suite)(void)) {
  // Add suites
  CU_pSuite refcount_suite = CU_add_suite("Refcount Suite", init_suite, clean_suite);
  if (NULL == refcount_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  // Add tests
  if (
    (NULL == CU_add_test(refcount_suite, "test of retain()", test_retain)) ||
    (NULL == CU_add_test(refcount_suite, "test of release()", test_release)) ||
    (NULL == CU_add_test(refcount_suite, "test of count()", test_count))
    ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
