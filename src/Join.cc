#include "RelOp.h"
#include "pthread.h"
#include "JoinUtil.h"

Join::Join(){
    bufferSize=8;
}

void Join::Run (Pipe &inPipeL, Pipe &inPipeR, Pipe &outPipe, CNF &selOp, Record &literal){
    JoinUtil *joinUtil=new JoinUtil();
    JoinUtilArgs *jArgs= new JoinUtilArgs;
    jArgs->inPipeL=&inPipeL;
    jArgs->inPipeR=&inPipeR;
    jArgs->outPipe=&outPipe;
    jArgs->selOp=&selOp;
    jArgs->literal=&literal;
    jArgs->bufferSize=bufferSize;
    jArgs->instance=joinUtil;
    pthread_create(&thread,NULL,(THREADFUNCPTR)&JoinUtil::process,(void*)jArgs);
}
void Join::WaitUntilDone (){
    pthread_join(thread,NULL);

}
void Join::Use_n_Pages (int n){
    bufferSize=n;
}
