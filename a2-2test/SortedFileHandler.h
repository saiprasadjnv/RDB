#ifndef SORTEDFILEHANDLER_H
#define SORTEDFILEHANDLER_H

#include "File.h"
// #include "SortedFile.h"
#include "BigQ.h"
#include "pthread.h"

class SortedFileHandler{
    friend class SortedFile;
    private:
        char currentState;
        int currentReadPage;
        // struct consumerArgs{
        //     Pipe *outputPipe;
        //     char* fPath;
        // };
        struct bigQArgs{
            Pipe *inputPipe;
            Pipe *outputPipe;
            OrderMaker sortedOrder;
            int runlen;
        };
        Pipe *inputPipe;
        Pipe *outputPipe;
        OrderMaker* sortOrder;
        int runlength;
        // pthread_t consumerThread;
        // pthread_t bigQThread;
        char* f_path;
        off_t whichPage;

    public:
     SortedFileHandler();
     ~SortedFileHandler();
     int readHandler(File &file,Page &curPage,off_t &whichPage,int lastReadRecord);
     int writeHandler(File &file,Page &curPage,off_t &whichPage);
     int tearDown(File &file,Page &curPage,off_t &whichPage);
     char getCurrentState();
     int popRecordsFromCurPage(Page &curPage,int noOfRecordsToPop);
     int init(Page &curPage,off_t &whichPage,int &currentRecord);
    //  static void* consumer(void * args);
     static void* bigq(void *args);
     void mergeNewRecords(File &file,Pipe *outputPipe);
     void Add(File &file,Page &page,Record &rec);
};
#endif