
CC = clang++ -O2 -Wno-deprecated -std=c++11 

tag = -i

ifdef linux
tag = -n
endif

all: main test1.out test.out sortedfiletest

main: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o main.o
	$(CC) -o main Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o main.o -ll -lpthread

test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o  LoserTree.o BigQ.o GenericDBFile.o Pipe.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o test.o
	$(CC) -o test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o  LoserTree.o BigQ.o GenericDBFile.o Pipe.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o test.o -ll -lpthread
	
test1.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o test1.o
	$(CC) -o test1.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o test1.o -ll -lpthread

a2test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o GenericDBFile.o Pipe.o LoserTree.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o a2-test.o
	$(CC) -o a2test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o BigQ.o GenericDBFile.o Pipe.o LoserTree.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o a2-test.o -ll -lpthread

a1test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o a1-test.o
	$(CC) -o a1test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o a1-test.o -ll -lpthread

sortedfiletest: Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o sortedfiletest.o
	$(CC) -o sortedfiletest Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o y.tab.o lex.yy.o sortedfiletest.o -ll -lpthread -lgtest

sortedfiletest.o: SortedFileTest.cc
	$(CC) -g -c SortedFileTest.cc

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

LoserTree.o : LoserTree.cc
	$(CC) -g -c LoserTree.cc

GenericDBFile.o: GenericDBFile.cc
	$(CC) -g -c GenericDBFile.cc

DBFile.o: DBFile.cc
	$(CC) -g -c DBFile.cc

HeapFile.o: HeapFile.cc
	$(CC) -g -c HeapFile.cc

HeapFileHandler.o: HeapFileHandler.cc
	$(CC) -g -c HeapFileHandler.cc

SortedFile.o: SortedFile.cc
	$(CC) -g -c SortedFile.cc

SortedFileHandler.o: SortedFileHandler.cc
	$(CC) -g -c SortedFileHandler.cc

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

	
