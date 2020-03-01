#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	
	myLoserTree = new  LoserTree(in,out,runlen, sortorder); 
	pthread_t sortingThread; 
	Record temp1;  
	Schema mySchema ("catalog", "lineitem"); 
	long i =0; 
	// in.Remove(&temp1);
	// temp1.Print(&mySchema);
	// return;
	
	int err = pthread_create(&sortingThread, NULL, (THREADFUNCPTR)&LoserTree::sort,(void *)myLoserTree); 
	if(err){
		printf("Thread Not created due to some error!! \n");
		exit(0);
	}
	pthread_join(sortingThread, NULL); 
	remove("tempFile.bin");
	out.ShutDown ();
}

BigQ::~BigQ () {
}
