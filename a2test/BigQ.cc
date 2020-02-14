#include "BigQ.h"

BigQ :: BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen) {
	// read data from in pipe sort them into runlen pages

    // construct priority queue over sorted runs and dump sorted data 
 	// into the out pipe

    // finally shut down the out pipe
	printf("\n ********* Inside BIgQ constructor *********\n");
	records = new Record[runlen * 610]; 
	Schema mySchema ("catalog", "orders");
	int i =0; 
	while (1)
	{
		if(0==in.Remove(&records[i]))
			break; 
		//records[i].Print(&mySchema);
		i++; 
		if(i==runlen*610)
			break; 
		
	}
	printf("****************Collected some records************\n");
	LoserTree sorter(records, runlen, sortorder, in, out); 	
	printf("**************After LoserTree is constructed *************\n");
	sorter.sort(); 
	out.ShutDown ();
}

BigQ::~BigQ () {
}
