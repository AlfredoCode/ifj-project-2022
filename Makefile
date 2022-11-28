#
# =================================================== #
# Name:       Makefile                                # 
# Authors:    xsafar27                                #
# Last modif: 10/28/2022                              #
# =================================================== #
#

testFile = parser
CC = gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g

all: expr_stack.o expr_parser.o  error.o scanner.o parser.o symtable.o  main.o
	$(CC) $(CFLAGS) $^ -o ifj22 

# TESTS
test: $(testFile).o  expr_stack.o expr_parser.o  error.o scanner.o parser.o symtable.o  main.o
	$(CC) $(CFLAGS) $^ -o $@ 

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

scanner.o: scanner.c

parser.o: parser.c

main.o: main.c

generator.o: generator.c

# CLEANUP
clean:
	@rm -f $(testFile) *.o
	@rm -f ./Testing

clear: clean
