#ifndef HANDLER_H
#define HANDLER_H

#include "File.h"

class Handler{
    private:
        char currentState='r';
    public:
     Handler();
     int readHandler(File &file,Page &curPage,off_t &whichPage);
     int writeHandler(File &file,Page &curPage,off_t &whichPage);
     int tearDown(File &file,Page &curPage,off_t &whichPage);
     char getCurrentState();
};
#endif