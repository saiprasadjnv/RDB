#ifndef HANDLER_H
#define HANDLER_H

#include "File.h"

class Handler{
    private:
        char currentState='r';
        int currentReadPage;
    public:
     Handler();
     int readHandler(File &file,Page &curPage,off_t &whichPage,int lastReadRecord);
     int writeHandler(File &file,Page &curPage,off_t &whichPage);
     int tearDown(File &file,Page &curPage,off_t &whichPage);
     char getCurrentState();
     int popRecordsFromCurPage(Page &curPage,int noOfRecordsToPop);
     int init(Page &curPage,off_t &whichPage,int &currentRecord);
};
#endif