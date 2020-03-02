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
}

/*
* Read Handler that preprocess the state of the DBFile from write to read mode.
*/
int SortedFileHandler::readHandler(File &file,Page &curPage,off_t &whichPage,int lastReadRecord){
    if(currentState=='w'){
		inputPipe->ShutDown();	
		mergeNewRecords(file,outputPipe);
		// pthread_join(bigQThread,NULL);

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
		// printf("Inside writeHandler");
        currentReadPage=whichPage;
		// delete inputPipe;
		// delete outputPipe;
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
	// printf("Inside bigq runlen: %ld \n", bigqArgs->runlen);
	//  printf("INpipe address in bigQthread file: %ld\n", bigqArgs->inputPipe);
	//  printf("outpipe address in bigQthread file: %ld\n", bigqArgs->outputPipe);
	//Record temp1; 
	//int result = bigqArgs->inputPipe->Remove(&temp1); 
	// printf("Result after remove: %d\n", result);
	bigqArgs->sortedOrder.Print(); 
	BigQ bigq(*bigqArgs->inputPipe,*bigqArgs->outputPipe,bigqArgs->sortedOrder,bigqArgs->runlen);
	return NULL;
}

void SortedFileHandler::mergeNewRecords(File &file,Pipe *outputPipe){
	Record *temp1=new Record;
	Record *temp2=new Record;
	file.Close();
	char newFile[100];
	// printf("%s",f_path);
	sprintf(newFile,"%s.tmp",f_path);
	Page tempcurrPage;
	Page mainCurrPage;
	File commonFile; 
	off_t tempPageNum=0;
	off_t mainPageNum=0;
	printf("Opening tmpFile\n");
	commonFile.Open(0, newFile);
	commonFile.Close(); 

	if(outputPipe->Remove(temp1) == 0 ){
		// printf("Setting temp1 to null\n");
		temp1=nullptr;
	}
	if(GetRecord(mainCurrPage,mainPageNum,f_path,*temp2) == 0){
		// printf("Setting temp2 to null\n");
		temp2=nullptr;
	}
	ComparisonEngine cEngine;
	while(temp1!=nullptr || temp2!=nullptr){
		if(temp1!=nullptr && temp2!=nullptr){
			if(cEngine.Compare(temp1,temp2,sortOrder)<=0){
				// tmpFile->Add(*temp1);
				AddRecord(tempcurrPage,tempPageNum,newFile,*temp1);
				delete temp1;
				temp1=new Record();
				if(!outputPipe->Remove(temp1)){
					temp1=nullptr;
				}
			}else{
				// tmpFile->Add(*temp2);
				AddRecord(tempcurrPage,tempPageNum,newFile,*temp2);
				delete temp2;
				temp2=new Record();
				if(GetRecord(mainCurrPage,mainPageNum,f_path,*temp2) == 0){
					temp2=nullptr;
				}

			}
		}else if(temp1==nullptr){
			//temp2
				// tmpFile->Add(*temp2);
				AddRecord(tempcurrPage,tempPageNum,newFile,*temp2);
				delete temp2;
				temp2=new Record();
				if(GetRecord( mainCurrPage,mainPageNum,f_path,*temp2) == 0){
					temp2=nullptr;
				}
		}else if(temp2==nullptr){
			//temp1
				// tmpFile->Add(*temp1);
				AddRecord(tempcurrPage,tempPageNum,newFile,*temp1);
				delete temp1;
				
				temp1=new Record();
				if(!outputPipe->Remove(temp1)){
					// printf("Read all the records from oupipe\n");
					temp1=nullptr;
				}

		}
		
	}
	// outputPipe->ShutDown();
	AddPage(tempcurrPage, tempPageNum, newFile);
	commonFile.Close();

}


int SortedFileHandler::GetRecord(Page &currPage, off_t &pageNum ,char *f_path, Record &rec){
	if(currPage.GetFirst(&rec)==0){
		File *file = new File(); 
		file->Open(1, f_path); 
		off_t fileLen = file->GetLength();
		currPage.EmptyItOut(); 
		if(pageNum +1 < fileLen -1){
			++pageNum;
			file->GetPage(&currPage, pageNum);
			currPage.GetFirst(&rec);
			file->Close();
			delete file;
			return 1; 
		}
		else{
			file->Close();
			delete file;
			return 0; 
		}
	}
	else{
		return 1;
	}
	return 0;
}


int SortedFileHandler::AddRecord(Page &currPage, off_t &pageNum ,char *f_path, Record &rec){
	if(currPage.Append(&rec)==0){
		File *file = new File();
		file->Open(1,f_path);
		file->AddPage(&currPage, pageNum); 
		pageNum++; 
		currPage.EmptyItOut();
		currPage.Append(&rec);
		file->Close(); 
		delete file;
	}
	return 1; 
}

void SortedFileHandler::AddPage(Page &currPage, off_t &pageNum, char* f_path){
	File *file = new File();
	file->Open(1, f_path); 
	file->AddPage(&currPage, pageNum); 
	pageNum++;
	file->Close();
	delete file;
}