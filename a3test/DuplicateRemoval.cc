#include "RelOp.h"

void DuplicateRemoval::Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {
    struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        Schema* mySchema; 
        long bufferSize; 
    };

    Args* inArgs = new Args {&inPipe, &outPipe, &mySchema, bufferSize}; 
    pthread_create(&thread,NULL, DuplicateRemovalThread, (void*)inArgs); 
}

void DuplicateRemoval::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void DuplicateRemoval::Use_n_Pages (int runlen) {
    bufferSize = runlen; 
}

void *DuplicateRemovalThread(void* myargs){
    struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        Schema* mySchema; 
        long bufferSize;
    };
    Args* inArgs = (Args*)myargs; 
    int numAttsinSchema = inArgs->mySchema->GetNumAtts(); 
    Attribute* attsinSchema = new Attribute[numAttsinSchema]; 
    Type* typesinSchema = new Type[numAttsinSchema];  
    int* attNumsinSchema = new int[numAttsinSchema]; 
    attsinSchema =  inArgs->mySchema->GetAtts(); 
    for(int i=0; i<numAttsinSchema; i++){
        attNumsinSchema[i] = i; 
        typesinSchema[i] = attsinSchema[i].myType;
    }
    OrderMaker *sortOrder = new OrderMaker(); 
    sortOrder->setAttributes(attNumsinSchema, typesinSchema, numAttsinSchema); 
    
    struct BigQArgs{
        Pipe *inPipe; 
        Pipe *outPipe; 
        OrderMaker* sortOrder; 
        long bufferSize; 
    };
    Pipe* bigQPipe = new Pipe(100);  
    BigQArgs* bigQArgs = new BigQArgs {inArgs->inPipe, bigQPipe, sortOrder, inArgs->bufferSize}; 
    pthread_t bigQthread; 
    pthread_create(&bigQthread, NULL, bigQThread, (void*)bigQArgs);
    Record* temp = new Record; 
    Record* rec1 = NULL; 
    Record* rec2 = NULL; 
    ComparisonEngine ceng; 
    while(bigQPipe->Remove(temp)){
        rec1 = rec2; 
        rec2 = temp; 
        while(rec1 && rec2 && ceng.Compare(rec1, rec2, sortOrder)==0){
            delete rec2; 
            rec2 = new Record; 
            bigQPipe->Remove(rec2);
        }
        inArgs->outPipe->Insert(rec1); 
        delete temp; 
        temp = new Record; 
    }
    delete temp; 
    delete rec2; 
    delete bigQPipe; 
    delete bigQArgs;
    inArgs->outPipe->ShutDown(); 
    delete inArgs; 
    return NULL; 
    
}

void* bigQThread(void* myargs){
    struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        OrderMaker* sortOrder; 
        long bufferSize;
    };
    Args* inArgs = (Args*)myargs;  
    BigQ bigQ (*inArgs->inPipe, *inArgs->outPipe,*inArgs->sortOrder,inArgs->bufferSize); 
    delete inArgs; 
    return NULL; 
}