#include "gc_tests.h"
#include "../rootset/rootset.h"
#include <stdio.h>

void zero_stack_above(void) {
  int memory[100];
  
  for (int i = 0; i < 100; i++) {
    memory[i] = 0;
  }
}

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
  
  zero_stack_above();
  CU_ASSERT(collect((Memory) mem) == 0);
  
  priv_free((Memory) mem,p0);
  priv_free((Memory) mem,p1);
  priv_free((Memory) mem,p2);
  priv_free((Memory) mem,p3);
}

/*
 * Test when there is garbage to collect.
 */
 void test_collect_2(void) {
  Managed mem = (Managed) iMalloc(sizeof(int)*4, GCD + ADDRESS);
  
  int *p0 = mem->alloc((Memory) mem, sizeof(int));
  int *p1 = mem->alloc((Memory) mem, sizeof(int));
  int *p2 = mem->alloc((Memory) mem, sizeof(int));
  int *p3 = mem->alloc((Memory) mem, sizeof(int));

  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
  p3 = NULL;
  
  zero_stack_above();
  CU_ASSERT(collect((Memory) mem) == 1);
  
  priv_free((Memory) mem,p0);
  priv_free((Memory) mem,p1);
  priv_free((Memory) mem,p2);
}

  /*
 * Test when there is no garbage to collect but we cant reach all posts from  * the stack.
 */
 void test_collect_3(void) {
  Managed mem = (Managed) iMalloc(sizeof(void*)*4, GCD + ADDRESS);
  void *p0 = mem->alloc((Memory) mem, sizeof(void*));
  void *p1 = mem->alloc((Memory) mem, sizeof(void*));
  void *p2 = mem->alloc((Memory) mem, sizeof(void*));
  void *p3 = mem->alloc((Memory) mem, sizeof(void*));
  *(int*)p0 = 0;
  *(int*)p1 = 1;
  *(int*)p3 = 3;
  *(void**)p2 = p3;
   p3 = NULL;
  CU_ASSERT(collect((Memory) mem) == 0);
  priv_free((Memory) mem,p0);
  priv_free((Memory) mem,p1);
  priv_free((Memory) mem,p2);
}

/*
 * Test when we have garbage but it can't be reached without traversing the    * heap. 
 */
 void test_collect_4(void) {
  Managed mem = (Managed) iMalloc(sizeof(int)*4, GCD + ADDRESS);
  int *p0 = mem->alloc((Memory) mem, sizeof(int));
  int *p1 = mem->alloc((Memory) mem, sizeof(int));
  int *p2 = mem->alloc((Memory) mem, sizeof(int));
  int *p3 = mem->alloc((Memory) mem, sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
  *p3 = 3;
   p2 = NULL;
   p3 = NULL;
  CU_ASSERT(collect((Memory) mem) == 2);
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
    (NULL == CU_add_test(gc_suite, "test of collect() 1", test_collect_1)) ||
    (NULL == CU_add_test(gc_suite, "test of collect() 2", test_collect_2)) ||
    (NULL == CU_add_test(gc_suite, "test of collect() 3", test_collect_3)) ||
    (NULL == CU_add_test(gc_suite, "test of collect() 4", test_collect_4))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
/*
 * Test when there is no garbage to collect.
 */
 /*void test_collect_1(void) {  
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
}*/
