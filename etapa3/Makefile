.PHONY: clean test

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
LIBS += -ll
endif

ifeq ($(UNAME), Linux)
LIBS += -lfl
endif

all: hash.o parser.tab.o lex.yy.o ast.o main.o
	gcc -Wall hash.o lex.yy.o parser.tab.o main.o ast.o -o etapa3

main.o:
	gcc -c main.c hash.c ast.c

hash.o:
	gcc -c hash.c

ast.o:
	gcc -c ast.c

parser.tab.o:
	bison -v --report=all -d parser.y
	gcc -c parser.tab.c

lex.yy.o:
	flex --header-file=lex.yy.h scanner.l
	gcc -c lex.yy.c

test:
	./etapa3 test.txt output1.txt
	./etapa3 output1.txt output2.txt
	diff output1.txt output2.txt

clean:
	rm -rf lex.yy.* parser.tab.* main.o hash.o ast.o parser.output etapa3 results.txt errors.txt output1.txt output2.txt
