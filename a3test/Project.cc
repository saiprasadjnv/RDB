#include "RelOp.h"

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
    
    // Attribute IA = {"int", Int};
    // Attribute SA = {"string", String};
    // Attribute DA = {"double", Double};

	// Attribute att3[] = {IA, SA, DA};
	// Schema out_sch ("out_sch", inArgs->numAttsOutput, att3);
    // Schema mySchema ("catalog", "part");

    while(inArgs->inPipe->Remove(temp)){
        temp->Project(inArgs->keepMe, inArgs->numAttsOutput, inArgs->numAttsOutput); 
        // printf("\n %d Pipe in Project thread\n", inArgs->outPipe);
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
    struct Args{
        Pipe *inPipe; 
        Pipe *outPipe; 
        int *keepMe; 
        int numAttsInput; 
        int numAttsOutput; 
    };
    Args* inArgs = new Args;
     inArgs->inPipe = &inPipe;
     inArgs->outPipe = &outPipe;
     inArgs->keepMe =  keepMe;
     inArgs->numAttsInput = numAttsInput;
     inArgs->numAttsOutput= numAttsOutput; 
     pthread_create(&thread, NULL, ProjectThread, (void*)inArgs); 
}

void Project::WaitUntilDone () {
    pthread_join (thread, NULL);
}

void Project::Use_n_Pages (int runlen) {
    bufferSize = runlen; 
}

