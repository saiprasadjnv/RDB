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
#include "DBFile.h"
#include "Defs.h"
#include "string"
#include <iostream>
#include "Handler.h" 
using namespace std;

DBFile::DBFile () {
    whichPage = 0; 
	currRecord = 0;  

}
/* 
 * Method that creates a new file based on the ftype provided. 
 * Before creating the new file it closes any existing file stream.
 */
int DBFile::Create (const char *f_path, fType f_type, void *startup) {  
    myDBFile.Close();
    switch(f_type){
        case heap:
            myDBFile.Open(0,(char *)f_path);
            return 1;
        case sorted:
            //In progress
            break;
        case tree:
            //In progress
            break;
        default:
           return 0;
    }
    return 0;
}

/*
* Method used to load the records from tbl file given in 
* loadpath to the new file created in bit string.
*/
void DBFile::Load (Schema &f_schema, const char *loadpath) {
    FILE *tableFile = fopen (loadpath, "r"); 
    Record temp;
    while (temp.SuckNextRecord(&f_schema,tableFile)==1){
        Add(temp);
    }
    myDBFile.AddPage(&currPage, whichPage);
}

/*
* Method to open the given filepath(f_path) in read/write mode.
*/
int DBFile::Open (const char *f_path) {
    myDBFile.Close();
    myDBFile.Open(1, (char *) f_path);
    handler.init(currPage,whichPage,currRecord);
    return 1;
}

/*
* Method to move the pointer to the first data page in the file.
*/
void DBFile::MoveFirst () {
    handler.readHandler(myDBFile,currPage,whichPage,0);
    myDBFile.GetPage(&currPage,0);
    whichPage=0;
    currRecord=0;
}
/*
* Method to close the file.
*/
int DBFile::Close () {
    handler.tearDown(myDBFile,currPage,whichPage);
    myDBFile.Close();
    return 1;
}

/*

* Method to add new records given in rec to the current page and file if current page is full.
*/
void DBFile::Add (Record &rec) {
     handler.writeHandler(myDBFile,currPage,whichPage);
     if(currPage.Append(&rec)==0){
            myDBFile.AddPage(&currPage, whichPage);
            whichPage++;
            currPage.EmptyItOut();
            currPage.Append(&rec);
        }
}

/*
* Method to get the next record from the file and store it in the fetchme param.
*/
int DBFile::GetNext (Record &fetchme) { 
    handler.readHandler(myDBFile,currPage,whichPage,currRecord);
    off_t len=myDBFile.GetLength();
    if(currPage.GetFirst(&fetchme)==0){
        currPage.EmptyItOut();
        if(whichPage+1<len-1){ 
            ++whichPage;
            myDBFile.GetPage(&currPage,whichPage);
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
int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    handler.readHandler(myDBFile,currPage,whichPage,currRecord);
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


