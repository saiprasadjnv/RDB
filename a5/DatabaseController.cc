
#include <iostream>
#include "string.h"
#include "cstdio"
using namespace std;
#include "DatabaseController.h"

#define BIN_EXT ".bin"
#define SCHEMA_EXT ".schema"
#define TABLE_EXT ".tbl"


DatabaseController::DatabaseController(){

}
DatabaseController::~DatabaseController(){

}

int DatabaseController::processQuery(){
    switch(operationType){
        case SELECTOP:
        case CREATEOP:
            Create();
            return 1;
        case INSERTOP:
            Insert();
            return 1;
        case DROPOP:
        case UPDATEOP:
        case SETOP:
        default:
            cerr << "Invalid query passed!!\n";
            exit(1);
    }
    return 0;
}

int DatabaseController::Create(){
    DBFile *dbFile;
    string tableName;
    struct TableList* currTables=tables;
    if(currTables!=NULL){
        tableName=strdup(currTables->tableName);
    }
    string newTableSchema;
    newTableSchema.append(tableName).append(SCHEMA_EXT);
    // if(fopen(newTableSchema.c_str(),"r")!=NULL){
    //     cerr << "Table " << tableName << " already exists!\n";
    //     exit(1);
    // }
    string fileName;
    fileName.append(tableName).append(BIN_EXT);
    Schema *newSchema=NULL;
    struct sortInfo{
        OrderMaker *ordermakr;
        int runlen;
    }; 
    newSchema=createSchema(schemaAttributes,tableName);
    if(strcmp(fileType,"HEAP")==0){
        dbFile=new DBFile();
        dbFile->Create(fileName.c_str(),heap,NULL);
        dbFile->Close();
    }else if(strcmp(fileType,"SORTED")==0){
        OrderMaker *newOrderMaker=new OrderMaker();
        createOrderMaker(newOrderMaker,sortAttributes,newSchema);
        sortInfo *newSortInfo=new sortInfo();
        newSortInfo->ordermakr=newOrderMaker;
        newSortInfo->runlen=8;
        dbFile=new DBFile();
        dbFile->Create(fileName.c_str(),sorted,(void*)newSortInfo);
        dbFile->Close();
    }else{
        cerr << "Invalid fileType given for create query!!\n";
        exit(1);
    }

    return 1;

}


void DatabaseController::createOrderMaker(OrderMaker *toWhich,SortAttributes *fromWhich,Schema *whichSchema){
    struct SortAttributes *currSortAtts=fromWhich;
    vector<int> setTheseAttributes;
    vector<Type> setTheseTypes;
    while(currSortAtts!=NULL){
        int attPos=whichSchema->Find(currSortAtts->name);
        if(attPos==-1){
            cerr << "The sort attribute " << currSortAtts->name << " doesn't match the schema attributes!\n";
            exit(1);
        }
        setTheseAttributes.push_back(attPos);
        setTheseTypes.push_back(whichSchema->FindType(currSortAtts->name));
        currSortAtts=currSortAtts->next;
    }
    toWhich->setAttributes(setTheseAttributes.data(),setTheseTypes.data(),setTheseAttributes.size());
}
Schema* DatabaseController::createSchema(SchemaAttributes *fromWhich,string tableNme){
    if(fromWhich==NULL || fromWhich==nullptr){
        cerr << "Empty schema attributes found for create!!\n";
        exit(1);
    }
    SchemaAttributes *currSchema=fromWhich;
    vector<Attribute> newAtts;
    while(currSchema!=NULL){
        newAtts.push_back(Attribute{currSchema->name,(Type)currSchema->type});
        currSchema=currSchema->next;
    }
    Schema *toSchema=new Schema("create_sch",newAtts.size(),newAtts.data());
    // to->Print();
    string schemaFile;
    schemaFile.append(tableNme).append(SCHEMA_EXT);
    string tmpTable;
    tmpTable.append(tableNme).append(TABLE_EXT);
    FILE *fileDes=fopen(schemaFile.c_str(),"w+");
    fprintf(fileDes,"BEGIN\n");
    fprintf(fileDes,"%s\n", tableNme.c_str());
    fprintf(fileDes,"%s\n",tmpTable.c_str());
    for(auto it=newAtts.begin();it!=newAtts.end();it++){
        fprintf(fileDes,"%s ",it->name);
        switch(it->myType){
            case Int:
                fprintf(fileDes,"Int\n");
                break;
            case Double:
                fprintf(fileDes,"Double\n");
                break;
            case String:
                fprintf(fileDes,"String\n");
                break;
            default:
                cerr << "Invalid attribute type found while creating schema!!\n";
                fclose(fileDes);
                remove(schemaFile.c_str());
                exit(1);
        }
    }
    fprintf(fileDes,"END\n");
    fclose(fileDes);    
    newAtts.clear();
    return toSchema;

}

int DatabaseController::Insert(){
    string tableName;
    struct TableList* currTables=tables;
    if(currTables!=NULL){
        tableName=strdup(currTables->tableName);
    }
    string tableFile;
    tableFile.append(tableName).append(BIN_EXT);
    string tableSchema;
    tableSchema.append(tableName).append(SCHEMA_EXT);
    // if(fopen(newTableSchema.c_str(),"r")==NULL){
    //     cerr << "Table " << tableName << " doesn't exists!\n";
    //     exit(1);
    // }
    Schema myschema((char*)tableSchema.c_str(),(char*)tableName.c_str());
    DBFile dbfile;
    dbfile.Open(tableFile.c_str());
    dbfile.Load(myschema,fileName);
    dbfile.Close();

    return 1;
}