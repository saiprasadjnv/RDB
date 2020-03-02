#include <string.h>
#include <iostream>
#include "SortedFileHandler.h"
#include "HeapFile.h"
#include "pthread.h"
using namespace std;

SortedFileHandler::SortedFileHandler(){
    currentReadPage=0;
	inputPipe = nullptr; 
	outputPipe = nullptr;
	currentState='r';
	sortOrder = nullptr; 
	f_path = nullptr;
	runlength = 0; 
	whichPage=0;
}

SortedFileHandler::~SortedFileHandler(){
	// pthread_join(bigQThread,NULL);
}
/*
* Read Handler that preprocess the state of the DBFile from write to read mode.
*/
int SortedFileHandler::readHandler(File &file,Page &curPage,off_t &whichPage,int lastReadRecord){
    // printf("Current State: %c\n",currentState);
	if(currentState == 'w'){
		//   printf("Current State: %c\n",currentState);
		inputPipe->ShutDown();	
		//   printf("Current State: %c\n",currentState);
		mergeNewRecords(file,outputPipe);
		// pthread_join(bigQThread,NULL);
		
        if(inputPipe != nullptr)
			delete inputPipe;
		if(outputPipe != nullptr)
			 delete outputPipe;
		
        currentState='r';

        return 1;
    }
    return 0;
}

/*
* Write Handler that preprocess the state of the DBFile from read to write mode.
*/
int SortedFileHandler::writeHandler(File &file,Page &curPage,off_t &whichPage){
	if(currentState=='r'){
		// printf("Inside writeHandler");
        currentReadPage=whichPage;
		if(inputPipe != nullptr)
			delete inputPipe;
		if(outputPipe != nullptr)
			 delete outputPipe;
		// printf("INpipe address in before create file: %ld\n", inputPipe);
	 	// printf("outpipe address in before create file: %ld\n", outputPipe);
		// delete inputPipe;
		// delete outputPipe;
		inputPipe=new Pipe(100);
		outputPipe=new Pipe(100);
		// printf("INpipe address in after create file: %ld\n", inputPipe);
	 	// printf("outpipe address in after create file: %ld\n", outputPipe);
		// consumerArgs consumerInfo={outputPipe,f_path};
		// pthread_create(&consumerThread,NULL,SortedFileHandler::consumer,(void *)&consumerInfo);
		bigQArgs *bigqArgs= new bigQArgs();
		bigqArgs->inputPipe=inputPipe;
		bigqArgs->outputPipe=outputPipe;
		bigqArgs->runlen=runlength;
		bigqArgs->sortedOrder= *sortOrder; 
		// printf("*******inside writehandler*****\n");
		//  printf("INpipe address in bigqArgs file: %ld\n", bigqArgs->inputPipe);
	 	// printf("outpipe address in bigqArgs file: %ld\n", bigqArgs->outputPipe);
		pthread_t bigQThread; 
		pthread_create(&bigQThread,NULL,this->bigq,(void*)bigqArgs);
		currentState='w';
		// pthread_join(bigQThread,NULL); 
    	return 1;
    }
    return 0;
}

/*
* Method to perform any pre shutdown tasks.
*/
int SortedFileHandler::tearDown(File &file,Page &curPage,off_t &whichPage){
    return readHandler(file,curPage,whichPage,0);
    // return status;

}

/*
* Method to return the currentState of the file.
*/
char SortedFileHandler::getCurrentState(){
    return currentState;
}

/*
* Method to pop the records - number given in recordsToBePopped  from the curPage 
*/
int SortedFileHandler::popRecordsFromCurPage(Page &curPage,int recordsToBePopped){
    Record temp;
    for(int i=0;i<recordsToBePopped;i++){
        if(curPage.GetFirst(&temp)==0){
            return 0;
        }
    }
    return 1;
}

/*
* Method to initialize the system.
*/
int SortedFileHandler::init(Page &curPage,off_t &whichPage,int &currentRecord){
    currentState='r';
    return 1;
}

