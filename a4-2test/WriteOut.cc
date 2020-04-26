#include "RelOp.h"

void* WriteOutThread(void* myargs){
    struct Args{
        Pipe *inPipe;  
        FILE *outFile; 
        Schema *mySchema; 
    };
    Args* inArgs = (Args*)myargs; 
    Record* temp = new Record;  
    while(inArgs->inPipe->Remove(temp)){
        temp->WriteToFile(inArgs->outFile, inArgs->mySchema); 
        delete temp;
        temp = new Record; 
    }
    delete temp; 
    return NULL; 
}

void WriteOut::Run (Pipe &inPipe, FILE *outFile, Schema &mySchema) {
    struct Args{
        Pipe *inPipe;  
        FILE *outFile; 
        Schema *mySchema; 
    };
    Args* inArgs = new Args;
    inArgs->inPipe = &inPipe;
    inArgs->outFile = outFile;
    inArgs->mySchema = &mySchema; 
    pthread_create(&thread, NULL, WriteOutThread, (void*)inArgs); 
}

void WriteOut::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void WriteOut::Use_n_Pages (int runlen) {
    bufferSize = runlen; 
}
