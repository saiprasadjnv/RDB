
CC = clang++ -O2 -Wno-deprecated

tag = -i

ifdef linux
tag = -n
endif

all: main a2test.out test1.out test.out

main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o main.o
	$(CC) -o main Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o main.o -ll

test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o Pipe.o y.tab.o lex.yy.o test.o
	$(CC) -o test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o Pipe.o y.tab.o lex.yy.o test.o -ll -lpthread
	
test1.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o test1.o
	$(CC) -o test1.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o test1.o -ll

a2test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o Pipe.o y.tab.o lex.yy.o a2-test.o
	$(CC) -o a2test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o Pipe.o y.tab.o lex.yy.o a2-test.o -ll -lpthread

a1test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o HeapFileHandler.o HeapFile.o DBFile.o Pipe.o y.tab.o lex.yy.o a1-test.o
	$(CC) -o a1test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.oHeapFileHandler.o HeapFile.o DBFile.o Pipe.o y.tab.o lex.yy.o a1-test.o -ll

test.o: test.cc
	$(CC) -g -c test.cc

test1.o: test1.cc
	$(CC) -g -c test1.cc

a2-test.o: a2-test.cc
	$(CC) -g -c a2-test.cc

a1-test.o: a1-test.cc
	$(CC) -g -c a1-test.cc

Comparison.o: Comparison.cc
	$(CC) -g -c Comparison.cc
	
ComparisonEngine.o: ComparisonEngine.cc
	$(CC) -g -c ComparisonEngine.cc
	
Pipe.o: Pipe.cc
	$(CC) -g -c Pipe.cc

BigQ.o: BigQ.cc
	$(CC) -g -c BigQ.cc

GenericDBFile.o: GenericDBFile.cc
	$(CC) -g -c GenericDBFile.cc

DBFile.o: DBFile.cc
	$(CC) -g -c DBFile.cc


HeapFile.o: HeapFile.cc
	$(CC) -g -c HeapFile.cc

HeapFileHandler.o: HeapFileHandler.cc
	$(CC) -g -c HeapFileHandler.cc

File.o: File.cc
	$(CC) -g -c File.cc

Record.o: Record.cc
	$(CC) -g -c Record.cc

Schema.o: Schema.cc
	$(CC) -g -c Schema.cc

main.o: main.cc
	$(CC) -g -c main.cc
	
y.tab.o: Parser.y
	yacc -d Parser.y
	sed $(tag) -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/"  y.tab.c
	g++ -c y.tab.c

lex.yy.o: Lexer.l
	lex  Lexer.l
	gcc  -c lex.yy.c

clean: 
	rm -f *.o
	rm -f *.out
	rm -f y.tab.c
	rm -f lex.yy.c
	rm -f y.tab.h
