.PHONY: clean test

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
LIBS += -ll
endif

ifeq ($(UNAME), Linux)
LIBS += -lfl
endif

all: hash.o parser.tab.o lex.yy.o ast.o semantic.o main.o
	@gcc -Wall hash.o lex.yy.o parser.tab.o main.o ast.o semantic.o tacs.o -o etapa5

main.o:
	@gcc -c main.c hash.c ast.c tacs.c

hash.o:
	@gcc -c hash.c

ast.o:
	@gcc -c ast.c

semantic.o:
	@gcc -c semantic.c

tacs.o:
	@gcc -c tacs.c

parser.tab.o:
	@bison -v --report=all -d parser.y
	@gcc -c parser.tab.c

lex.yy.o:
	@flex --header-file=lex.yy.h scanner.l
	@gcc -c lex.yy.c

clean:
	@rm -rf lex.yy.* parser.tab.* main.o hash.o ast.o semantic.o tacs.o parser.output etapa4 results.txt errors.txt output1.txt output2.txt

test: clean all
	@./etapa5 test2.txt

testfull: clean all
	@./etapa5 test.txt
