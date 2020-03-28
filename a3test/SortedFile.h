#ifndef SortedFile_H
#define SortedFile_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "GenericDBFile.h"
#include "BigQ.h"
#include "Pipe.h"
#include "SortedFileHandler.h"

class SortedFile: public GenericDBFile{ 

private:
	File *mySortedFile; 
	Page *currPage;
	off_t whichPage; 
	int currRecord;  
	OrderMaker *sortOrder;
	long sortRunLength;
	struct sortInfo {OrderMaker *o; int l;};
	Pipe *inputPipe;
	Pipe *outputPipe;
	SortedFileHandler *sortFileHandler;
	OrderMaker *storedSortOrder;
	OrderMaker *queryOrderMaker;
	bool isQueryOrderMakerConstructReqd;
	bool isBinarySearchNeeded;
public:
	 SortedFile ();
	 ~SortedFile(); 

	 int Create (const char *fpath, void *startup);
	 int Open (const char *fpath);
	 int Close ();

	 void Load (Schema &myschema, const char *loadpath);

	 void MoveFirst ();
	 void Add (Record &addme);
	 int GetNext (Record &fetchme);
	 int GetNext (Record &fetchme, CNF &cnf, Record &literal);
	 void AddMetadata(const char *fpath,void *startup);
	 void setup(const char *fpath,void *startup);
	 int ConstructQueryOrderMaker(CNF &cnf);
	 int binarysearch(Record &temp, Record &literal, off_t low, off_t high);

};
#endif
