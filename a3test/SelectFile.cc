#include "RelOp.h"

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
	Schema mySchema ("catalog", "part");
	inArgs->inFile->MoveFirst(); 
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

void SelectFile::Run (DBFile &inFile, Pipe &outPipe, CNF &selOp, Record &literal) {
	struct args
	{
		/* data */
		DBFile *inFile; 
		Pipe *outPipe;
		CNF *selop; 
		Record *literal;  
	}; 
	args* inArgs = new args; 
	inArgs->inFile = &inFile; 
	inArgs->outPipe= &outPipe;
	inArgs->selop=  &selOp;
	inArgs->literal= &literal;
	pthread_create(&thread, NULL, SelectFileThread, (void *) inArgs); 
}

void SelectFile::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void SelectFile::Use_n_Pages (int runlen) {
	bufferSize = runlen; 
}

