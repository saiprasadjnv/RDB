#include "RelOp.h"

void Sum::Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe) {
	struct args
	{
		/* data */
		Pipe *inPipe; 
		Pipe *outPipe;
        Function *computeMe; 
	}; 
	args* inArgs = new args{&inPipe, &outPipe, &computeMe};
	
	pthread_create(&thread, NULL, SumThread, (void *) inArgs); 
}

void Sum::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void Sum::Use_n_Pages (int runlen) {
	bufferSize = runlen; 
}

void* SumThread(void* myargs){
	struct args
	{
		Pipe *inPipe; 
		Pipe *outPipe;
        Function *computeMe; 
	}; 
	args* inArgs = (args*)myargs; 
    int intResult, finalIntRes=0; 
    double doubleResult, finalDoubleRes = 0.0; 
    Type resType; 
    Record *temp = new Record; 
    while (inArgs->inPipe->Remove(temp))
    {
        resType = inArgs->computeMe->Apply(*temp, intResult, doubleResult); 
        if(resType == Int){
            finalIntRes += intResult;
        }
        else{
            finalDoubleRes += doubleResult;
        }
        delete temp; 
        temp = new Record; 
    }
    if(resType == Int){
        FILE *tempFile = fopen("tempSumResFile", "w"); 
        fprintf(tempFile, "%d", finalIntRes); 
        Attribute IA = {"int", Int}; 
        Schema sum_sch ("sum_sch", 1, &IA); 
        temp->SuckNextRecord(&sum_sch, tempFile); 
        inArgs->outPipe->Insert(temp); 
        fclose(tempFile); 
        remove("tempSumResFile");
    }
    if(resType == Double){ 
        FILE *tempFile = fopen("tempSumResFile", "w"); 
        fprintf(tempFile, "%lf", finalDoubleRes); 
        Attribute DA = {"double", Double}; 
        Schema sum_sch ("sum_sch", 1, &DA); 
        temp->SuckNextRecord(&sum_sch, tempFile); 
        inArgs->outPipe->Insert(temp); 
        fclose(tempFile); 
        remove("tempSumResFile");
    }
    delete temp; 
    inArgs->outPipe->ShutDown(); 
    delete inArgs; 
    return NULL; 
}