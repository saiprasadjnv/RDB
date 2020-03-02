#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, long runlen) {
	// printf("Bigq constructor:\n");
	// Record temp1; 
	// int result = in.Remove(&temp1); 
// m
// 	printf("INpipe address in BIgq file: %ld\n", &in);
//     printf("outpipe address in bigq file: %ld\n", &out);
	myLoserTree = new  LoserTree(in,out,runlen, sortorder); 
	pthread_t sortingThread; 
 
	// Schema mySchema ("catalog", "lineitem"); 
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
	delete myLoserTree;
}

BigQ::~BigQ () {
}
