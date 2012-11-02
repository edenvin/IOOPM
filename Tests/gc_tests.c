#include "gc_tests.h"


/*
 * Test when there is no garbage to collect.
 */
 void test_collect_1(void) {  
  Managed mem = (Managed) iMalloc(sizeof(int)*4, GCD + ADDRESS);
  int *p0 = mem->alloc((Memory) mem, sizeof(int));
  int *p1 = mem->alloc((Memory) mem, sizeof(int));
  int *p2 = mem->alloc((Memory) mem, sizeof(int));
  int *p3 = mem->alloc((Memory) mem, sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
  *p3 = 3;
  CU_ASSERT(collect((Memory) mem) == 0);
  p3 = NULL;
  CU_ASSERT(collect((Memory) mem) == 1);
  p3 = mem->alloc((Memory) mem, sizeof(void*));
  *(void**)p2 = p3;
  p3 = NULL;
  CU_ASSERT(collect((Memory) mem) == 0);
  p2 = NULL;
  p3 = NULL;
  priv_free((Memory) mem,p0);
  priv_free((Memory) mem,p1);
}

/*
 * Add tests to suites.
 */
int gc_tests(int (*init_suite)(void), int (*clean_suite)(void)) {
  // Add suites
  CU_pSuite gc_suite = CU_add_suite("gc Suite", init_suite, clean_suite);
  if (NULL == gc_suite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  // Add tests
  if (
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_1))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
