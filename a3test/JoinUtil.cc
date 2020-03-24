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
    ceng=new ComparisonEngine();
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
    pthread_t leftBigQ;
    pthread_t rightBigQ;
    if(orderMakerRes==0){
        //block-nested join
    }else{
        //Join using bigQ
        joinUtil->leftTemp=new Record();
        joinUtil->rightTemp=new Record();
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
        int leftRes=joinUtil->leftBigQPipe->Remove(joinUtil->leftTemp);
        int rightRes=joinUtil->rightBigQPipe->Remove(joinUtil->rightTemp);
        if(leftRes==0 || rightRes==0){
            return NULL;
        }
        int leftNumAttrbs=joinUtil->leftTemp->numberOfAttrbs();
        int rightNumAttrbs=joinUtil->rightTemp->numberOfAttrbs();
        int *AttstoKeep=new int[leftNumAttrbs+rightNumAttrbs];
        for(int i=0;i<leftNumAttrbs;i++){
            AttstoKeep[i]=i;
        }
        for(int i=leftNumAttrbs;i<leftNumAttrbs+rightNumAttrbs;i++){
            AttstoKeep[i]=i-leftNumAttrbs;
        }
        Record *ltmp=new Record();
        Record *rtmp=new Record();
        ltmp->Copy(joinUtil->leftTemp);
        rtmp->Copy(joinUtil->rightTemp);
        joinUtil->leftVector.push_back(ltmp);
        joinUtil->rightVector.push_back(rtmp);
        delete joinUtil->leftTemp, joinUtil->rightTemp;
        joinUtil->leftTemp=new Record();
        joinUtil->rightTemp=new Record();
        joinUtil->getNextLeftGroup();
        joinUtil->getNextRightGroup();
        // int itr=0;
        while(1){
            if(joinUtil->leftVector.size()==0 || joinUtil->rightVector.size()==0){
                break;
            }
            int cRes=joinUtil->ceng->Compare(joinUtil->leftVector.front(),joinUtil->leftTableSortOrder,joinUtil->rightVector.front(),joinUtil->rightTableSortOrder);
            if(cRes==0){
                // itr++;
                // printf("In compare=0, itr:%d\n",itr);
                joinUtil->performCatesianProduct(AttstoKeep,leftNumAttrbs,rightNumAttrbs);
                joinUtil->clearVector(joinUtil->leftVector);
                if(joinUtil->leftTemp!=nullptr && joinUtil->rightTemp!=nullptr){
                Record *ltmp1=new Record();
                ltmp1->Copy(joinUtil->leftTemp);
                joinUtil->leftVector.push_back(ltmp1);
                joinUtil->getNextLeftGroup();
                joinUtil->clearVector(joinUtil->rightVector);
                Record *rtmp1=new Record();
                rtmp1->Copy(joinUtil->rightTemp);
                joinUtil->rightVector.push_back(rtmp1);
                joinUtil->getNextRightGroup();
                }
                

            }else if(cRes==-1){
                // itr++;
                // printf("In compare=-1, itr:%d\n",itr);
                joinUtil->clearVector(joinUtil->leftVector);
                Record *ltmp1=new Record();
                ltmp1->Copy(joinUtil->leftTemp);
                joinUtil->leftVector.push_back(ltmp1);
                joinUtil->getNextLeftGroup();

            }else if(cRes==1){
                // itr++;
                // printf("In compare=1, itr:%d\n",itr);
                joinUtil->clearVector(joinUtil->rightVector);
                Record *rtmp1=new Record();
                rtmp1->Copy(joinUtil->rightTemp);
                joinUtil->rightVector.push_back(rtmp1);
                joinUtil->getNextRightGroup();

            }
        }

    }
joinUtil->out->ShutDown();
return NULL;

}

void* JoinBigQThread(void* args){
    BigQArgs* inArgs = (BigQArgs*)args;  
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
    while(leftBigQPipe->Remove(leftTemp)!=0){
            if(compEng.Compare(leftVector.back(),leftTemp,leftTableSortOrder)!=0){
                return;
            }
            Record *tmp1=new Record();
            tmp1->Copy(leftTemp);
            leftVector.push_back(tmp1);
            delete leftTemp;
            leftTemp=new Record();
        }
        delete leftTemp;
        leftTemp=nullptr;
}
void JoinUtil::getNextRightGroup(){
    ComparisonEngine compEng;
    if(rightTemp!=nullptr){
        delete rightTemp;
    }
    rightTemp=new Record();
    while(rightBigQPipe->Remove(rightTemp)!=0){
            if(compEng.Compare(rightVector.back(),rightTemp,rightTableSortOrder)!=0){
                // break;
                return;
            }
            Record *tmp=new Record();
            tmp->Copy(rightTemp);
            rightVector.push_back(tmp);
        }
        delete rightTemp;
        rightTemp=nullptr;
}
void JoinUtil::performCatesianProduct(int* attsToKeep, int leftNumAttrs,int rightNumAttrs){
    for(int i=0;i<leftVector.size();i++){
        for(int j=0;j<rightVector.size();j++){
            Record temp;
            temp.MergeRecords(leftVector[i],rightVector[j],leftNumAttrs,rightNumAttrs,attsToKeep,leftNumAttrs+rightNumAttrs,leftNumAttrs);
            out->Insert(&temp);
        }
    }   
}

void JoinUtil::clearVector(vector<Record*> &toClear){
    for(int i=0;i<toClear.size();i++){
        delete toClear[i];
    }
    toClear.clear();
}