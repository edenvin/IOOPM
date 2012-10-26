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
  
  ascending->freelist = new_chunk(NULL, 8, memory_freelist(ascending));
  ascending->freelist = new_chunk(NULL, 4, memory_freelist(ascending));
  ascending->freelist = new_chunk(NULL, 2, memory_freelist(ascending));
  ascending->freelist = new_chunk(NULL, 1, memory_freelist(ascending));
  
  descending->freelist = new_chunk(NULL, 2, memory_freelist(descending));
  descending->freelist = new_chunk(NULL, 4, memory_freelist(descending));
  descending->freelist = new_chunk(NULL, 8, memory_freelist(descending));
  descending->freelist = new_chunk(NULL, 16, memory_freelist(descending));
  
  address->freelist = new_chunk(address_pointers+3, 1, memory_freelist(address));
  address->freelist = new_chunk(address_pointers+2, 4, memory_freelist(address));
  address->freelist = new_chunk(address_pointers+1, 2, memory_freelist(address));
  address->freelist = new_chunk(address_pointers+0, 8, memory_freelist(address));
  
  /*
   *  ascending = [ 1 |  2  |   4   |     8     |       16        ]
   * descending = [       16        |     8     |   4   |  2  | 1 ]
   *    address = [     8     |  2  |   4   | 1 |       16        ]
   */
  
  // Test claiming a whole free chunk. Also tests the first chunk.
  Chunk asc1 = claim_memory(1, ascending);
  CU_ASSERT(asc1 != NULL && asc1->size == 1);
  CU_ASSERT(memory_freelist(ascending)->size == 2);
  CU_ASSERT(memory_alloclist(ascending) == asc1);
  Chunk desc1 = claim_memory(16, descending);
  CU_ASSERT(desc1 != NULL && desc1->size == 16);
  CU_ASSERT(memory_freelist(descending)->size == 8);
  CU_ASSERT(memory_alloclist(descending) == desc1);
  Chunk addr1 = claim_memory(8, address);
  CU_ASSERT(addr1 != NULL && addr1->size == 8);
  CU_ASSERT(memory_freelist(address)->size == 2);
  CU_ASSERT(memory_alloclist(address) == addr1);
  
  /*
   *  ascending = [  2  |   4   |     8     |       16        ]
   * descending = [     8     |   4   |  2  | 1 ]
   *    address = [  2  |   4   | 1 |       16        ]
   */
  
  // Test claiming a whole chunk from the end.
  Chunk asc2 = claim_memory(16, ascending);
  CU_ASSERT(asc2 != NULL && asc2->size == 16);
  CU_ASSERT(memory_freelist(ascending)->next->next->next == NULL);
  CU_ASSERT(memory_alloclist(ascending) == asc2);
  CU_ASSERT(memory_alloclist(ascending)->next == asc1);
  Chunk addr2 = claim_memory(16, address);
  CU_ASSERT(addr2 != NULL && addr2->size == 16);
  CU_ASSERT(memory_freelist(address)->next->next->next == NULL);
  CU_ASSERT(memory_alloclist(address) == addr2);
  CU_ASSERT(memory_alloclist(address)->next == addr1);
  
  /*
   *  ascending = [  2  |   4   |     8     ]
   * descending = [     8     |   4   |  2  | 1 ]
   *    address = [  2  |   4   | 1 ]
   */
  
  // Test claiming a chunk that is too large.
  Chunk asc3 = claim_memory(32, ascending);
  CU_ASSERT(asc3 == NULL);
  Chunk desc3 = claim_memory(32, descending);
  CU_ASSERT(desc3 == NULL);
  Chunk addr3 = claim_memory(32, address);
  CU_ASSERT(addr3 == NULL);
  
  /*
   *  ascending = [  2  |   4   |     8     ]
   * descending = [     8     |   4   |  2  | 1 ]
   *    address = [  2  |   4   | 1 ]
   */
  
  // Test sorting
  Chunk asc4 = claim_memory(3, ascending);
  CU_ASSERT(asc4 != NULL && asc4->size == 3);
  CU_ASSERT(ascending->freelist->size == 1);
  CU_ASSERT(ascending->freelist->next->size == 2);
  CU_ASSERT(ascending->freelist->next->next->size == 8);
  Chunk desc4 = claim_memory(5, descending);
  CU_ASSERT(desc4 != NULL && desc4->size == 5);
  CU_ASSERT(descending->freelist->size == 4);
  CU_ASSERT(descending->freelist->next->size == 3);
  CU_ASSERT(descending->freelist->next->next->size == 2);
  CU_ASSERT(descending->freelist->next->next->next->size == 1);
  Chunk addr4 = claim_memory(3, address);
  CU_ASSERT(addr4 != NULL && addr4->size == 3);
  CU_ASSERT(address->freelist->size == 2);
  CU_ASSERT(address->freelist->next->size == 1);
  CU_ASSERT(address->freelist->next->next->size == 1);
  
  /*
   *  ascending = [ 1 |  2  |     8     ]
   * descending = [   4   |   3   |  2  | 1 ]
   *    address = [  2  | 1 | 1 ]
   */
  
}

