
#include <iostream>
#include "DBFile.h"
#include "Schema.h"
//#include "Record.h"
#include <stdlib.h>
#include "Handler.h"

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
	newDBfile.Create("/mnt/d/UF/DBI_project1/P1/bin/lineitem.bin", heap, NULL);
	newDBfile.Load(mySchema, "/mnt/d/UF/DBI/tpch-dbgen/lineitem.tbl");
	newDBfile.Close();  
	//newDBfile.Open("/mnt/d/UF/DBI_project1/P1/bin/lineitem.bin"); 
	//newDBfile.GetNext(tmp);

	return 123;
}


