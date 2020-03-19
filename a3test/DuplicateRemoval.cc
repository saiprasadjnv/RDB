#include "RelOp.h"

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

void *DuplicateRemovalThread(void* myargs){
    struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        Schema* mySchema; 
        long bufferSize;
    };
     struct BigQArgs{
        Pipe *inPipe; 
        Pipe *outPipe; 
        OrderMaker* sortOrder; 
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
    Pipe* bigQPipe = new Pipe(100);  
    BigQArgs* bigQArgs = new BigQArgs; 
    bigQArgs->inPipe =  inArgs->inPipe;
    bigQArgs->outPipe = bigQPipe;
    bigQArgs->sortOrder = sortOrder;
    bigQArgs->bufferSize =  inArgs->bufferSize;

    pthread_t bigQthread; 
    pthread_create(&bigQthread, NULL, bigQThread, (void*)bigQArgs);
    Record* temp = new Record; 
    Record* rec1 = nullptr; 
    ComparisonEngine ceng; 
    int i=0; 
    while(bigQPipe->Remove(temp)){
        if(rec1==nullptr){
            // printf("exiting from if1: %d ", i++);
            rec1 = new Record; 
            rec1->Consume(temp); 
            delete temp;  
            temp = new Record; 
            continue; 
        }
        if(ceng.Compare(rec1, temp, sortOrder)==0){
            delete temp; 
            temp = new Record; 
            // printf("exiting from if2: %d ", i++);
            continue; 
        }
        rec1->Print(inArgs->mySchema);
        inArgs->outPipe->Insert(rec1); 
        delete rec1; 
        rec1 = new Record; 
        rec1->Consume(temp); 
        delete temp;  
        temp = new Record; 
    }
    delete temp; 
    delete rec1; 
    delete bigQPipe; 
    inArgs->outPipe->ShutDown(); 
    delete inArgs; 
    return NULL; 
    
}

void DuplicateRemoval::Run (Pipe &inPipe, Pipe &outPipe, Schema &mySchema) {
    struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        Schema* mySchema; 
        long bufferSize; 
    };
 
    Args* inArgs = new Args; 
    inArgs->inPipe = &inPipe;
    inArgs->outPipe = &outPipe;
    inArgs->mySchema = &mySchema;
    inArgs->bufferSize = bufferSize;
    printf("run len in dup run : %ld , class level: %ld\n", inArgs->bufferSize,bufferSize); 
    pthread_create(&thread,NULL, DuplicateRemovalThread, (void*)inArgs); 
}

void DuplicateRemoval::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void DuplicateRemoval::Use_n_Pages (int runlen) {
    printf("%d runlen in use_n\n", runlen);
    bufferSize = runlen; 
    printf("%ld  buffsize in use_n\n", bufferSize);
}

DuplicateRemoval::DuplicateRemoval(){ 
    bufferSize = 100; 
}

