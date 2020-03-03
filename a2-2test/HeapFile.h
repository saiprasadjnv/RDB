#ifndef HeapFile_H
#define HeapFile_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "HeapFileHandler.h"
#include "GenericDBFile.h"

class HeapFile: public GenericDBFile{ 
	friend class HeapFileHandler;

private:
	File *myHeapFile; 
	Page *currPage;
	off_t whichPage; 
	int currRecord;  
	HeapFileHandler *handler;
public:
	 HeapFile (); 
	 ~HeapFile();

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
