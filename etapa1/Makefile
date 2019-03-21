UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
LIBS += -ll
endif

ifeq ($(UNAME), Linux)
LIBS += -lfl
endif

all: lex.yy.o
	gcc -Wall main.c lex.yy.o $(LIBS) -o etapa1

lex.yy.o:
	flex --header-file=lex.yy.h scanner.l
	gcc -c lex.yy.c

clean:
	rm lex.yy.* etapa1
