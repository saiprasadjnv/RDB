#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "SortedFile.h"
#include "Defs.h"
#include "string"
#include <iostream>
#include <stdio.h>
using namespace std;

SortedFile::SortedFile () {
    whichPage = 0; 
	currRecord = 0; 
    inputPipe=nullptr;
    outputPipe=nullptr; 
    // bigQ=nullptr;
    // printf("before");
    sortFileHandler=new SortedFileHandler();
    storedSortOrder=nullptr;
    // printf("after");
    // handler=new HeapFileHandler();
}
SortedFile::~SortedFile(){
    delete sortFileHandler;
}
/* 
 * Method that creates a new file based on the ftype provided. 
 * Before creating the new file it closes any existing file stream.
 */
int SortedFile::Create (const char *f_path, void *startup) {  
    mySortedFile.Close();
    mySortedFile.Open(0,(char *)f_path);
    sortInfo *sort_info=(sortInfo *)startup;
    sortFileHandler->f_path=(char *)f_path;
    sortFileHandler->sortOrder=sort_info->o;
    sortFileHandler->runlength=sort_info->l;
    // sortFileHandler->inputPipe=inputPipe;
    // sortFileHandler->outputPipe=outputPipe;
    // printf("Inside create");
    int *sortAttr=new int[20];
    Type *sortTypeAttr=new Type[20];
    int numAttrs=0;
    sort_info->o->getAttributes(sortAttr,sortTypeAttr,numAttrs);
    sortOrder.setAttributes(sortAttr,sortTypeAttr,numAttrs);
    sortRunLength=sort_info->l;
    delete[] sortAttr;
    delete[] sortTypeAttr;
    return 1;
}

/*
* Method used to load the records from tbl file given in 
* loadpath to the new file created in bit string.
*/
void SortedFile::Load (Schema &f_schema, const char *loadpath) {
    FILE *tableFile = fopen (loadpath, "r"); 
    Record temp;
    int i=0;
    // while(i++ <40000){
    // printf("i: %d \n", i); 
    // temp.SuckNextRecord(&f_schema,tableFile);
    while (temp.SuckNextRecord(&f_schema,tableFile)==1){
        Add(temp);
        // Close();
     }
    // mySortedFile.AddPage(&currPage, whichPage);
}

/*
* Method to open the given filepath(f_path) in read/write mode.
*/
int SortedFile::Open (const char *f_path) {
    mySortedFile.Close();
    mySortedFile.Open(1, (char *) f_path);
    // handler->init(currPage,whichPage,currRecord);
    return 1;
}

/*
* Method to move the pointer to the first data page in the file.
*/
void SortedFile::MoveFirst () {
    // handler->readHandler(mySortedFile,currPage,whichPage,0);
    mySortedFile.GetPage(&currPage,0);
    whichPage=0;
    currRecord=0;
}
/*
* Method to close the file.
*/
int SortedFile::Close () {
    // handler->tearDown(mySortedFile,currPage,whichPage);
    // sortFileHandler->readHandler(mySortedFile,currPage,whichPage,0,)
    sortFileHandler->tearDown(mySortedFile,currPage,whichPage);
    mySortedFile.Close();
    return 1;
}

/*

* Method to add new records given in rec to the current page and file if current page is full.
*/
void SortedFile::Add (Record &rec) {
    //  handler->writeHandler(mySortedFile,currPage,whichPage);
    // Schema mySchema("catalog", "lineitem");
    // rec.Print(&mySchema);  
    sortFileHandler->writeHandler(mySortedFile,currPage,whichPage);
    // printf("INpipe address in sorted file: %ld\n", inputPipe);
    // printf("outpipe address in sorted file: %ld\n", outputPipe);
    sortFileHandler->inputPipe->Insert(&rec);
}

/*
* Method to get the next record from the file and store it in the fetchme param.
*/
int SortedFile::GetNext (Record &fetchme) { 
    // handler->readHandler(mySortedFile,currPage,whichPage,currRecord);
    off_t len=mySortedFile.GetLength();
    if(currPage.GetFirst(&fetchme)==0){
        currPage.EmptyItOut();
        if(whichPage+1<len-1){ 
            ++whichPage;
            mySortedFile.GetPage(&currPage,whichPage);
            currPage.GetFirst(&fetchme);
            currRecord=1;
            return 1;
        }
    }else{
        currRecord++;
        return 1;
    }
    return 0; 
}

/*
* Method to get the next record from file and check against the provided CNF 
* and store it in the fetchme param if matched.
*/
int SortedFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    // handler->readHandler(mySortedFile,currPage,whichPage,currRecord);
    Record temp; 
    ComparisonEngine comp;
    int res;
    int count=0;
    read_next_record:
        res = GetNext(temp);
        count++; 
        if (res==0)
            return 0; 
        if(!(comp.Compare(&temp, &literal, &cnf))){
            goto read_next_record;
        }
    fetchme.Consume(&temp);
    return 1; 
}

void SortedFile::AddMetadata(const char *fpath,void *startup){
    // printf("Inside AddMetadata");
    char filePath[100];
    sprintf(filePath,"%s.meta",fpath);
    FILE *metaFile=fopen(filePath,"w+");
    fprintf(metaFile,"sorted\n");
    // printf("Inside AddMetadata1");
    int sortAttr[MAX_ANDS];
    Type sortTypeAttr[MAX_ANDS];
    int numAttrs;
    sortOrder.getAttributes(sortAttr,sortTypeAttr,numAttrs);
    for(int i=0;i<numAttrs;i++){
        fprintf(metaFile,"%d %d ",sortAttr[i],sortTypeAttr[i]);
    }
    fprintf(metaFile,"\n");
    fclose(metaFile);
}

void SortedFile::setup(const char *fpath,void *startup){
    char metaFilePath[100];
	sprintf(metaFilePath,"%s.meta",fpath);
    FILE *metaFile=fopen(metaFilePath,"r");
	char fileType[20];
	fscanf(metaFile,"%s",fileType);
    int sortAttr[MAX_ANDS];
    Type sortTypeAttr[MAX_ANDS];
    if(storedSortOrder!=nullptr){
        delete storedSortOrder;
    }
    storedSortOrder=new OrderMaker();
    int numAttrs=0;
    // int 
    
    
}




