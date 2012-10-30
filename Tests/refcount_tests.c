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
  // PLACEHOLDER
  Memory test = malloc(10);
  // PLACEHOLDER
  char *t = refcount_to_object(malloc(10 * sizeof(char)+ sizeof(int)));
  strcpy(t,"Jultomte");
  *object_to_refcount(t) = 3;
  CU_ASSERT(release(test, t) == 2);
  CU_ASSERT(release(test, t) == 1);
  CU_ASSERT(release(test, t) == 0);
  CU_ASSERT(t == NULL);
  // PLACERHOLDER
  free(test);
  // PLACEHOLDER
}

void test_count(void) {
  // PLACEHOLDER
  Memory test = malloc(10);
  // PLACEHOLDER
  char *t = refcount_to_object(malloc(10 * sizeof(char)+ sizeof(int)));
  strcpy(t,"tjenixen");
  *object_to_refcount(t) = 3;
  CU_ASSERT(count(t) == 3);
  retain(t);
  release(test, t);
  CU_ASSERT(count(t) == 3);
  // PLACEHOLDER
  free(test);
  // PLACEHOLDER
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
