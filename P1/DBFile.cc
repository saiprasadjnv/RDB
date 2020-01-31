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
using namespace std;
// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {

}

int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    myDBFile.Close();
    myDBFile.Open(0,(char *)f_path);
    return 1;
}

void DBFile::Load (Schema &f_schema, const char *loadpath) {
    FILE *tableFile = fopen (loadpath, "r"); 
    Record temp;
    while (temp.SuckNextRecord(&f_schema,tableFile)==1){
        Add(temp);
    }
    myDBFile.AddPage(&currPage, whichPage);
}

int DBFile::Open (const char *f_path) {
    myDBFile.Close();
    myDBFile.Open(1, (char *) f_path);
    return 1;
}

void DBFile::MoveFirst () {
    lastDirtyPage = whichPage; 
    myDBFile.AddPage(&currPage, whichPage);
    myDBFile.GetPage(&currPage,0);
    whichPage=0;
    currRecord=0;
}

int DBFile::Close () {
    return myDBFile.Close();
}

void DBFile::Add (Record &rec) {
     if(currPage.Append(&rec)==0){
            myDBFile.AddPage(&currPage, whichPage);
            if(whichPage==0){ 
                myDBFile.AddPage(&currPage,++whichPage);
            }
            whichPage++;
            currPage.EmptyItOut();
            currPage.Append(&rec);
        }
}

int DBFile::GetNext (Record &fetchme) { 
    numRecRead++; 
    // cout << "NumRecs Read   " << numRecRead;
    off_t len=myDBFile.GetLength();
    if(currPage.GetFirst(&fetchme)==0){
        currPage.EmptyItOut();
        if(++whichPage<len-1){ 
            myDBFile.GetPage(&currPage,whichPage);
            currPage.GetFirst(&fetchme);
            return 1;
        }
    }else{
        return 1;
    }
    return 0; 
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    Record temp; 
    ComparisonEngine comp;
    int res;
    int cres;
    int count=0;
    read_next_record:
        res = GetNext(temp);
        count++;
        // cout << "Got new record"; 
        if (res==0)
            return 0; 
        if(cres = !(comp.Compare(&temp, &literal, &cnf))){
            // cout << cres << " is Obtained " << count <<"\n"; 
            goto read_next_record;
        }
    // Schema mySchema ("catalog", "lineitem");
    // temp.Print(&mySchema);
    fetchme.Consume(&temp);
    return 1; 
}

