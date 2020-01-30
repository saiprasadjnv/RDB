
#include <iostream>
#include "DBFile.h"
#include "Schema.h"
//#include "Record.h"
#include <stdlib.h>
#include "DBFile.cc" 

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
	DBFile newDBfile; 
	//FILE *tableFile = fopen ("/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/tpch-dbgen/lineitem.tbl", "r");
	newDBfile.Create("/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/DBI_project1/P1/bin/lineitem.bin", heap, NULL);
	newDBfile.Load(mySchema, "/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/tpch-dbgen/lineitem.tbl");
	newDBfile.Close();

	return 123;
}


