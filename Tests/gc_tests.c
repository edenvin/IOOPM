#include "gc_tests.h"


/*
 * Test when there is no garbage to collect.
 */
 void test_collect_1(void) {
  Memory mem = (Memory) iMalloc(sizeof(int)*4, GCD + ASCENDING_SIZE);
  void *p0 = mem->alloc(mem, sizeof(int));
  void *p1 = mem->alloc(mem, sizeof(int));
  void *p2 = mem->alloc(mem, sizeof(int));
  void *p3 = mem->alloc(mem, sizeof(int));
  *(int*)p0 = 0;
  *(int*)p1 = 1;
  *(int*)p2 = 2;
  *(int*)p3 = 3;
  CU_ASSERT(collect(mem) == 0);
  managed_free(mem,p0);
  managed_free(mem,p1);
  managed_free(mem,p2);
  managed_free(mem,p3);
}


/*
 * Test when there is garbage to collect.
 */
 /*void test_collect_2(void) {
  Managed mem = (Managed) iMalloc(sizeof(int)*4, GCD + ASCENDING_SIZE);
  int *p0 = mem->alloc(sizeof(int));
  int *p1 = mem->alloc(sizeof(int));
  int *p2 = mem->alloc(sizeof(int));
  int *p3 = mem->alloc(sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
   p3 = NULL;
  CU_ASSERT(collect(mem) == 1);
  managed_free(mem); //not made yet
}*/

/*
 * Test when there is no garbage to collect but we cant reach all posts from  * the stack.
 */
 /*void test_collect_3(void) {
  Managed mem = (Managed) iMalloc(sizeof(int)*4, GCD + ASCENDING_SIZE);
  int *p0 = mem->alloc(sizeof(int));
  int *p1 = mem->alloc(sizeof(int));
  int *p2 = mem->alloc(sizeof(int));
  int *p3 = mem->alloc(sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p3 = 3;
  *p2 = p3;
   p3 = NULL;
  CU_ASSERT(collect(mem) == 0);
  free_memory(mem); //not made yet
}*/

/*
 * Test when we have garbage but it can't be reached without traversing the    * heap. 
 */
 /*void test_collect_4(void) {
  Managed mem = (Managed) iMalloc(sizeof(int)*4, GCD + ASCENDING_SIZE);
  int *p0 = mem->alloc(sizeof(int));
  int *p1 = mem->alloc(sizeof(int));
  int *p2 = mem->alloc(sizeof(int));
  int *p3 = mem->alloc(sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
  *p3 = 3;
   p2 = NULL;
   p3 = NULL;
  CU_ASSERT(collect(mem) == 1);
  free_memory(mem); //not made yet
}
*/

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
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_1)) //||
   // (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_2)) ||
    //(NULL == CU_add_test(gc_suite, "test of collect()", test_collect_3)) ||
    //(NULL == CU_add_test(gc_suite, "test of collect()", test_collect_4))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
