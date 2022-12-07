#
# =================================================== #
# Name:       Makefile                                # 
# Authors:    xsafar27                                #
# Last modif: 12/06/2022                              #
# =================================================== #
#

testFile = parser
packageName = xmoskv01
CC = gcc
file = testfiles/PASS/string.php
CFLAGS=-std=c99 -Wall -Wextra -pedantic -g


all: expr_stack.o expr_parser.o  error.o scanner.o parser.o symtable.o  main.o generator.o expr_list.o ins_list.o
	$(CC) $(CFLAGS) $^ -o ifj22 

run: all
	@./ifj22 < $(file) > out.ifj
	@./ic22int out.ifj

pack:
	tar -cvzf $(packageName).tgz *.c *.h Makefile rozdeleni dokumentace.pdf


# TESTS
test: $(testFile).o  expr_stack.o expr_parser.o  error.o scanner.o parser.o symtable.o  main.o
	$(CC) $(CFLAGS) $^ -o $@ 


test: $(testFile).o symtable.o expr_stack.o error.o expr_parser.o scanner.o parser.o

gen: generator.o generator

test_run: test
	./test 2>/dev/null

test_verbose: test
	./test

# FILES
$(testFile).o: $(testFile).c

symtable.o: symtable.c

expr_stack.o: expr_stack.c

expr_parser.o: expr_parser.c

expr_list.o: expr_list.c

error.o: error.c

scanner.o: scanner.c

parser.o: parser.c

generator.o: generator.c

main.o: main.c

generator.o: generator.c

ins_list.o: ins_list.c


# CLEANUP
clean:
	@rm -f $(testFile) *.o
	@rm -f ./Testing

clear: clean
