#ifndef JOINUTIL_H
#define JOINUTIL_H
#include <iostream>
#include "Pipe.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "BigQ.h"
#include "vector"
#include "Record.h"
using namespace std;

class JoinUtil{
    private:
        struct JoinUtilArgs{
            Pipe *inPipeL;
            Pipe *inPipeR;
            Pipe *outPipe;
            CNF *selOp;
            Record *literal;
            long bufferSize;
            JoinUtil *instance;
        };
        Pipe *leftBigQPipe;
        Pipe *rightBigQPipe;
        Pipe *out;
        OrderMaker *leftTableSortOrder;
        OrderMaker *rightTableSortOrder;
        CNF joinCNF;
        vector<Record*> leftVector;
        vector<Record*> rightVector;
        Record* leftTemp;
        Record* rightTemp;
        ComparisonEngine *ceng;
        
    public:
    JoinUtil();
    ~JoinUtil();
    static void* process(void* args);
    void getNextLeftGroup();
    void getNextRightGroup();
    void performCatesianProduct(int* attsToKeep, int leftNumAttrs,int rightNumAttrs);
    void clearVector(vector<Record*> &toClear);

};
#endif