void test_combine_adjecent(void) {
  int *ptr = malloc(sizeof(int) * 10);
  Chunk new;
  Chunk chunk;
  
  Lists lists = create_lists(ptr+1, sizeof(int), ASCENDING_SIZE);
  
  chunk = new_chunk(ptr+8, sizeof(int), NULL);
  chunk = new_chunk(ptr+6, sizeof(int), chunk);
  chunk = new_chunk(ptr+3, 2*sizeof(int), chunk);
  lists->freelist->next = chunk;
  
  /*
   * [   ptr+1     ptr+3   ptr+6   ptr+8 ]
   * [ x | 1 | x |  2  | x | 1 | x | 1 | x ]
   */
  
  // Test combine with a chunk that lies after.
  new = new_chunk(ptr, sizeof(int), NULL);
  new = combine_adjecent(lists, new);
  CU_ASSERT(new->size == 2*sizeof(int));
  CU_ASSERT(new->start == ptr);
  CU_ASSERT(lists->freelist->start == ptr+3);
  free_chunk(new);
  
  /*
   * [ ptr+3   ptr+6   ptr+8 ]
   * [  2  | x | 1 | x | 1 | x ]
   */
   
  
  // Test combine with a chunk that lies before.
  new = new_chunk(ptr+9, sizeof(int), NULL);
  new = combine_adjecent(lists, new);
  CU_ASSERT(new->size == 2*sizeof(int));
  CU_ASSERT(new->start == ptr+8);
  CU_ASSERT(lists->freelist->next->next == NULL);
  free_chunk(new);
  
  /*
   * [ ptr+3   ptr+6 ]
   * [  2  | x | 1 ]
   */
   
  // Test combine with a chunk that lies before and after.
  new = new_chunk(ptr+5, sizeof(int), NULL);
  new = combine_adjecent(lists, new);
  CU_ASSERT(new->size == 4*sizeof(int));
  CU_ASSERT(new->start == ptr+3);
  CU_ASSERT(lists->freelist == NULL);
  free_chunk(new);
  
  /*
   * [ ]
   * [ ]
   */
   
  free_lists(lists);
  free(ptr);
}

void test_free_memory(void) {
  void *ptr = malloc(10);
  Lists lists = create_lists(ptr, 10, ASCENDING_SIZE);
  
  Chunk chunk1 = claim_memory(4, lists);
  Chunk chunk2 = claim_memory(6, lists);
  
  free_memory(chunk1, lists);
  CU_ASSERT(lists->freelist == chunk1);
  CU_ASSERT(lists->freelist->next == NULL);
  free_memory(chunk2, lists);
  CU_ASSERT(lists->freelist->size == 10);
  
  free(ptr);
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
    (NULL == CU_add_test(memory_suite, "test of search_memory()", test_search_memory)) ||
    (NULL == CU_add_test(memory_suite, "test of claim_memory()", test_claim_memory)) ||
    (NULL == CU_add_test(memory_suite, "test of combine_adjecent()", test_combine_adjecent)) ||
    (NULL == CU_add_test(memory_suite, "test of free_memory()", test_free_memory))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  
  return 0;
}
