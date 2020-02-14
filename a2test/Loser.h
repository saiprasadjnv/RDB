#include "Record.h"
#include "File.h" 
#include "DBFile.h"
#include "Pipe.h"
#define PARENT(i) (i-1)/2
class LoserTree
{
private:
    friend class BigQ;   
    int treeOrder; 
    int runLen;
    File tempFile; 
    DBFile mytempFile;
    Page tempPage; 
    Record *records; 
    OrderMaker *sortorder;
    Pipe *in;
    Pipe *out; 
    int *pagesForRun;  
    int whichPage;  
    struct LoserNode
    {
        int index; 
        int runNumber; 
    };
    LoserNode currentWinner; 
    LoserNode *myTree; 
    LoserNode play(int j, LoserNode player2);
    LoserNode removeMin(); 
   
    //Return 1 if(node1 <= node2) else returns 0 
    int compare(LoserNode node1, LoserNode node2);
    int compare(Record &rec1, Record &rec2); 
   
    void pass1();
    void pass2(); 
    /* data */
public:
    LoserTree(Record *recs, int runLen, OrderMaker &sortorder, Pipe input, Pipe output);
   // ~LoserTree();
    void initialize(); 
    void sort();
};
