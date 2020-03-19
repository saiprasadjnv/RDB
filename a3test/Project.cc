#include "RelOp.h"

void Project::Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput) {
    struct Args{
        Pipe *inPipe; 
        Pipe *outPipe; 
        int *keepMe; 
        int numAttsInput; 
        int numAttsOutput; 
    };
    Args* inArgs = new Args {&inPipe, &outPipe, keepMe, numAttsInput, numAttsOutput}; 

}

void Project::WaitUntilDone () {
    pthread_join (thread, NULL);
}

void Project::Use_n_Pages (int runlen) {
    bufferSize = runlen; 
}

void* ProjectThread(void* myargs){
    struct Args{
        Pipe *inPipe; 
        Pipe *outPipe; 
        int *keepMe; 
        int numAttsInput; 
        int numAttsOutput; 
    };
    Args* inArgs = (Args*)myargs; 
    Record* temp = new Record; 
    while(inArgs->inPipe->Remove(temp)){
        temp->Project(inArgs->keepMe, inArgs->numAttsOutput, inArgs->numAttsOutput); 
        inArgs->outPipe->Insert(temp); 
        delete temp;
        temp = new Record; 
    } 
    
    delete temp; 
    inArgs->outPipe->ShutDown(); 
    delete inArgs; 
    return NULL;   
} 
