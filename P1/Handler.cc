#include <string.h>
#include <iostream>
#include "Handler.h"
using namespace std;

Handler::Handler(){

}

/*
* Read Handler that preprocess the state of the DBFile from write to read mode.
*/
int Handler::readHandler(File &file,Page &curPage,off_t &whichPage){
    if(currentState=='w'){
        file.AddPage(&curPage,whichPage);
        currentState='r';
        return 1;
    }
    return 0;
}

/*
* Write Handler that preprocess the state of the DBFile from read to write mode.
*/
int Handler::writeHandler(File &file,Page &curPage,off_t &whichPage){
    if(currentState=='r'){
        int len=file.GetLength();
        if(len!=0){
            file.GetPage(&curPage,len-2);
            return 1;
        }
        currentState='w';
    }
    return 0;
}