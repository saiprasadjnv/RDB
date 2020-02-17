#ifndef BIGQ_H
#define BIGQ_H
#include <pthread.h>
#include <iostream>
#include "Pipe.h"
#include "File.h"
#include "Record.h"
#include "LoserTree.h"

using namespace std;

class BigQ {
private: 
	typedef void * (*THREADFUNCPTR)(void *); 
public:
	LoserTree *myLoserTree; 
	BigQ (Pipe &in, Pipe &out, OrderMaker &sortorder, int runlen);
	~BigQ ();
};

#endif
