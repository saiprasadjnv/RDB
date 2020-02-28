#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "DBFile.h"
#include "Defs.h"


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
			// myFile->test(); 
            return 1;
        case sorted:
            break;
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
    myFile->GetNext(fetchme); 
	return 0;
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    myFile->GetNext(fetchme, cnf, literal); 
	return 1;
}
