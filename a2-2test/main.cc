
#include <iostream>
#include "DBFile.h"
#include "Schema.h"
#include "Record.h"
#include <stdlib.h>
#include "SortedFile.h"
// #include "test.h"
#include "Comparison.h"
#include "Defs.h"
#include "test1.h"

using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

extern struct AndList *final;

int main () {
	const char *dbfile_dir = "bin/"; // dir where binary heap files should be stored
	const char *tpch_dir ="/mnt/d/UF/DBI/tpch-dbgen/"; // dir where dbgen tpch files (extension *.tbl) can be found
	const char *catalog_path = "catalog"; // full path of the catalog file
	setup (catalog_path, dbfile_dir, tpch_dir);

	// now open up the text file and start procesing it
       // FILE *tableFile = fopen ("/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/tpch-dbgen/lineitem.tbl", "r");

    //Record temp;
	relation *rel_ptr[] = {n, r, c, p, ps, s, o, li};
	relation *rel;
	rel = rel_ptr [7];
	CNF cnf; 
	Record literal;
	rel->get_cnf (cnf, literal);
	// cnf.Print();
	// return 321;
    Schema *mySchema=new Schema("catalog", "lineitem");
	// DBFile newDBfile;
	DBFile *newDBFile=new DBFile(); 
	//FILE *tableFile = fopen ("/Users/macuser/Documents/Study_1/Study/DBI/Projects/Project1/tpch-dbgen/lineitem.tbl", "r");
	// newDBfile.Create("bin/lineitem.bin", heap, NULL);
	// SortedFile sortedfile;
	OrderMaker o;
	// // rel->get_sort_order (o);
	// int sortAttr[2]={0,1};
	// Type typeAttr[2]={Int,Int};
	// o.setAttributes(sortAttr,typeAttr,2);
	int sortAttr[1]={4};
	Type typeAttr[1]={Double};
	o.setAttributes(sortAttr,typeAttr,1);
	struct {OrderMaker *o; int l;} startup = {&o, (long)1};
	// newDBFile->Create("bin/test.bin", sorted ,&startup);
	// newDBfile.Create("bin/test.bin", heap, &startup);
	// newDBFile->Load(*mySchema, "/mnt/d/UF/DBI/tpch-dbgen/lineitem.tbl");
	// newDBFile->Close();
	newDBFile->Open("bin/test.bin");
	//Print using GentNext1
	// Record temp;
	// int cnt=0;
	// newDBFile->MoveFirst();
	// while (newDBFile->GetNext(temp) && ++cnt) {
	// 	temp.Print (mySchema);
	// 	// if (cnt % 10000) {
	// 	// 	cerr << ".";
	// 	// }
	// }

	//Print using Getnext2
	Record temp;
	int cnt=0;
	while (newDBFile->GetNext (temp, cnf, literal) && ++cnt) {
		temp.Print (rel->schema());
		// if (cnt % 10000 == 0) {
		// 	cerr << ".";
		// }
	}
	cout << "\n scanned " << cnt << " recs \n";
	printf("Closing File now**************\n");
	newDBFile->Close();  
	delete mySchema;
	delete newDBFile;
	//newDBfile.Open("/mnt/d/UF/DBI_project1/P1/bin/lineitem.bin"); 
	//newDBfile.GetNext(tmp);

	return 123;
}


