
#include <iostream>
#include "DBFile.h"
#include "Schema.h"
#include "Record.h"
#include <stdlib.h>
#include "SortedFile.h"
#include "test.h"
#include "Comparison.h"
#include "Defs.h"

using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

extern struct AndList *final;

int main () {

	// now open up the text file and start procesing it
       // FILE *tableFile = fopen ("/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/tpch-dbgen/lineitem.tbl", "r");

    //Record temp;
    Schema mySchema ("catalog", "lineitem");
	// DBFile newDBfile;
	SortedFile *newDBFile=new SortedFile(); 
	//FILE *tableFile = fopen ("/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/tpch-dbgen/lineitem.tbl", "r");
	// newDBfile.Create("bin/lineitem.bin", heap, NULL);
	// SortedFile sortedfile;
	OrderMaker o;
	// rel->get_sort_order (o);
	int sortAttr[1]={0};
	Type typeAttr[1]={Int};
	o.setAttributes(sortAttr,typeAttr,1);
	struct {OrderMaker *o; int l;} startup = {&o, 10};
	newDBFile->Create("bin/test.bin",&startup);
	// newDBfile.Create("bin/test.bin", sorted, &startup);
	newDBFile->Load(mySchema, "/mnt/d/UF/DBI/tpch-dbgen/lineitem.tbl");
	newDBFile->Close();  
	//newDBfile.Open("/mnt/d/UF/DBI_project1/P1/bin/lineitem.bin"); 
	//newDBfile.GetNext(tmp);

	return 123;
}


