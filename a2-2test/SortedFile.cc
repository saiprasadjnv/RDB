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
    // handler=new HeapFileHandler();
}
/* 
 * Method that creates a new file based on the ftype provided. 
 * Before creating the new file it closes any existing file stream.
 */
int SortedFile::Create (const char *f_path, void *startup) {  
    myHeapFile.Close();
    myHeapFile.Open(0,(char *)f_path);
    return 1;
}

/*
* Method used to load the records from tbl file given in 
* loadpath to the new file created in bit string.
*/
void SortedFile::Load (Schema &f_schema, const char *loadpath) {
    FILE *tableFile = fopen (loadpath, "r"); 
    Record temp;
    while (temp.SuckNextRecord(&f_schema,tableFile)==1){
        Add(temp);
    }
    myHeapFile.AddPage(&currPage, whichPage);
}

/*
* Method to open the given filepath(f_path) in read/write mode.
*/
int SortedFile::Open (const char *f_path) {
    myHeapFile.Close();
    myHeapFile.Open(1, (char *) f_path);
    // handler->init(currPage,whichPage,currRecord);
    return 1;
}

/*
* Method to move the pointer to the first data page in the file.
*/
void SortedFile::MoveFirst () {
    // handler->readHandler(myHeapFile,currPage,whichPage,0);
    myHeapFile.GetPage(&currPage,0);
    whichPage=0;
    currRecord=0;
}
/*
* Method to close the file.
*/
int SortedFile::Close () {
    // handler->tearDown(myHeapFile,currPage,whichPage);
    myHeapFile.Close();
    return 1;
}

/*

* Method to add new records given in rec to the current page and file if current page is full.
*/
void SortedFile::Add (Record &rec) {
    //  handler->writeHandler(myHeapFile,currPage,whichPage);
     if(currPage.Append(&rec)==0){
            myHeapFile.AddPage(&currPage, whichPage);
            whichPage++;
            currPage.EmptyItOut();
            currPage.Append(&rec);
        }
}

/*
* Method to get the next record from the file and store it in the fetchme param.
*/
int SortedFile::GetNext (Record &fetchme) { 
    // handler->readHandler(myHeapFile,currPage,whichPage,currRecord);
    off_t len=myHeapFile.GetLength();
    if(currPage.GetFirst(&fetchme)==0){
        currPage.EmptyItOut();
        if(whichPage+1<len-1){ 
            ++whichPage;
            myHeapFile.GetPage(&currPage,whichPage);
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
    // handler->readHandler(myHeapFile,currPage,whichPage,currRecord);
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
    char filePath[100];
    sprintf(filePath,"%s.meta",fpath);
    FILE *metaFile=fopen(filePath,"w+");
    fprintf(metaFile,"sorted\n");
    fclose(metaFile);
}

void SortedFile::setup(const char *fpath,void *startup){
    
}




