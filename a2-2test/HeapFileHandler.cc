#include <string.h>
#include <iostream>
#include "HeapFileHandler.h"
using namespace std;

HeapFileHandler::HeapFileHandler(){
    currentReadPage=0;
}

/*
* Read Handler that preprocess the state of the DBFile from write to read mode.
*/
int HeapFileHandler::readHandler(File *file,Page &curPage,off_t &whichPage,int lastReadRecord){
    if(currentState=='w'){
        file->AddPage(&curPage,whichPage);
        file->GetPage(&curPage,currentReadPage);
        popRecordsFromCurPage(curPage,lastReadRecord);
        whichPage=currentReadPage;
        currentState='r';
        return 1;
    }
    return 0;
}

/*
* Write Handler that preprocess the state of the DBFile from read to write mode.
*/
int HeapFileHandler::writeHandler(File &file,Page &curPage,off_t &whichPage){
    if(currentState=='r'){
        currentReadPage=whichPage;
        int len=file.GetLength();
        if(len!=0){
            file.GetPage(&curPage,len-2);
            return 1;
        }
        currentState='w';
    }
    return 0;
}

/*
* Method to perform any pre shutdown tasks.
*/
int HeapFileHandler::tearDown(File &file,Page &curPage,off_t &whichPage){
    int status=readHandler(&file,curPage,whichPage,0);
    return status;

}

/*
* Method to return the currentState of the file.
*/
char HeapFileHandler::getCurrentState(){
    return currentState;
}

/*
* Method to pop the records - number given in recordsToBePopped  from the curPage 
*/
int HeapFileHandler::popRecordsFromCurPage(Page &curPage,int recordsToBePopped){
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
int HeapFileHandler::init(Page &curPage,off_t &whichPage,int &currentRecord){
    currentState='r';
    return 1;
}