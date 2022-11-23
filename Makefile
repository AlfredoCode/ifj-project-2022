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

# TESTS
test: htab.o $(testFile).o parser.o main.o scanner.o
	$(CC) $(CFLAGS) $^ -o $@ 

test_run: test
	./test 2>/dev/null

test_verbose: test
	./test

# FILES
$(testFile).o: $(testFile).c

<<<<<<< HEAD
symtable.o: symtable.c

expr_stack.o: expr_stack.c

expr_parser.o: expr_parser.c

error.o: error.c

scanner.o: scanner.c

parser.o: parser.c

=======
parser.o: parser.c

main.o: main.c

scanner.o: scanner.c

>>>>>>> Makefile changes
# CLEANUP
clean:
	@rm -f $(testFile) *.o
	@rm -f ./Testing

clear: clean
