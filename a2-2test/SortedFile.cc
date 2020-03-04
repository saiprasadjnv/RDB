#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TwoWayList.h"
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "Comparison.h"
#include "ComparisonEngine.h"
#include "SortedFile.h"
#include "Defs.h"
#include "string"
#include <iostream>
#include <stdio.h>
#include "string.h"
// #include "algorithm"
using namespace std;

SortedFile::SortedFile () {
    whichPage = 0; 
	currRecord = 0; 
    inputPipe=nullptr;
    outputPipe=nullptr; 
    sortFileHandler=new SortedFileHandler();
    storedSortOrder=nullptr;
    queryOrderMaker=nullptr;
    isQueryOrderMakerConstructReqd=true;
    mySortedFile=new File();
    currPage=new Page;
    sortOrder=new OrderMaker;
    isBinarySearchNeeded=true;
}
SortedFile::~SortedFile(){
    delete sortFileHandler;
    delete mySortedFile;
    delete currPage;
    delete sortOrder;
}
/* 
 * Method that creates a new file based on the ftype provided. 
 * Before creating the new file it closes any existing file stream.
 */
int SortedFile::Create (const char *f_path, void *startup) {  
    mySortedFile->Close();
    mySortedFile->Open(0,(char *)f_path);
    sortInfo *sort_info=(sortInfo *)startup;
    sortFileHandler->f_path=(char *)f_path;
    sortFileHandler->sortOrder=sort_info->o;
    sortFileHandler->runlength=(long)sort_info->l;
    sortFileHandler->inputPipe=inputPipe;
    sortFileHandler->outputPipe=outputPipe;
    int *sortAttr=new int[20];
    Type *sortTypeAttr=new Type[20];
    int numAttrs=0;
    sort_info->o->getAttributes(sortAttr,sortTypeAttr,numAttrs);
    sortOrder->setAttributes(sortAttr,sortTypeAttr,numAttrs);
    sortRunLength=sort_info->l;
    delete[] sortAttr;
    delete[] sortTypeAttr;
    return 1;
}

/*
* Method used to load the records from tbl file given in 
* loadpath to the new file created in bit string.
*/
void SortedFile::Load (Schema &f_schema, const char *loadpath) {
    FILE *tableFile = fopen (loadpath, "r"); 
    Record temp;
    int i=0;
    while (temp.SuckNextRecord(&f_schema,tableFile)==1){
        Add(temp);
     }
}

/*
* Method to open the given filepath(f_path) in read/write mode.
*/
int SortedFile::Open (const char *f_path) {
    mySortedFile->Close();
    // printf("In sortedfile open\n");
    mySortedFile->Open(1, (char *) f_path);
    setup(f_path,NULL);
    // handler->init(currPage,whichPage,currRecord);
    sortFileHandler->init(*currPage,whichPage,currRecord);
    return 1;
}

/*
* Method to move the pointer to the first data page in the file.
*/
void SortedFile::MoveFirst () {
    // handler->readHandler(mySortedFile,currPage,whichPage,0);
    mySortedFile->GetPage(currPage,0);
    whichPage=0;
    currRecord=0;
}
/*
* Method to close the file.
*/
int SortedFile::Close () {
    sortFileHandler->tearDown(*mySortedFile,*currPage,whichPage);
    mySortedFile->Close();
    return 1;
}

/*

* Method to add new records given in rec to the current page and file if current page is full. 
This will insert the record to a pipe to get the record processed based on the sort order.
*/
void SortedFile::Add (Record &rec) { 
    sortFileHandler->writeHandler(*mySortedFile,*currPage,whichPage);
    sortFileHandler->inputPipe->Insert(&rec);
}

/*
* Method to get the next record from the file and store it in the fetchme param.
*/
int SortedFile::GetNext (Record &fetchme) { 
    sortFileHandler->readHandler(mySortedFile,*currPage,whichPage,currRecord);
    off_t len=mySortedFile->GetLength();
    if(currPage->GetFirst(&fetchme)==0){
        currPage->EmptyItOut();
        if(whichPage+1<len-1){ 
            ++whichPage;
            mySortedFile->GetPage(currPage,whichPage);
            currPage->GetFirst(&fetchme);
            currRecord=1;
            return 1;
        }
    }else{
        currRecord++;
        return 1;
    }
    return 0; 
}

/*
* Method to get the next record from file and check against the provided CNF 
* and store it in the fetchme param if matched. It uses the query ordermaker constructed to perform 
the combination of binary and linear search wherever possible.
*/
int SortedFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
    sortFileHandler->readHandler(mySortedFile,*currPage,whichPage,currRecord);
    ConstructQueryOrderMaker(cnf);
    ComparisonEngine comp;
    Record temp;
    if(GetNext(temp)==0){
                return 0; 
            }
    if(comp.Compare(&temp,&literal,&cnf)==1){
            fetchme.Consume(&temp);
            return 1; 
        }
    if(queryOrderMaker!=nullptr && isBinarySearchNeeded){
        int searchresult = binarysearch(temp, literal, whichPage, mySortedFile->GetLength()-1); 
        isBinarySearchNeeded=false;
        if(searchresult == 0){
            return 0; 
        }
    }
    //Linear Search

    int res;
    int count=0;
    read_next_record:
        res = GetNext(temp);
        count++; 
        if (res==0){
            return 0; 
        }
        if(!(comp.Compare(&temp, &literal, &cnf))){
            goto read_next_record;
        }
    fetchme.Consume(&temp);
    return 1; 
}

