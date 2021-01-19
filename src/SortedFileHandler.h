#ifndef SORTEDFILEHANDLER_H
#define SORTEDFILEHANDLER_H

#include "File.h"
// #include "SortedFile.h"
#include "BigQ.h"
#include "pthread.h"

class SortedFileHandler{
    friend class SortedFile;
    private:
        char currentState='r';
        int currentReadPage;
        struct bigQArgs{
            Pipe *inputPipe;
            Pipe *outputPipe;
            OrderMaker sortedOrder;
            long runlen;
        };
        Pipe* inputPipe;
        Pipe* outputPipe;
        OrderMaker* sortOrder;
        long runlength;
        pthread_t bigQThread;
        char* f_path;

    public:
     SortedFileHandler();
     int readHandler(File *file,Page &curPage,off_t &whichPage,int lastReadRecord);
     int writeHandler(File &file,Page &curPage,off_t &whichPage);
     int tearDown(File &file,Page &curPage,off_t &whichPage);
     char getCurrentState();
     int popRecordsFromCurPage(Page &curPage,int noOfRecordsToPop);
     int init(Page &curPage,off_t &whichPage,int &currentRecord);
     static void* bigq(void *args);
     void mergeNewRecords(File &file,Pipe *outputPipe);
     int AddRecord(Page &currPage, off_t &pageNum, char* f_path, Record &rec);
     int GetRecord(Page &currPage, off_t &pageNum, char* f_path, Record &rec); 
     void AddPage(Page &currPage, off_t &pageNum, char* f_path);
};
#endif