#include "RelOp.h"

void* SelectPipeThread(void* myargs){
	struct args
	{
		/* data */
		Pipe *inPipe; 
		Pipe *outPipe;
		CNF *selop; 
		Record *literal;  
	}; 
	args* inArgs = (args*)myargs; 
	Record *temp = new Record; 
    ComparisonEngine ceng; 
	while(inArgs->inPipe->Remove(temp)){
		if(ceng.Compare(temp, inArgs->literal, inArgs->selop)){
            inArgs->outPipe->Insert(temp); 
        }
		delete temp; 
		temp = new Record; 
	}
    delete temp;
    inArgs->outPipe->ShutDown(); 
    delete inArgs; 
    return NULL;
}

void SelectPipe::Run (Pipe &inPipe, Pipe &outPipe, CNF &selOp, Record &literal) {
	struct args
	{
		/* data */
		Pipe *inPipe; 
		Pipe *outPipe;
		CNF *selop; 
		Record *literal;  
	}; 
	args* inArgs = new args; 
	inArgs->inPipe = &inPipe;
	inArgs->outPipe = &outPipe;
	inArgs->selop =  &selOp;
	inArgs->literal=  &literal;
	pthread_create(&thread, NULL, SelectPipeThread, (void *) inArgs); 
}

void SelectPipe::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void SelectPipe::Use_n_Pages (int runlen) {
	bufferSize = runlen; 
}
