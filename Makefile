all: test

# Tex settings
C_COMPILER   = gcc
C_OPTIONS    = -ggdb -Wall -std=c99

# Clean settings
GEN_EXTENSIONS = *.o *.out

clean:
	rm -rf $(GEN_EXTENSIONS) *.orig *.dSYM

# Tests to load
TEST_FILES = Tests/memory_tests.c Tests/format_tests.c Tests/refcount_tests.c Tests/priv_imalloc_tests.c

# Ordinary files
FILES = imalloc.c priv_imalloc.c format.c memory.c refcount.c gc.c

test: unittests.c
	$(C_COMPILER) $(C_OPTIONS) unittests.c $(TEST_FILES) $(FILES) -o unittests -lcunit
	./unittests

