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

/*
Method to create a dbfile instance based on the type (f_type) sent in the parameters,  
creates a metadata file that contains the created dbfile details like file type, runlength, sortorder.
It returns 1 in case of success otherwise 0.
*/
int DBFile::Create (const char *f_path, fType f_type, void *startup) {
    switch(f_type){
        case heap:
			myFile = new HeapFile();
			myFile->Create(f_path, NULL); 
			myFile->AddMetadata(f_path,startup);
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

/*
Method to load the data from the given file path into the current dbfile instance. It requires schema of 
the given table.
*/

void DBFile::Load (Schema &f_schema, char *loadpath) {
    myFile->Load(f_schema, loadpath); 
}

/*
Opens the file given by f_path in argument. It uses the metadata file that is already created for the given file
form which it determines the file type and other details. Using these details it opens the respective dbfile.
It returns 1 in case of sucess.
*/
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
	fclose(metaFile);
	myFile->Open(f_path);
	return 1;
}

/*
Moves the file pointer to starting of the file.
*/
void DBFile::MoveFirst () {
    myFile->MoveFirst(); 
}

/*
Closes the current dbfile and returns 1 in case of success.*/
int DBFile::Close () {
    myFile->Close(); 
	return 1;
}

/*
Adds record to the current dbfile. The record is sent as the parameter. 
*/
void DBFile::Add (Record &rec) {
    myFile->Add(rec); 
}

/*
Retrieves the next record from the current dbfile and return 1 in case of successfuly retrieved and stored
in the argument passed otherwise 0.
*/
int DBFile::GetNext (Record &fetchme) {
    return myFile->GetNext(fetchme); 
}

/*
Retrieves the next record that matches the given CNF and literal. Returns 1 in case of success otherwise 0.
*/
int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    return myFile->GetNext(fetchme, cnf, literal); 
}