/*
Method to create and add details of the current file into new metadata file for future use.
*/
void SortedFile::AddMetadata(const char *fpath,void *startup){
    char filePath[100];
    sprintf(filePath,"%s.meta",fpath);
    FILE *metaFile=fopen(filePath,"w+");
    fprintf(metaFile,"sorted\n");
    fprintf(metaFile,"%ld\n",sortRunLength);
    int sortAttr[MAX_ANDS];
    Type sortTypeAttr[MAX_ANDS];
    int numAttrs;
    sortOrder->getAttributes(sortAttr,sortTypeAttr,numAttrs);
    for(int i=0;i<numAttrs;i++){
        fprintf(metaFile,"%d %d ",sortAttr[i],sortTypeAttr[i]);
    }
    fprintf(metaFile,"\n");
    fclose(metaFile);
}

/*
Performs the intial setup by parsing the metadata file and storing the required details for future use.
*/
void SortedFile::setup(const char *fpath,void *startup){
    char metaFilePath[100];
	sprintf(metaFilePath,"%s.meta",fpath);
    FILE *metaFile=fopen(metaFilePath,"r");
	char fileType[20];
	fscanf(metaFile,"%s",fileType);
    long runLength=0;
    fscanf(metaFile,"%ld",&runLength);
    int sortAttr[MAX_ANDS];
    Type sortTypeAttr[MAX_ANDS];
    if(storedSortOrder!=nullptr){
        delete storedSortOrder;
    }
    storedSortOrder=new OrderMaker();
    int numAttrs=0;
    string::size_type sz;
    int val1=0,val2=0;
    int i=0;
    while(fscanf(metaFile,"%d",&val1)!=-1){
        sortAttr[i]=val1;
        fscanf(metaFile,"%d",&val2);
        sortTypeAttr[i]=(Type)val2;
        numAttrs++;
        i++;
    }
    storedSortOrder->setAttributes(sortAttr,sortTypeAttr,numAttrs);
    fclose(metaFile);
    sortFileHandler->f_path=(char*)fpath;
    sortFileHandler->sortOrder=storedSortOrder;
    sortRunLength=runLength;
    sortFileHandler->runlength=sortRunLength;
}

/*
Constructs new query ordermaker from the given CNF and stored sortOrder to use in future comparisons.
*/
int SortedFile::ConstructQueryOrderMaker(CNF &cnf){
    if(isQueryOrderMakerConstructReqd){
        int sortOrderAttr[MAX_ANDS];
        int literalAttr[MAX_ANDS];
        Type sortOrderTypeAttr[MAX_ANDS];
        int sortOrderNumAttr=0;
        storedSortOrder->getAttributes(sortOrderAttr,sortOrderTypeAttr,sortOrderNumAttr);
        int CNFAttr[MAX_ANDS];
        Type CNFAttTypes[MAX_ANDS];
        int CNFNumAttr=0;
        cnf.getSingleExpressionAttributes(literalAttr, CNFAttr,CNFAttTypes,CNFNumAttr);
        int queryOrderAttr[MAX_ANDS];
        Type queryOrderTypeAttr[MAX_ANDS];
        int queryOrderNumAttr=0;
        for(int i=0; i<sortOrderNumAttr; i++){
            int j=0;
            for(j=0; j<CNFNumAttr; j++){
                if(sortOrderAttr[i]==CNFAttr[j]){
                    queryOrderAttr[queryOrderNumAttr] = literalAttr[j];
                    queryOrderTypeAttr[queryOrderNumAttr] = CNFAttTypes[j]; 
                    queryOrderNumAttr++; 
                    break;
                }
            }
            if(j==CNFNumAttr){
                break;
            }
        }
        if(queryOrderNumAttr!=0){
            queryOrderMaker = new OrderMaker;
            queryOrderMaker->setAttributes(queryOrderAttr,queryOrderTypeAttr,queryOrderNumAttr);
        }
        isQueryOrderMakerConstructReqd=false;
        return 1;
    }
    
    return 0;
}

/*
Perform binary search on the current dbfile using the query ordermaker and given record literal.
It then stores the matched record in the temp parameter and return 1 in case of success otherwise 0.
*/
int SortedFile::binarysearch(Record &temp, Record &literal, off_t low, off_t high){
    if(low>high){
        return 0; 
    }
    ComparisonEngine ceng; 
    off_t mid = (low) + (high-low)/2; 
    Page *tempPage = new Page; 
    mySortedFile->GetPage(tempPage, mid); 
    if(tempPage->GetFirst(&temp)==0){
        delete tempPage;
        return 0; 
    }
    if(ceng.Compare(&literal, queryOrderMaker, &temp, storedSortOrder)<=0){
        delete tempPage; 
        return binarysearch(temp, literal, low, mid-1); 
    }
    int recNum=2;
    while (tempPage->GetFirst(&temp)!=0)
    {   
        if(ceng.Compare(&literal, queryOrderMaker, &temp, storedSortOrder)==0){
            whichPage = mid; 
            currPage->EmptyItOut();
            mySortedFile->GetPage(currPage,mid);
            sortFileHandler->popRecordsFromCurPage(*currPage, recNum-1); 
            delete tempPage;
            return 1; 
        }
        recNum++; 
    }
    off_t fileLen = mySortedFile->GetLength(); 
    if(mid+1 < fileLen-1){
        tempPage->EmptyItOut();
        mySortedFile->GetPage(tempPage, mid+1); 
        tempPage->GetFirst(&temp);
        if(ceng.Compare(&literal, queryOrderMaker, &temp, storedSortOrder)==0){
            whichPage = mid; 
            currPage->EmptyItOut();
            mySortedFile->GetPage(currPage,mid);
            sortFileHandler->popRecordsFromCurPage(*currPage, recNum); 
            delete tempPage;
            return 1; 
        }
        delete tempPage;
        return binarysearch(temp, literal, mid+1,high);
    }
    delete tempPage;
    return 0;   
}


