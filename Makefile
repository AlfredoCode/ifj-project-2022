#
# =================================================== #
# Name:       Makefile                                # 
# Authors:    xsafar27                                #
# Last modif: 10/28/2022                              #
# =================================================== #
#

testFile = run_test
CC = gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g -O0

# TESTS
test: htab.o $(testFile).o
	$(CC) $(CFLAGS) $^ -o $@ -l cmocka -L /usr/bin/lib

test_run: test
	./test 2>/dev/null

test_verbose: test
	./test

# FILES
htab.o: htab.c

$(testFile).o: $(testFile).c

# CLEANUP
clean:
	@rm -f $(testFile) *.o
	@rm -f ./Testing

clear: clean
