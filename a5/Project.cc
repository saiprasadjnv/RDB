#include "RelOp.h"

struct Args{
    Pipe *inPipe; 
    Pipe *outPipe; 
    int *keepMe; 
    int numAttsInput; 
    int numAttsOutput; 
};

void* ProjectThread(void* myargs){

    Args* inArgs = (Args*)myargs; 
    Record* temp = new Record; 

    while(inArgs->inPipe->Remove(temp)){
        temp->Project(inArgs->keepMe, inArgs->numAttsOutput, inArgs->numAttsInput); 
        inArgs->outPipe->Insert(temp); 
        delete temp;
        temp = new Record; 
    } 
    delete temp; 
    inArgs->outPipe->ShutDown(); 
    delete inArgs; 
    return NULL;   
} 

void Project::Run (Pipe &inPipe, Pipe &outPipe, int *keepMe, int numAttsInput, int numAttsOutput) {
    Args* inArgs = new Args();
    inArgs->inPipe = &inPipe;
    inArgs->outPipe = &outPipe;
    inArgs->keepMe =  keepMe;
    inArgs->numAttsInput = numAttsInput;
    inArgs->numAttsOutput= numAttsOutput; 
    pthread_create(&thread, NULL, ProjectThread, inArgs); 
}

void Project::WaitUntilDone () {
    pthread_join (thread, NULL);
}

void Project::Use_n_Pages (int runlen) {
    bufferSize = runlen; 
}

