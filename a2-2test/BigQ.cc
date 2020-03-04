#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, long runlen) {
	myLoserTree = new  LoserTree(in,out,runlen, sortorder); 
	pthread_t sortingThread; 
 
	long i =0; 
	
	int err = pthread_create(&sortingThread, NULL, (THREADFUNCPTR)&LoserTree::sort,(void *)myLoserTree); 
	if(err){
		printf("Thread Not created due to some error!! \n");
		exit(0);
	}
	pthread_join(sortingThread, NULL); 
	remove("tempFile.bin");
	out.ShutDown ();
	delete myLoserTree;
}

BigQ::~BigQ () {
}
