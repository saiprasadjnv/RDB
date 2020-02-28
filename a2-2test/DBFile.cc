#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"
#include <stdio.h>
#include <string.h>


// stub file .. replace it with your own DBFile.cc

DBFile::DBFile () {

}

int DBFile::Create (const char *f_path, fType f_type, void *startup) {
	// HeapFile heapFile;
	// GenericDBFile *myFile = &heapFile;
	// heapFile.test(); 
    switch(f_type){
        case heap:
			myFile = new HeapFile();
			// myFile = &heapFile;
			// myFile->test(); 
			myFile->Create(f_path, NULL); 
			myFile->AddMetadata(f_path,startup);
			// myFile->test(); 
            return 1;
        case sorted:
			myFile=new SortedFile();
			myFile->Create(f_path,startup);
			myFile->AddMetadata(f_path,startup);
            return 1;
        case tree:
            //In progress
            break;
        default:
           return 0;
    }
    return 0;
}


void DBFile::Load (Schema &f_schema, char *loadpath) {
    myFile->Load(f_schema, loadpath); 
}

int DBFile::Open (const char *f_path) {
	char metaFilePath[100];
	sprintf(metaFilePath,"%s.meta",f_path);
	FILE *metaFile=fopen(metaFilePath,"r");
	char fileType[20];
	fscanf(metaFile,"%s",fileType);
	if(strcmp("heap",(const char*)fileType)==0){
		myFile=new HeapFile();
	}else if(strcmp("sorted",(const char*)fileType)==0){
		myFile=new SortedFile();

	}
	myFile->Open(f_path);
	return 1;
}

void DBFile::MoveFirst () {
    myFile->MoveFirst(); 
}

int DBFile::Close () {
    myFile->Close(); 
	return 1;
}

void DBFile::Add (Record &rec) {
    myFile->Add(rec); 
}

int DBFile::GetNext (Record &fetchme) {
    return myFile->GetNext(fetchme); 
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    return myFile->GetNext(fetchme, cnf, literal); 
}
