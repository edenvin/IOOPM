#include "gc_tests.h"


/*
 * Test when there is no garbage to collect.
 */
 typedef struct priv_mem *Priv_mem; 

typedef struct {
  void* start;
  void* end;
  Lists lists;
  manual functions;
} priv_manual, *Priv_manual;

typedef struct {
  void* start;
  void* end;
  Lists lists;
  managed functions;
} priv_managed, *Priv_managed;

typedef union {
  priv_manual priv_manual;
  priv_managed priv_managed;
} priv_mem;
 void test_collect_1(void) {
   priv_mem *mem = MALLOC
   mem->start = 
   // MANUAL
  if (flags < 13) {
    Priv_manual new_mem = malloc(sizeof(priv_manual));
    new_mem->start = malloc(memsiz);
    new_mem->end = (void*) ((char*) (new_mem->start) + memsiz - 1);
    new_mem->functions.alloc        = &manual_alloc;
    new_mem->functions.avail        = &avail;
    new_mem->functions.free         = &priv_free;
    switch (flags) {
      case 9:  new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
      break;
      case 10: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
      break;
      case 12: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
      break;
  // MANAGED + GC
    } else if (flags < 37) {
      new_mem->functions.gc.alloc   = &typed_alloc;
      new_mem->functions.gc.collect = &collect;
      switch (flags) {
        case 33: new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
        break;
        case 34: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
        break;
        case 36: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
        break;
      }
  // MANAGED + REFCOUNT + GC
    } else {
      new_mem->functions.rc.retain  = &retain;
      new_mem->functions.rc.release = &release;
      new_mem->functions.rc.count   = &count;
      new_mem->functions.gc.alloc   = &typed_alloc;
      new_mem->functions.gc.collect = &collect;
      switch (flags) {
        case 49: new_mem->lists = create_lists(new_mem->start, memsiz, ASCENDING_SIZE);
        break;
        case 50: new_mem->lists = create_lists(new_mem->start, memsiz, DESCENDING_SIZE);
        break;
        case 52: new_mem->lists = create_lists(new_mem->start, memsiz, ADDRESS);
        break;
      }
    }
}




  /*managed mem = (managed) iMalloc(sizeof(int)*4, GC + ASCENDING_SIZE);
  int *p0 = mem->alloc(mem, sizeof(int));
  int *p1 = mem->alloc(mem, sizeof(int));
  int *p2 = mem->alloc(mem, sizeof(int));
  int *p3 = mem->alloc(mem, sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
  *p3 = 3;
  CU_ASSERT(collect(mem) == 0);
  free_memory(mem); //not made yet */



/*
 * Test when there is garbage to collect.
 */
 void test_collect_2(void) {
  managed mem = (managed) iMalloc(sizeof(int)*4, GC + ASCENDING_SIZE);
  int *p0 = mem->alloc(mem, sizeof(int));
  int *p1 = mem->alloc(mem, sizeof(int));
  int *p2 = mem->alloc(mem, sizeof(int));
  int *p3 = mem->alloc(mem, sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
   p3 = NULL;
  CU_ASSERT(collect(mem) == 1);
  free_memory(mem); //not made yet
}

/*
 * Test when there is no garbage to collect but we cant reach all posts from  * the stack.
 */
 void test_collect_3(void) {
  managed mem = (managed) iMalloc(sizeof(int)*4, GC + ASCENDING_SIZE);
  int *p0 = mem->alloc(mem, sizeof(int));
  int *p1 = mem->alloc(mem, sizeof(int));
  int *p2 = mem->alloc(mem, sizeof(int));
  int *p3 = mem->alloc(mem, sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p3 = 3;
  *p2 = p3;
   p3 = NULL;
  CU_ASSERT(collect(mem) == 0);
  free_memory(mem); //not made yet
}

/*
 * Test when we have garbage but it can't be reached without traversing the    * heap. 
 */
 void test_collect_4(void) {
  managed mem = (managed) iMalloc(sizeof(int)*4, GC + ASCENDING_SIZE);
  int *p0 = mem->alloc(mem, sizeof(int));
  int *p1 = mem->alloc(mem, sizeof(int));
  int *p2 = mem->alloc(mem, sizeof(int));
  int *p3 = mem->alloc(mem, sizeof(int));
  *p0 = 0;
  *p1 = 1;
  *p2 = 2;
  *p3 = 3;
   p2 = NULL;
   p3 = NULL;
  CU_ASSERT(collect(mem) == 1);
  free_memory(mem); //not made yet
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
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_1)) ||
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_2)) ||
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_3)) ||
    (NULL == CU_add_test(gc_suite, "test of collect()", test_collect_4))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
