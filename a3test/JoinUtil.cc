#include "JoinUtil.h"
#include <iostream>
using namespace std;

void* JoinBigQThread(void* args);
struct BigQArgs{
            Pipe *inPipe; 
            Pipe *outPipe; 
            OrderMaker* sortOrder; 
            long bufferSize; 
        };

JoinUtil::JoinUtil(){
    leftTableSortOrder=new OrderMaker;
    rightTableSortOrder=new OrderMaker;
    // leftTemp=nullptr;
    // rightTemp=nullptr;
}

JoinUtil::~JoinUtil(){

}
void* JoinUtil::process(void* args){
    printf("Inside JoinUtil\n");
    JoinUtil *joinUtil=nullptr;
    JoinUtilArgs *jArgs=(JoinUtilArgs*)args;
    joinUtil=jArgs->instance;
    joinUtil->joinCNF=*jArgs->selOp;
    joinUtil->out=jArgs->outPipe;
    int orderMakerRes=joinUtil->joinCNF.GetSortOrders(*joinUtil->leftTableSortOrder,*joinUtil->rightTableSortOrder);
    // joinUtil->joinCNF.Print();
    // joinUtil->leftTableSortOrder->Print();
    // joinUtil->rightTableSortOrder->Print();
    pthread_t leftBigQ;
    pthread_t rightBigQ;
    if(orderMakerRes==0){
        //block-nested join
    }else{
        //Join using bigQ
        joinUtil->leftTemp=nullptr;
        joinUtil->rightTemp=nullptr;
        ComparisonEngine cEng;
        int *AttstoKeep=new int[41];
        joinUtil->leftBigQPipe=new Pipe(100);
        joinUtil->rightBigQPipe=new Pipe(100);
        BigQArgs *leftbigQArgs=new BigQArgs;
        leftbigQArgs->inPipe=jArgs->inPipeL;
        leftbigQArgs->bufferSize=jArgs->bufferSize;
        leftbigQArgs->sortOrder=joinUtil->leftTableSortOrder;
        leftbigQArgs->outPipe=joinUtil->leftBigQPipe;
        BigQArgs *rightbigQArgs=new BigQArgs;
        rightbigQArgs->inPipe=jArgs->inPipeR;
        rightbigQArgs->bufferSize=jArgs->bufferSize;
        rightbigQArgs->sortOrder=joinUtil->rightTableSortOrder;
        rightbigQArgs->outPipe=joinUtil->rightBigQPipe;
        pthread_create(&leftBigQ,NULL,JoinBigQThread,(void*)leftbigQArgs);
        pthread_create(&rightBigQ,NULL,JoinBigQThread,(void*)rightbigQArgs);
        // printf("INpipe address in JoinUtil file: %ld-%ld\n", jArgs->inPipeL,leftbigQArgs->inPipe);
        // printf("outpipe address in JoinUtil file: %ld-%ld\n", joinUtil->leftBigQPipe,leftbigQArgs->outPipe);
        Schema schema1("catalog","supplier");
        Schema schema2("catalog","partsupp");
        Record temp1;
        Record temp2;
        int leftRes=joinUtil->leftBigQPipe->Remove(&temp1);
        int rightRes=joinUtil->rightBigQPipe->Remove(&temp2);
        if(leftRes==0 || rightRes==0){
            return NULL;
        }
        int leftNumAttrbs=temp1.numberOfAttrbs();
        int rightNumAttrbs=temp2.numberOfAttrbs();
        for(int i=0;i<leftNumAttrbs;i++){
            AttstoKeep[i]=i;
        }
        for(int i=0;i<rightNumAttrbs;i++){
            AttstoKeep[i]=i;
        }
        // temp2.Print(&schema2);
        joinUtil->leftVector.push_back(temp1);
        joinUtil->rightVector.push_back(temp2);
        joinUtil->getNextLeftGroup();
        joinUtil->getNextRightGroup();
        // joinUtil->leftBigQPipe->Remove(&temp1);
        // joinUtil->leftVector.back().Print(&schema1);
        // temp1.Print(&schema1);
        while(1){
            if(joinUtil->leftVector.size()==0 || joinUtil->rightVector.size()==0){
                break;
            }
            int cRes=cEng.Compare(&joinUtil->leftVector.front(),joinUtil->leftTableSortOrder,&joinUtil->rightVector.front(),joinUtil->rightTableSortOrder);
            if(cRes==0){
                printf("In compare=0\n");
                joinUtil->performCatesianProduct(AttstoKeep,leftNumAttrbs,rightNumAttrbs);
                joinUtil->leftVector.clear();
                joinUtil->leftVector.push_back(*joinUtil->leftTemp);
                joinUtil->getNextLeftGroup();
                joinUtil->rightVector.clear();
                joinUtil->rightVector.push_back(*joinUtil->rightTemp);
                joinUtil->getNextRightGroup();

            }else if(cRes==-1){
                printf("In compare=-1\n");
                joinUtil->leftVector.clear();
                joinUtil->leftVector.push_back(*joinUtil->leftTemp);
                joinUtil->getNextLeftGroup();

            }else if(cRes==1){
                printf("In compare=1\n");
                joinUtil->rightVector.clear();
                joinUtil->rightVector.push_back(*joinUtil->rightTemp);
                joinUtil->getNextRightGroup();

            }

        }

    }

return NULL;

}

void* JoinBigQThread(void* args){
    BigQArgs* inArgs = (BigQArgs*)args;  
    // printf("INpipe address in bigQThread file: %ld\n", inArgs->inPipe);
    // printf("outpipe address in bigQThread file: %ld\n", inArgs->outPipe);
    BigQ bigQ (*inArgs->inPipe, *inArgs->outPipe,*inArgs->sortOrder,inArgs->bufferSize); 
    delete inArgs; 
    return NULL; 

}

void JoinUtil::getNextLeftGroup(){
    ComparisonEngine compEng;
    if(leftTemp!=nullptr){
       delete leftTemp;
    }
    leftTemp=new Record();
    Schema schema("catalog","supplier");
    printf("leftVector size:%d\n",leftVector.size());
    while(leftBigQPipe->Remove(leftTemp)!=0){
        leftVector.back().Print(&schema);
        if(compEng.Compare(&leftVector.back(),leftTemp,leftTableSortOrder)!=0){
            printf("in getnextleftgroup break\n");
            break;
        }
        leftVector.push_back(*leftTemp);
    }

}
void JoinUtil::getNextRightGroup(){
    ComparisonEngine compEng;
    if(rightTemp!=nullptr){
        delete rightTemp;
    }
    rightTemp=new Record();
    Schema schema("catalog","partsupp");
    printf("rightVector size:%d\n",rightVector.size());
    while(rightBigQPipe->Remove(rightTemp)!=0){
        // rightVector.back().Print(&schema);
        if(compEng.Compare(&rightVector.back(),rightTemp,rightTableSortOrder)!=0){
            break;
        }
        rightVector.push_back(*rightTemp);
    }

}
void JoinUtil::performCatesianProduct(int* attsToKeep, int leftNumAttrs,int rightNumAttrs){
    for(int i=0;i<leftVector.size();i++){
        for(int j=0;j<rightVector.size();j++){
            Record temp;
            temp.MergeRecords(&leftVector[i],&rightVector[j],leftNumAttrs,rightNumAttrs,attsToKeep,leftNumAttrs+rightNumAttrs,leftNumAttrs);
            out->Insert(&temp);
        }
    }

}