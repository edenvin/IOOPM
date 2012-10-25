#include "memory_tests.h"

void test_memory_is_marked(void) {
  Chunk chunk = new_chunk(NULL, 1 Kb, NULL);
  
  CU_ASSERT(!memory_is_marked(chunk));
  
  free_chunk(chunk);
}

void test_create_lists(void) {
  Lists lists = create_lists(NULL, 24 Kb, 1);
  
  CU_ASSERT(lists->freelist->start == NULL);
  CU_ASSERT(lists->freelist->size == 24 Kb);
  CU_ASSERT(lists->freelist->next == NULL);
  CU_ASSERT(lists->freelist->mark == FALSE);
  
  free_lists(lists);
}

void test_memory_size(void) {
  Chunk chunk = new_chunk(NULL, 2 Kb, NULL);
  
  CU_ASSERT(memory_size(chunk) == 2*1024);
}

void test_set_memory_mark(void) {
  Chunk chunk = NULL;
  CU_ASSERT(memory_is_marked(chunk) == FALSE);
  
  chunk = new_chunk(NULL, 10, NULL);
  CU_ASSERT(!memory_is_marked(chunk));
  
  set_memory_mark(chunk, FALSE);
  CU_ASSERT(!memory_is_marked(chunk));
  
  set_memory_mark(chunk, TRUE);
  CU_ASSERT(memory_is_marked(chunk));
}

void test_search_memory(void) {
  int *ptr = malloc(sizeof(int));
  int *ptr2 = malloc(sizeof(int));
  
  Chunk haystack = new_chunk(NULL, sizeof(int), NULL);
  CU_ASSERT(!search_memory(ptr, haystack, TRUE));
  
  Chunk h1 = new_chunk(ptr, 2*sizeof(int), haystack);
  Chunk h2 = new_chunk(NULL, 3*sizeof(int), h1);
  Chunk h3 = new_chunk(ptr2, 4*sizeof(int), h2);
  Chunk h4 = new_chunk(NULL, 5*sizeof(int), h3);
  CU_ASSERT(search_memory(ptr, h4, TRUE) != NULL);
  CU_ASSERT(search_memory(ptr2 + 1, h4, TRUE) == NULL);
  CU_ASSERT(search_memory(ptr2 + 1, h4, FALSE) != NULL);
  
  free_chunklist(haystack);
}

void test_claim_memory(void) {
  void *address_pointers = malloc(5);
  
  Lists ascending = create_lists(NULL, 16, ASCENDING_SIZE);
  Lists descending = create_lists(NULL, 1, DESCENDING_SIZE);
  Lists address = create_lists(address_pointers+4, 16, ADDRESS);
  
  ascending->freelist = new_chunk(NULL, 8, memory_freelist(ascending)->next);
  ascending->freelist = new_chunk(NULL, 4, memory_freelist(ascending)->next);
  ascending->freelist = new_chunk(NULL, 2, memory_freelist(ascending)->next);
  ascending->freelist = new_chunk(NULL, 1, memory_freelist(ascending)->next);
  
  descending->freelist = new_chunk(NULL, 2, memory_freelist(descending)->next);
  descending->freelist = new_chunk(NULL, 4, memory_freelist(descending)->next);
  descending->freelist = new_chunk(NULL, 8, memory_freelist(descending)->next);
  descending->freelist = new_chunk(NULL, 16, memory_freelist(descending)->next);
  
  address->freelist = new_chunk(address_pointers+3, 1, memory_freelist(address)->next);
  address->freelist = new_chunk(address_pointers+2, 4, memory_freelist(address)->next);
  address->freelist = new_chunk(address_pointers+1, 2, memory_freelist(address)->next);
  address->freelist = new_chunk(address_pointers+0, 8, memory_freelist(address)->next);
  
  /*
   *  ascending = [ 1 |  2  |   4   |     8     |       16        ]
   * descending = [       16        |     8     |   4   |  2  | 1 ]
   *    address = [     8     |  2  |   4   | 1 |       16        ]
   */
  
  // Test claiming a whole free chunk. Also tests the first chunk.
  Chunk asc1 = claim_memory(1, ascending);
  CU_ASSERT(asc1 != NULL && asc1->size == 1);
  CU_ASSERT(memory_freelist(ascending)->size == 2);
  Chunk desc1 = claim_memory(16, descending);
  CU_ASSERT(desc1 != NULL && desc1->size == 16);
  CU_ASSERT(memory_freelist(descending)->size == 8);
  Chunk addr1 = claim_memory(8, address);
  CU_ASSERT(addr1 != NULL && addr1->size == 8);
  CU_ASSERT(memory_freelist(address)->size == 2);
  
  /*
   *  ascending = [  2  |   4   |     8     |       16        ]
   * descending = [     8     |   4   |  2  | 1 ]
   *    address = [  2  |   4   | 1 |       16        ]
   */
  
/*   Chunk asc2 = claim_memory(16, ascending);
   CU_ASSERT(asc1 != NULL && asc1->size == 16);
   CU_ASSERT(memory_freelist(ascending)->next->next->next == NULL);
   Chunk desc1 = claim_memory(1, descending);
   CU_ASSERT(desc1 != NULL && desc1->size == 1);
   CU_ASSERT(memory_freelist(descending)->size == 8);
   Chunk addr1 = claim_memory(8, address);
   CU_ASSERT(addr1 != NULL && addr1->size == 8);
   CU_ASSERT(memory_freelist(address)->size == 2);
 */ 
  
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
    (NULL == CU_add_test(memory_suite, "test of create_lists()", test_create_lists)) ||
    (NULL == CU_add_test(memory_suite, "test of memory_size()", test_memory_size)) ||
    (NULL == CU_add_test(memory_suite, "test of set_memory_mark()", test_set_memory_mark)) ||
    (NULL == CU_add_test(memory_suite, "test of search_memory()", test_search_memory))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
