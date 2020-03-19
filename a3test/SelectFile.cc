#include "RelOp.h"

void SelectFile::Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal) {
	struct args
	{
		/* data */
		DBFile *inFile; 
		Pipe *outPipe;
		CNF *selop; 
		Record *literal;  
	}; 
	args* inArgs = new args{&inFile, &outPipe, &selOp, &literal};
	
	pthread_create(&thread, NULL, SelectFileThread, (void *) inArgs); 
}

void SelectFile::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void SelectFile::Use_n_Pages (int runlen) {
	bufferSize = runlen; 
}

void* SelectFileThread(void* myargs){
	struct args
	{
		/* data */
		DBFile *inFile; 
		Pipe *outPipe;
		CNF *selop; 
		Record *literal;  
	}; 
	args* inArgs = (args*)myargs; 
	Record *temp = new Record; 
	while(inArgs->inFile->GetNext(*temp, *inArgs->selop, *inArgs->literal)){
		inArgs->outPipe->Insert(temp); 
		delete temp; 
		temp = new Record; 
	}
	delete temp; 
	inArgs->outPipe->ShutDown(); 
	delete inArgs; 
	return NULL;
}