#ifndef GENERICDBFILE_H
#define GENERICDBFILE_H
#include "Defs.h"
#include "File.h" 
#include "Record.h" 
#include "Comparison.h"
#include "ComparisonEngine.h"

class GenericDBFile {	
	protected: 
	char state; 
	
	public: 
    virtual int Create (const char *fpath, void *startup)=0;
	virtual int Open (const char *fpath)=0;
	virtual int Close ()=0;

	virtual void Load (Schema &myschema, const char *loadpath)=0;

	virtual void MoveFirst ()=0;
	virtual void Add (Record &addme)=0;
	virtual int GetNext (Record &fetchme)=0;
    virtual int GetNext (Record &fetchme, CNF &cnf, Record &literal)=0;
	virtual void AddMetadata(const char *fpath,void *startup)=0;
	virtual void setup(const char *fpath,void *startup)=0;
}; 
#endif
