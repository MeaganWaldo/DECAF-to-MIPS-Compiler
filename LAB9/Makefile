#  
#  Lab 9 Makefile
#  Meagan Waldo 
#  May 06, 2021
#  
#  What is the purpose: This makefile is used to compile all dependencies and create an executable file. This makefile also cleans up the directory. 
#

all:    lab9

lab9:   lab9.l lab9.y ast.c ast.h emit.c emit.h symtable.c symtable.h
	yacc -d lab9.y
	lex lab9.l
	gcc lex.yy.c y.tab.c ast.c emit.c symtable.c -o lab9 -lm

clean:
	rm -f lab9
