testFile = run_test
CC = gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon -g

# TESTS
test:
	$(CC) $(CFLAGS) htab.c $(testFile).c -o $(testFile) -l cmocka -L /usr/bin/lib


test_run: test
	./$(testFile)

# CLEANUP
clean:
	@rm -f $(testFile) *.o

clear: clean
