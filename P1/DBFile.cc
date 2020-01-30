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
        if(currPage.Append(&temp)==0){
            myDBFile.AddPage(&currPage, whichPage);
            whichPage++;
            currPage.EmptyItOut();
            currPage.Append(&temp);
        }
    }
    myDBFile.AddPage(&currPage, whichPage);
}

int DBFile::Open (const char *f_path) {
    myDBFile.Close();
    return 1;
}

void DBFile::MoveFirst () {
}

int DBFile::Close () {
    return myDBFile.Close();
}

void DBFile::Add (Record &rec) {
    Record temp; 
     if(currPage.Append(&temp)==0){
            myDBFile.AddPage(&currPage, whichPage);
            whichPage++;
            currPage.EmptyItOut();
            currPage.Append(&temp);
        }
}

int DBFile::GetNext (Record &fetchme) {
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
}
