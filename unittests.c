#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "imalloc.h"
#include "CUnit/Basic.h"
#include "Tests/memory_tests.h"
#include "Tests/format_tests.h"

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

int main()
{ 
  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add the tests to the suite */
  int error;
  error = memory_tests(&init_suite, &clean_suite);
  if (error)
    return error;
  error = format_tests(&init_suite, &clean_suite);
  if (error)
    return error;
  
  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
