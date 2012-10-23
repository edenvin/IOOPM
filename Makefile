all: test

# Tex settings
C_COMPILER   = gcc
C_OPTIONS    = -ggdb -Wall -std=c99

# Clean settings
GEN_EXTENSIONS = *.o *.out

clean:
	rm -rf $(GEN_EXTENSIONS) *.orig *.dSYM

# Tests to load
TEST_FILES = Tests/memory_tests.c

# Ordinary files
FILES = memory.c

test: unittests.c
	$(C_COMPILER) $(C_OPTIONS) unittests.c $(TEST_FILES) $(FILES) -o unittests -lcunit
	./unittests
