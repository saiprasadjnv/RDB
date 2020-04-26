
CC = clang++ -O2 -Wno-deprecated -std=c++11 

tag = -i

ifdef linux
tag = -n
endif

classes= Record.o Comparison.o ComparisonEngine.o Schema.o File.o GenericDBFile.o Pipe.o LoserTree.o BigQ.o SortedFileHandler.o SortedFile.o HeapFileHandler.o HeapFile.o DBFile.o Function.o SelectFile.o SelectPipe.o Project.o DuplicateRemoval.o WriteOut.o Sum.o GroupBy.o JoinUtil.o Join.o Statistics.o y.tab.o yyfunc.tab.o lex.yy.o lex.yyfunc.o

all: a4-1.out a2test statisticstest

a4-1.out: $(classes) test.o
	$(CC) -o a4-1.out $(classes) test.o -ll -lpthread
	
a2-2test.out: $(classes) a2-2test.o
	$(CC) -o a2-2test.out $(classes) a2-2test.o -ll -lpthread
	
a2test.out: $(classes) a2-test.o
	$(CC) -o a2test.out $(classes) a2-test.o -ll -lpthread
	
a1test.out: $(classes) a1-test.o
	$(CC) -o a1test.out $(classes) a1-test.o -ll

a2test: $(classes) a2test.o
	$(CC) -o a2test $(classes) a2test.o -ll -lpthread

statisticstest: $(classes) Statisticstest.o 
	$(CC) -o statisticstest $(classes) Statisticstest.o  -ll -lpthread -lgtest

Statisticstest.o: Statisticstest.cc
	$(CC) -g -c Statisticstest.cc

Statistics.o: Statistics.cc
	$(CC) -g -c Statistics.cc

test.o: test.cc
	$(CC) -g -c test.cc
	
a2-2test.o: a2-2test.cc
	$(CC) -g -c a2-2test.cc

a2-test.o: a2-test.cc
	$(CC) -g -c a2-test.cc

a1-test.o: a1-test.cc
	$(CC) -g -c a1-test.cc

a2test.o: a2test.cc
	$(CC) -g -c a2test.cc

Comparison.o: Comparison.cc
	$(CC) -g -c Comparison.cc
	
ComparisonEngine.o: ComparisonEngine.cc
	$(CC) -g -c ComparisonEngine.cc
	
DBFile.o: DBFile.cc
	$(CC) -g -c DBFile.cc

Pipe.o: Pipe.cc
	$(CC) -g -c Pipe.cc

BigQ.o: BigQ.cc
	$(CC) -g -c BigQ.cc

LoserTree.o : LoserTree.cc
	$(CC) -g -c LoserTree.cc

GenericDBFile.o: GenericDBFile.cc
	$(CC) -g -c GenericDBFile.cc

HeapFile.o: HeapFile.cc
	$(CC) -g -c HeapFile.cc

HeapFileHandler.o: HeapFileHandler.cc
	$(CC) -g -c HeapFileHandler.cc

SortedFile.o: SortedFile.cc
	$(CC) -g -c SortedFile.cc

SortedFileHandler.o: SortedFileHandler.cc
	$(CC) -g -c SortedFileHandler.cc

Join.o: Join.cc
	$(CC) -g -c Join.cc

JoinUtil.o: JoinUtil.cc
	$(CC) -g -c JoinUtil.cc

# RelOp.o: RelOp.cc
# 	$(CC) -g -c RelOp.cc

SelectFile.o: SelectFile.cc
	$(CC) -g -c SelectFile.cc

SelectPipe.o: SelectPipe.cc
	$(CC) -g -c SelectPipe.cc

Sum.o: Sum.cc
	$(CC) -g -c Sum.cc

Project.o: Project.cc
	$(CC) -g -c Project.cc 

WriteOut.o: WriteOut.cc
	$(CC) -g -c WriteOut.cc 

DuplicateRemoval.o: DuplicateRemoval.cc
	$(CC) -g -c DuplicateRemoval.cc

GroupBy.o: GroupBy.cc
	$(CC) -g -c GroupBy.cc

Function.o: Function.cc
	$(CC) -g -c Function.cc

File.o: File.cc
	$(CC) -g -c File.cc

Record.o: Record.cc
	$(CC) -g -c Record.cc

Schema.o: Schema.cc
	$(CC) -g -c Schema.cc
		
yyfunc.tab.o: ParserFunc.y
	yacc -p "yyfunc" -b "yyfunc" -d ParserFunc.y
	#sed $(tag) yyfunc.tab.c -e "s/  __attribute__ ((__unused__))$$/# ifndef __cplusplus\n  __attribute__ ((__unused__));\n# endif/" 
	g++ -c yyfunc.tab.c

lex.yyfunc.o: LexerFunc.l
	lex -Pyyfunc LexerFunc.l
	gcc  -c lex.yyfunc.c


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
	rm -f y.tab.*
	rm -f yyfunc.tab.*
	rm -f lex.yy.*
	rm -f lex.yyfunc*