void* SortedFileHandler::bigq(void * arg){
	bigQArgs *bigqArgs=(bigQArgs*)arg;
	// printf("Inside bigq runlen: %ld \n", bigqArgs->runlen);
	//  printf("INpipe address in bigQthread file: %ld\n", bigqArgs->inputPipe);
	//  printf("outpipe address in bigQthread file: %ld\n", bigqArgs->outputPipe);
	//Record temp1; 
	//int result = bigqArgs->inputPipe->Remove(&temp1); 
	// printf("Result after remove: %d\n", result);
	// bigqArgs->sortedOrder.Print(); 
	BigQ bigq(*bigqArgs->inputPipe,*bigqArgs->outputPipe,bigqArgs->sortedOrder,bigqArgs->runlen);
	return NULL;
}

void SortedFileHandler::mergeNewRecords(File &file,Pipe *outputPipe){
	Record *temp1=new Record();
	Record *temp2=new Record();
	File *tempFile=new File();
	Page *tempPage=new Page();
	file.Close();
	char newFile[100];
	// printf("%s",f_path);
	//   printf("Line 134\n");
	sprintf(newFile,"%s.tmp",f_path);
	//   printf("Line 138\n");
	// HeapFile *tmpFile = new HeapFile;
	// printf("Creating tmpFile\n");
	// tmpFile->Create((const char*)newFile,NULL);
	HeapFile *mainFile = new HeapFile();
	long i=0;
	Schema *mySchema=new Schema("catalog", "lineitem");
	// printf("Opening mainFile\n");
	mainFile->Open((const char*)f_path);
	tempFile->Open(0,newFile);
	if(outputPipe->Remove(temp1) == 0 ){
		// printf("Setting temp1 to null\n");
		temp1=nullptr;
	}
	if(mainFile->GetNext(*temp2) == 0){
		// printf("Setting temp2 to null\n");
		temp2=nullptr;
	}
	//  printf("Line 151\n");
	ComparisonEngine cEngine;
	while(temp1!=nullptr || temp2!=nullptr){
		//  printf("Line 154\n");
		if(temp1!=nullptr && temp2!=nullptr){
			// printf("Line 156\n");
			if(cEngine.Compare(temp1,temp2,sortOrder)<=0){
				// tmpFile->Add(*temp1);
				Add(*tempFile,*tempPage,*temp1);
				delete temp1;
				temp1=new Record();
				if(!outputPipe->Remove(temp1)){
					temp1=nullptr;
				}
			}else{
				// tmpFile->Add(*temp2);
				Add(*tempFile,*tempPage,*temp1);
				delete temp2;
				temp2=new Record();
				if(!mainFile->GetNext(*temp2)){
					temp2=nullptr;
				}

			}
		}else if(temp1==nullptr){
			//temp2
				// printf("Line 174\n");
				// tmpFile->Add(*temp2);
				Add(*tempFile,*tempPage,*temp1);
				delete temp2;
				temp2=new Record();
				if(!mainFile->GetNext(*temp2)){
					temp2=nullptr;
				}
		}else if(temp2==nullptr){
			//temp1
				// printf("Line 182\n");
				// temp1->Print(&mySchema);
					// printf("Line 185\n");
				// tmpFile->Add(*temp1);
				Add(*tempFile,*tempPage,*temp1);
					// printf("Line 186\n");
				if(temp1 != nullptr)
					delete temp1;
				//  printf("i: %ld\n",i++);
				// temp1->Print(&mySchema);
				// printf("Line 193\n");
				// printf("Line 194\n");
				// printf("Line 195\n");
				// printf("Line 196\n");
				temp1=new Record();
				// printf("Line 188\n");
				if(!outputPipe->Remove(temp1)){
					//  printf("Line 189\n");
					// printf("Read all the records from oupipe\n");
					temp1=nullptr;
				}

		}
		
	}
	// outputPipe->ShutDown();
	tempFile->AddPage(tempPage,whichPage);
	delete temp1;
	delete temp2;
	delete mySchema;
	mainFile->Close();
	// tmpFile->Close();
	// delete tmpFile;
	delete mainFile;

}

void SortedFileHandler::Add(File &file,Page &page,Record &rec){
	if(page.Append(&rec)==0){
            printf("In Add - SortedFileHandler %ld\n",this);
            file.AddPage(&page, whichPage);
            whichPage++;
            page.EmptyItOut();
            page.Append(&rec);
        }

}
