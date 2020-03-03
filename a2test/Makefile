
CC = clang++ -O2 -Wno-deprecated -std=c++11 

tag = -i

ifdef linux
tag = -n
endif

test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o LoserTree.o BigQ.o Handler.o DBFile.o Pipe.o y.tab.o lex.yy.o test.o
	$(CC) -o test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o LoserTree.o BigQ.o Handler.o DBFile.o Pipe.o y.tab.o lex.yy.o test.o -ll -lpthread
	
a1test.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o Handler.o DBFile.o Pipe.o y.tab.o lex.yy.o a1-test.o
	$(CC) -o a1test.out Record.o Comparison.o ComparisonEngine.o Schema.o File.o Handler.o DBFile.o Pipe.o y.tab.o lex.yy.o a1-test.o -ll

a2Tests.out: Record.o Comparison.o ComparisonEngine.o Schema.o File.o LoserTree.o BigQ.o Handler.o DBFile.o Pipe.o y.tab.o lex.yy.o a2Tests.o 
	$(CC) -o a2Tests Record.o Comparison.o ComparisonEngine.o Schema.o File.o LoserTree.o BigQ.o Handler.o DBFile.o Pipe.o y.tab.o lex.yy.o a2Tests.o -ll -lpthread -lgtest	

test.o: test.cc
	$(CC) -g -c test.cc

a1-test.o: a1-test.cc
	$(CC) -g -c a1-test.cc

Comparison.o: Comparison.cc
	$(CC) -g -c Comparison.cc

a2Tests.o: a2Tests.cc
	$(CC) -g -c a2Tests.cc

ComparisonEngine.o: ComparisonEngine.cc
	$(CC) -g -c ComparisonEngine.cc
	
Pipe.o: Pipe.cc
	$(CC) -g -c Pipe.cc

LoserTree.o: LoserTree.cc
	$(CC) -g -c LoserTree.cc

BigQ.o: BigQ.cc
	$(CC) -g -c BigQ.cc

Handler.o: Handler.cc
	$(CC) -g -c Handler.cc

DBFile.o: DBFile.cc
	$(CC) -g -c DBFile.cc

File.o: File.cc
	$(CC) -g -c File.cc

Record.o: Record.cc
	$(CC) -g -c Record.cc

Schema.o: Schema.cc
	$(CC) -g -c Schema.cc
	
y.tab.o: Parser.y
	yacc -d Parser.y
	sed $(tag) -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" y.tab.c
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
	rm -f tempFile.bin
