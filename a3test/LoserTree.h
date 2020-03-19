#include <iostream>
#include <limits.h>
#include "File.h"
#include "Pipe.h"

#define PARENT(i) ((i-1)/2) 
#define INTERNALNODES(num) ((num+1)/2 -1) 
#define TREENODEINDEX(index) (index+ (INTERNALNODES(treeSize)))

class LoserTree{

public:
  struct LoserNode{
        long recordIndex;
        long runNumber; 
    };
    Pipe *in;
    Pipe *out; 
    OrderMaker *sortorder; 
    long treeSize; 
    long runLen; 
    long currRunNumber; 
    long numRecsInRun;
    Record** records;
    File *tempFile;
    Page *tempPage; 
    Page **pageBuffers;
    off_t whichPage;
    LoserNode *myTree;
    long *pagesPerRun;
    Schema *mySchema; 
    LoserNode *currentWinner;
    Record *currWinnerRecord; 
    Record testing;

    void pass1(); 
    void pass2(); 
    long play(long j, LoserNode &player2);
    int compare(LoserNode &node1, LoserNode &node2);
    int compare(Record &rec1, Record &rec2);   
    void print(); 
    int Add(Record &rec);
    int writeDirtyPage();
    void setupPass1();
    void setupPass2(); 
    void initialize();
    void cleanup();

      LoserTree(Pipe &input, Pipe &output, long runNumber, OrderMaker &sortingOrder);
      ~LoserTree(); 
      static void* sort(void *args);
};