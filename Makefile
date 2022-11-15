#
# =================================================== #
# Name:       Makefile                                # 
# Authors:    xsafar27                                #
# Last modif: 10/28/2022                              #
# =================================================== #
#

testFile := run_test
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic -g -O0 

# TESTS

test: $(testFile).o symtable.o expr_stack.o error.o expr_parser.o
	$(CC) $(CFLAGS) $^ -o $@ -l cmocka -L /usr/bin/lib

test_run: test
	./test 2>/dev/null

test_verbose: test
	./test

# FILES
$(testFile).o: $(testFile).c

symtable.o: symtable.c

expr_stack.o: expr_stack.c

expr_parser.o: expr_parser.c

error.o: error.c

# CLEANUP
clean:
	@rm -f $(testFile) *.o
	@rm -f ./Testing

clear: clean
