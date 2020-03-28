#include "RelOp.h"

// Attribute IA11 = {"int", Int};
// Attribute SA11 = {"string", String};
// Attribute DA11 = {"double", Double};
// Attribute s_nationkey = {"s_nationkey", Int};
// Attribute ps_supplycost = {"ps_supplycost", Double};
// Attribute joinatt[] = {IA11, SA11, SA11, s_nationkey,SA11, DA11, SA11, IA11, IA11, IA11, ps_supplycost,SA11};
// Schema join_sch ("join_sch", 12, joinatt);

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
	// Schema mySchema ("catalog", "part");
	// Schema mySchema("catalog","partsupp");
	inArgs->inFile->MoveFirst(); 
	while(inArgs->inFile->GetNext(*temp, *inArgs->selop, *inArgs->literal)){
		// temp->Print(&join_sch);
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

