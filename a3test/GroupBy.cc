#include "RelOp.h"

  struct BigQArgs{
        Pipe *inPipe; 
        Pipe *outPipe; 
        OrderMaker* sortOrder; 
        long bufferSize; 
  };
void* bigQThreadGroupBy(void* myargs){
    BigQArgs* inArgs = (BigQArgs*)myargs;  
    BigQ bigQ (*inArgs->inPipe, *inArgs->outPipe,*inArgs->sortOrder,inArgs->bufferSize); 
    delete inArgs; 
    return NULL; 
}

void* GroupByThread(void* myargs){  
    struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        OrderMaker *groupAtts; 
        Function* computeMe; 
        long bufferSize;
    };
   
    Args* inArgs = (Args*)myargs; 
    int *sortAttributes = new int[30]; 
    Type *sortAttTypes = new Type[30]; 
    int numAttsinSort; 
    inArgs->groupAtts->getAttributes(sortAttributes, sortAttTypes, numAttsinSort); 
    Pipe* bigQPipe = new Pipe(100);  
    BigQArgs* bigQArgs = new BigQArgs; 
    bigQArgs->inPipe =  inArgs->inPipe;
    bigQArgs->outPipe = bigQPipe;
    bigQArgs->sortOrder = inArgs->groupAtts;
    bigQArgs->bufferSize =  inArgs->bufferSize;
    char* result = new char[50];
    pthread_t bigQthread; 
    pthread_create(&bigQthread, NULL, bigQThreadGroupBy, (void*)bigQArgs);
    Record* temp = new Record; 
    Record *rec1 = nullptr; 
    int intRes = 0;
    int finalIntRes = 0; 
    double doubleRes = 0.0; 
    double finalDoubleRes = 0.0; 
    Attribute IA = {"int", Int}; 
    Schema sum_sch_int ("sum_sch_int", 1, &IA);  
    Attribute DA = {"double", Double}; 
    Schema sum_sch_double ("sum_sch_double", 1, &DA); 
    ComparisonEngine ceng; 
    Type res; 
    Record *finalRecord = new Record; 
    //attstoKeep for mergeRecords 
    int *mergeOrder = new int[31]; 
    mergeOrder[0] = 0; 
    for(int i=0; i<numAttsinSort; i++){
        mergeOrder[i+1] = sortAttributes[i]; 
    }
    Schema mySchema ("catalog", "supplier");
    while (bigQPipe->Remove(temp))
    { 
        
      
        if (rec1 == nullptr)
        {
            rec1 = new Record; 
            rec1->Consume(temp); 
            delete temp; 
            temp = new Record; 
            continue; 
        }
        res = inArgs->computeMe->Apply(*rec1, intRes, doubleRes); 
        if(res==Int){
            finalIntRes += intRes; 
        }
        else{
            finalDoubleRes += doubleRes; 
        }
        if(ceng.Compare(rec1,temp,inArgs->groupAtts)!=0){
            if(res==Int){
                string  intRes = to_string(finalIntRes); 
                strcpy(result, intRes.c_str());
                strcat(result, "|"); 
                Record resultint; 
                resultint.ComposeRecord(&sum_sch_int, (const char*)result);
                finalRecord->MergeRecords(&resultint, rec1, 1, numAttsinSort, mergeOrder, 1+numAttsinSort, 1);
                Attribute atts1[2] = {IA, IA}; 
                Schema sum_sch1 ("sum_sch1", 2, atts1);
                inArgs->outPipe->Insert(finalRecord); 
                finalIntRes = 0; 
                delete finalRecord; 
                finalRecord = new Record; 
                delete[] result;
                result = new char[50];
            }
            else{
                string  doubleRes = to_string(finalDoubleRes); 
                strcpy(result, doubleRes.c_str());
                strcat(result, "|"); 
                Record resultDouble; 
                resultDouble.ComposeRecord(&sum_sch_double, (const char*)result);
                finalRecord->MergeRecords(&resultDouble, rec1, 1, numAttsinSort, mergeOrder, 1+numAttsinSort, 1);
                inArgs->outPipe->Insert(finalRecord); 
                finalDoubleRes = 0.0; 
                delete finalRecord; 
                finalRecord = new Record; 
                delete[] result;
                result = new char[50];
            }
        }
        delete rec1; 
        rec1 = new Record; 
        rec1->Consume(temp); 
        delete temp; 
        temp = new Record; 
    }

    if(rec1 != nullptr){
        res = inArgs->computeMe->Apply(*rec1, intRes, doubleRes); 
        if(res==Int){
            finalIntRes += intRes; 
            string  intRes = to_string(finalIntRes); 
            strcpy(result, intRes.c_str());
            strcat(result, "|"); 
            Record resultint; 
            resultint.ComposeRecord(&sum_sch_int, (const char*)result);
            finalRecord->MergeRecords(&resultint, rec1, 1, numAttsinSort, mergeOrder, 1+numAttsinSort, 1);
            Attribute atts1[2] = {IA, IA}; 
            Schema sum_sch1 ("sum_sch1", 2, atts1);
            inArgs->outPipe->Insert(finalRecord); 
        }
        else{
            finalDoubleRes += doubleRes; 
            string  doubleRes = to_string(finalDoubleRes); 
            strcpy(result, doubleRes.c_str());
            strcat(result, "|"); 
            Record resultDouble; 
            resultDouble.ComposeRecord(&sum_sch_double, (const char*)result);
            finalRecord->MergeRecords(&resultDouble, rec1, 1, numAttsinSort, mergeOrder, 1+numAttsinSort, 1);
            inArgs->outPipe->Insert(finalRecord); 
        }
    }
   

    inArgs->outPipe->ShutDown();
    delete inArgs; 
    delete temp; 
    delete rec1; 
    delete[] result;
    delete finalRecord; 
    return NULL; 
}

void GroupBy::Run (Pipe &inPipe, Pipe &outPipe, OrderMaker &groupAtts, Function &computeMe) {
     struct Args{
        Pipe* inPipe; 
        Pipe* outPipe; 
        OrderMaker* groupAtts; 
        Function* computeMe; 
        long bufferSize; 
    };
 
    Args* inArgs = new Args; 
    inArgs->inPipe = &inPipe;
    inArgs->outPipe = &outPipe;
    inArgs->groupAtts = &groupAtts;
    inArgs->computeMe = &computeMe;
    inArgs->bufferSize = bufferSize;
    pthread_create(&thread, NULL, GroupByThread, (void*)inArgs);

}

void GroupBy::WaitUntilDone () {
	pthread_join (thread, NULL);
}

void GroupBy::Use_n_Pages (int runlen) {
    bufferSize = runlen; 
}

GroupBy::GroupBy(){
    bufferSize = 100; 
}