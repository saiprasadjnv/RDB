#include "RelOp.h"
#include <string.h>

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
    // Schema mySchema("catalog", "nation"); 
    while (inArgs->inPipe->Remove(temp))
    {
        // temp->Print(&mySchema); 
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
        fprintf(tempFile, "%d|", finalIntRes); 
        fclose(tempFile); 
        tempFile = fopen("tempSumResFile", "r");  
        // exit(1); 
        Attribute IA = {"int", Int}; 
        Schema sum_sch ("sum_sch", 1, &IA);
        temp->SuckNextRecord(&sum_sch, tempFile); 
        // Attribute IA = {"aggregate", Int}; 
        // Schema outSch("outsch",1,&IA); 
        // temp->Print(&sum_sch);  
        inArgs->outPipe->Insert(temp); 
        fclose(tempFile); 
        remove("tempSumResFile");
    }
    if(resType == Double){ 
        FILE *tempFile = fopen("tempSumResFile", "w"); 
        fprintf(tempFile, "%lf|", finalDoubleRes); 
        fclose(tempFile);
        tempFile = fopen("tempSumResFile", "r"); 
        double test; 
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

void Sum::Run (Pipe &inPipe, Pipe &outPipe, Function &computeMe) {
	struct args
	{
		/* data */
		Pipe *inPipe; 
		Pipe *outPipe;
        Function *computeMe; 
	}; 
	args* inArgs = new args;
    inArgs->inPipe = &inPipe;
    inArgs->outPipe = &outPipe;
    inArgs->computeMe = &computeMe;
	
	pthread_create(&thread, NULL, SumThread, (void *) inArgs); 
}

void Sum::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void Sum::Use_n_Pages (int runlen) {
	bufferSize = runlen; 
}

