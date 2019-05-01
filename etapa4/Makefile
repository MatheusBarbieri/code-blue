.PHONY: clean test

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
LIBS += -ll
endif

ifeq ($(UNAME), Linux)
LIBS += -lfl
endif

all: parser.tab.o lex.yy.o
	gcc -Wall main.c lex.yy.o parser.tab.o $(LIBS) -o etapa3

parser.tab.o:
	bison -v --report=all -d parser.y
	gcc -c parser.tab.c

lex.yy.o:
	flex --header-file=lex.yy.h scanner.l
	gcc -c lex.yy.c

test:
	python tests.py

clean:
	rm -rf lex.yy.* parser.tab.* parser.output etapa2 results.txt errors.txt
