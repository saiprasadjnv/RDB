#include "iostream"
#include "DBFile.h"
#include "cstring"
using namespace std;
// make sure that the information below is correct

char *catalog_path = "catalog"; 
char *tpch_dir ="/mnt/d/UF/DBI/tpch-dbgen/"; // dir where dbgen tpch files (extension *.tbl) can be found
char *dbfile_dir = "./";

void loadHeapBinFile(const char* fname,char* loadPath,Schema &schema){
    DBFile dbfile;
    dbfile.Create(fname,heap,NULL);
    dbfile.Load(schema,loadPath);
    dbfile.Close();
}

int main(int argc, char *argv[]){
    char fname[100];
    sprintf(fname,"%ssupplier.bin",dbfile_dir);
    char filePath[100];
    sprintf(filePath,"%ssupplier.tbl",tpch_dir);
    Schema schema("catalog","supplier");
    loadHeapBinFile(fname,filePath,schema);
    fname[0]=0;
    filePath[0]=0;
    sprintf(fname,"%spartsupp.bin",dbfile_dir);
    sprintf(filePath,"%spartsupp.tbl",tpch_dir);
    Schema schema1("catalog","partsupp");
    loadHeapBinFile(fname,filePath,schema1);
    fname[0]=0;
    filePath[0]=0;
    sprintf(fname,"%spart.bin",dbfile_dir);
    sprintf(filePath,"%spart.tbl",tpch_dir);
    Schema schema2("catalog","part");
    loadHeapBinFile(fname,filePath,schema2);
    fname[0]=0;
    filePath[0]=0;
    sprintf(fname,"%snation.bin","");
    sprintf(filePath,"%snation.tbl","");
    Schema schema3("catalog","nation");
    loadHeapBinFile(fname,filePath,schema3);

    
}