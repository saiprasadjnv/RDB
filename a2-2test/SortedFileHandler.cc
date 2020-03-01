#include <string.h>
#include <iostream>
#include "SortedFileHandler.h"
#include "HeapFile.h"
#include "pthread.h"
using namespace std;

SortedFileHandler::SortedFileHandler(){
    currentReadPage=0;
}

/*
* Read Handler that preprocess the state of the DBFile from write to read mode.
*/
int SortedFileHandler::readHandler(File &file,Page &curPage,off_t &whichPage,int lastReadRecord){
    if(currentState=='w'){
		inputPipe->ShutDown();
		mergeNewRecords(file,outputPipe);

        // file.AddPage(&curPage,whichPage);
        // file.GetPage(&curPage,currentReadPage);
        // popRecordsFromCurPage(curPage,lastReadRecord);
        // whichPage=currentReadPage;
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
		printf("Inside writeHandler");
        currentReadPage=whichPage;
		// delete inputPipe;
		// delete outputPipe;
		inputPipe=new Pipe(100);
		outputPipe=new Pipe(100);
		// consumerArgs consumerInfo={outputPipe,f_path};
		// pthread_create(&consumerThread,NULL,SortedFileHandler::consumer,(void *)&consumerInfo);
		bigQArgs *bigqArgs= new bigQArgs();
		bigqArgs->inputPipe=inputPipe;
		bigqArgs->outputPipe=outputPipe;
		bigqArgs->runlen=runlength;
		bigqArgs->sortedOrder= *sortOrder; 
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
    int status=readHandler(file,curPage,whichPage,0);
    return status;

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
	printf("Inside bigq runlen: %ld \n", bigqArgs->runlen);
	bigqArgs->sortedOrder.Print(); 
	BigQ bigq(*bigqArgs->inputPipe,*bigqArgs->outputPipe,bigqArgs->sortedOrder,bigqArgs->runlen);
}

void SortedFileHandler::mergeNewRecords(File &file,Pipe *outputPipe){
	Record *temp1=new Record();
	Record *temp2=new Record;
	// file.Close();
	char newFile[100];
	printf("%s",f_path);
	sprintf(newFile,"%s.tmp",f_path);
	HeapFile tmpFile;
	tmpFile.Create((const char*)newFile,NULL);
	HeapFile mainFile;
	mainFile.Open((const char*)f_path);
	if(!outputPipe->Remove(temp1)){
		temp1=nullptr;
	}
	if(!mainFile.GetNext(*temp2)){
		temp2=nullptr;
	}
	ComparisonEngine cEngine;
	while(temp1!=nullptr || temp2!=nullptr){
		if(temp1!=nullptr && temp2!=nullptr){
			if(cEngine.Compare(temp1,temp2,sortOrder)<=0){
				tmpFile.Add(*temp1);
				delete temp1;
				temp1=new Record();
				if(!outputPipe->Remove(temp1)){
					temp1=nullptr;
				}
			}else{
				tmpFile.Add(*temp2);
				delete temp2;
				temp2=new Record();
				if(!mainFile.GetNext(*temp2)){
					temp2=nullptr;
				}

			}
		}else if(temp1==nullptr){
			//temp2
				tmpFile.Add(*temp2);
				delete temp2;
				temp2=new Record();
				if(!mainFile.GetNext(*temp2)){
					temp2=nullptr;
				}
		}else if(temp2==nullptr){
			//temp1
				tmpFile.Add(*temp1);
				delete temp1;
				temp1=new Record();
				if(!outputPipe->Remove(temp1)){
					temp1=nullptr;
				}

		}
	}
	tmpFile.Close();
	// mainFile.Close();
	

}
