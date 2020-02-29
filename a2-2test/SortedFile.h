#ifndef SortedFile_H
#define SortedFile_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "GenericDBFile.h"

class SortedFile: public GenericDBFile{ 

private:
	File myHeapFile; 
	Page currPage;
	off_t whichPage; 
	int currRecord;  
	// HeapFileHandler *handler;
	OrderMaker sortOrder;
	struct sortInfo {OrderMaker *o; int l;};
public:
	 SortedFile (); 

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

};
#endif
