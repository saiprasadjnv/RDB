#ifndef DBFILE_H
#define DBFILE_H

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "Handler.h"

typedef enum {heap, sorted, tree} fType;
 

class DBFile {
	friend class Handler;

private:
	File myDBFile; 
	Page currPage;
	off_t whichPage; 
	int currRecord;
	off_t lastDirtyPage; 
	int numRecRead; 
	Handler handler;
public:
	DBFile (); 

	int Create (const char *fpath, fType file_type, void *startup);
	int Open (const char *fpath);
	int Close ();

	void Load (Schema &myschema, const char *loadpath);

	void MoveFirst ();
	void Add (Record &addme);
	int GetNext (Record &fetchme);
	int GetNext (Record &fetchme, CNF &cnf, Record &literal);

};
#endif
