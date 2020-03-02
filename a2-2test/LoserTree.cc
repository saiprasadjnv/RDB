#include <iostream>
#include "LoserTree.h"

LoserTree::LoserTree(Pipe &input, Pipe &output, long runlength, OrderMaker &sortOrder){
    runLen = runlength * 600;
    treeSize = runLen -1; 
    myTree = new LoserNode[treeSize]; 
    records = new Record*[treeSize - INTERNALNODES(treeSize)];
    mySchema = new Schema("catalog", "customer");
    pagesPerRun = new long[150]; 
    tempFile = new File;
    tempPage = new Page;  
    whichPage =0; 
    currentWinner = new LoserNode; 
    currentWinner->recordIndex = -1;
    currentWinner->runNumber = LONG_MAX; 
    in = &input; 
    out= &output;
    sortorder = &sortOrder;
    //Initialize Tree internal nodes
    for(int i=0; i< INTERNALNODES(treeSize); i++){
        myTree[i].recordIndex = -1; 
        myTree[i].runNumber = LONG_MAX;
    }

    for(int i=INTERNALNODES(treeSize); i<treeSize; i++){
        myTree[i].recordIndex = i - INTERNALNODES(treeSize); 
        myTree[i].runNumber = LONG_MAX; 
    }
    //Initialize pagesPerRun
    for(int i=0; i<150; i++){
        pagesPerRun[i] = 0; 
    }
    for(int i=0;i<(treeSize-(INTERNALNODES(treeSize)));i++){
        myTree[TREENODEINDEX(i)].runNumber = LONG_MAX;
        records[i]=nullptr;

    }
}

LoserTree::~LoserTree(){
    // delete[] records;
    // delete[] myTree;
    delete[] pagesPerRun; 
    delete mySchema; 
    tempFile->Close();
    delete tempPage;
    delete tempFile;
    delete[] pageBuffers;
    delete currentWinner;
}


void *LoserTree::sort(void * args){
    // printf("IN sort");
    LoserTree *thisNode = (LoserTree *)args; 
    thisNode->pass1(); 
    thisNode-> pass2(); 
    return NULL;
}

void LoserTree::initialize(){
    int numOfRecords = treeSize - (INTERNALNODES(treeSize));
    for (int i = 0; i < numOfRecords; i++)
    {
      long res=play(PARENT(TREENODEINDEX(i)), myTree[TREENODEINDEX(i)]);
      //printf("### winner:%d, at %d ###\n",myTree[res].recordIndex,res);
       currentWinner->recordIndex = myTree[res].recordIndex;
       currentWinner->runNumber = myTree[res].runNumber;
    }
    //records[currentWinner->recordIndex]->Print(mySchema); 
}   

void LoserTree::pass1(){
    // printf("Opening tempFile\n");
    tempFile->Open(0,"tempFile.bin");
    setupPass1(); 
    initialize();
   try{
       Record temporaryWinnerRecord;   
       while(1){
            if(currentWinner->runNumber==LONG_MAX){
                writeDirtyPage();
                cleanup();
                break;
            }
            if(currentWinner->recordIndex!=-1){
                currWinnerRecord=records[currentWinner->recordIndex];
                temporaryWinnerRecord.Copy(currWinnerRecord);
                Add(*currWinnerRecord);
                Record temp;
                    int resForPipeRetreival=in->Remove(&temp);
                    if(records[currentWinner->recordIndex]!=nullptr){
                        delete records[currentWinner->recordIndex];
                    }
                    if(resForPipeRetreival==0){
                        records[currentWinner->recordIndex]=nullptr;
                    }else{
                    records[currentWinner->recordIndex]= new Record();
                    records[currentWinner->recordIndex]->Consume(&temp);
                    }
                    if(records[currentWinner->recordIndex]!=nullptr && compare(*records[currentWinner->recordIndex],temporaryWinnerRecord)){
                        myTree[TREENODEINDEX(currentWinner->recordIndex)].runNumber=currRunNumber+1;

                    }else if(records[currentWinner->recordIndex]==nullptr){
                        myTree[TREENODEINDEX(currentWinner->recordIndex)].runNumber=LONG_MAX;
                    }
                    long newWinnerPos=play(PARENT(TREENODEINDEX(currentWinner->recordIndex)),myTree[TREENODEINDEX(currentWinner->recordIndex)]);
                    currentWinner->recordIndex=myTree[newWinnerPos].recordIndex;
                    currentWinner->runNumber=myTree[newWinnerPos].runNumber;
                    if(currentWinner->runNumber==currRunNumber+1){
                        writeDirtyPage();
                        currRunNumber+=1;
                    }
            }
       } 
   }catch(exception ex){
       ex.what();
   }
}

void LoserTree::pass2(){
    int winnerIndex; 
    long res;
    int currentBuffer[150];  
    Record temp; 
    setupPass2(); 
    initialize(); 
    int maxPages = tempFile->GetLength() - 1;
    currentBuffer[0]=0; 
    for(int i=1; i<150; i++){
        currentBuffer[i] = pagesPerRun[i-1]; 
    }   
    //records[winnerIndex]->Print(mySchema); 
    while(1){
        winnerIndex = currentWinner->recordIndex;
        if(currentWinner->runNumber == LONG_MAX){
            break; 
        }
        out->Insert(records[winnerIndex]);
        int result = pageBuffers[winnerIndex]->GetFirst(&temp); 
        if(result==0){
            currentBuffer[winnerIndex]++;
            if(currentBuffer[winnerIndex] == pagesPerRun[winnerIndex])
            {
                pageBuffers[winnerIndex]->EmptyItOut();
                delete records[winnerIndex]; 
                records[winnerIndex] = nullptr; 
                myTree[TREENODEINDEX(winnerIndex)].runNumber = LONG_MAX; 
            }
            else{
                pageBuffers[winnerIndex]->EmptyItOut();
                if(currentBuffer[winnerIndex] < maxPages){
                    tempFile->GetPage(pageBuffers[winnerIndex], currentBuffer[winnerIndex]);
                    delete records[winnerIndex];
                    records[winnerIndex] = new Record; 
                    if(pageBuffers[winnerIndex]->GetFirst(records[winnerIndex]) ==0 ){
                        delete records[winnerIndex]; 
                        records[winnerIndex] = nullptr;
                        myTree[TREENODEINDEX(winnerIndex)].runNumber = LONG_MAX;
                    } 
                }
                else{
                    delete records[winnerIndex]; 
                    records[winnerIndex] = nullptr;
                    myTree[TREENODEINDEX(winnerIndex)].runNumber = LONG_MAX;
                }
            }
        }
        else{
            if(records[winnerIndex] != nullptr){
                delete records[winnerIndex];
            }
            records[winnerIndex] = new Record; 
            records[winnerIndex]->Consume(&temp);  
        }
        res = play(PARENT(TREENODEINDEX(winnerIndex)), myTree[TREENODEINDEX(winnerIndex)]);
       // printf("### winner:%d, at %d ###\n",myTree[res].recordIndex,res);
        currentWinner->recordIndex = myTree[res].recordIndex;
        currentWinner->runNumber = myTree[res].runNumber; 
    }
    cleanup(); 
}


long LoserTree::play(long j, LoserTree::LoserNode &player2){
  // printf(" **%d** ", j);
    LoserNode *winner = new LoserNode; 
    long winnerIndex=0;
    long res;
    if (j == 0)
    {
        if (myTree[j].recordIndex == -1)
        {
            myTree[j] = player2; 
            winner->recordIndex = player2.recordIndex;
            winner->runNumber = player2.runNumber;  
        }
        else if(compare(myTree[j],player2)){
            winner->recordIndex = myTree[j].recordIndex; 
            winner->runNumber = myTree[j].runNumber; 
            myTree[j] = player2; 
        }

        else
        {
            winner->recordIndex = player2.recordIndex; 
            winner->runNumber = player2.runNumber; 
        }
        winnerIndex=TREENODEINDEX(winner->recordIndex);
        delete winner;
        // return TREENODEINDEX(winner->recordIndex) ; 
        return winnerIndex;        
    }
    if (myTree[j].recordIndex == -1)
    {
        winner->recordIndex = player2.recordIndex;
        winner->runNumber = player2.runNumber; 
        myTree[j] = player2; 
        delete winner;
        return TREENODEINDEX(player2.recordIndex); 
    }
    else{
        if(compare(myTree[j], player2)){
            winner->recordIndex = myTree[j].recordIndex;
            winner->runNumber = myTree[j].runNumber;  
            myTree[j] = player2; 
            res = play(PARENT(j), *winner); 
            winner->recordIndex = myTree[res].recordIndex;
            winner->runNumber = myTree[res].runNumber;
        }
        else{
            res = play(PARENT(j), player2); 
            winner->recordIndex = myTree[res].recordIndex;
            winner->runNumber = myTree[res].runNumber;  
        }
    }
        winnerIndex=TREENODEINDEX(winner->recordIndex);
        delete winner;
        // return TREENODEINDEX(winner->recordIndex) ; 
        return winnerIndex;
}



/*
*This method compares two nodes in the Loser Tree, returns 1 if node1<=node2, otherwise 0. 
*/
int LoserTree::compare(LoserTree::LoserNode &node1, LoserTree::LoserNode &node2){
    ComparisonEngine comp; 
    if((node1.runNumber < node2.runNumber) || (node1.runNumber==LONG_MAX && node2.runNumber==LONG_MAX)){
        return 1; 
    }
    else if((node2.runNumber < node1.runNumber) || (node1.runNumber==LONG_MAX && node2.runNumber==LONG_MAX)){
        return 0; 
    }
    // int res = comp.Compare(&records[node1.recordIndex], &records[node2.recordIndex], sortorder); 
    int res = comp.Compare(records[node1.recordIndex], records[node2.recordIndex], sortorder);
    if(res<0){
        return 1; 
    }
    else{

        return 0; 

    }
}

/*
* This method compares two Records, returns 1 if rec1<rec2, otherwise 0. 
*/
int LoserTree::compare(Record &rec1, Record &rec2){
    ComparisonEngine comp; 
    int res = comp.Compare(&rec1, &rec2, sortorder); 
    if(res<0){
        return 1; 
    }
    else{
        return 0; 
    }
}

int LoserTree::Add (Record &rec) {
	//rec.Print(mySchema); 
     if(tempPage->Append(&rec)==0){
            tempFile->AddPage(tempPage, whichPage);
           // printf("Added page: %d\n", whichPage); 
            whichPage++;
            pagesPerRun[currRunNumber]++;
            tempPage->EmptyItOut();
            tempPage->Append(&rec);
            //printf("After append\n");          
        }
    numRecsInRun +=1; 
    return 1;
}


int LoserTree::writeDirtyPage(){
        tempFile->AddPage(tempPage, whichPage);
       // printf("Added Page in dirty Read: %d \n", whichPage);
        whichPage++;
        pagesPerRun[currRunNumber]++;
       // printf("pages per run in currRunNumber %d - %d \n", currRunNumber, pagesPerRun[currRunNumber]);
        tempPage->EmptyItOut();
        numRecsInRun=0;
        // currRunNumber+=1;
    return 1;
}

void LoserTree::print(){
   for(int i=0; i< treeSize; i++)
        printf(" Node %d - [%ld, %ld] ", i, myTree[i].recordIndex, myTree[i].runNumber);
    printf("\n");
}   

void LoserTree::setupPass1(){
    Record temp;
    int numOfRecords = treeSize - (INTERNALNODES(treeSize));
    for (int i = 0; i < numOfRecords; i++) {
        int result = in->Remove(&temp); 
       if(result==1){
          // printf("Read record at i=%d, updating treeindex: %ld\n", i, TREENODEINDEX(i));
           //temp.Print(mySchema);
            myTree[TREENODEINDEX(i)].runNumber = 0; 
            if(records[i]!=nullptr){

            delete records[i];
            }
            //printf("Read this record: \n");
            //temp.Print(mySchema);
            records[i]= new Record();
            records[i]->Consume(&temp);
       }
       else{
           records[i] = nullptr; 
       }
    }
}

void LoserTree::setupPass2(){
    int numOfruns = 0; 
    int i=0; 
    // printf("Opening tempFile pass2\n");
    tempFile->Open(1,"tempFile.bin");
    while(pagesPerRun[i]!=0){
        if(i>0)
            pagesPerRun[i] += pagesPerRun[i-1];
        numOfruns++;
        i++;
    }
    treeSize = (numOfruns *2) -1; 
    myTree = new LoserNode[treeSize]; 
    records = new Record* [numOfruns]; 
    pageBuffers = new Page* [numOfruns]; 
    for(i=0; i< treeSize; i++){
        myTree[i].recordIndex = -1; 
        myTree[i].runNumber = LONG_MAX;
    }
    for(i=0; i< numOfruns; i++){
        myTree[TREENODEINDEX(i)].recordIndex = i;
        pageBuffers[i] = new Page;
        records[i] = new Record; 
        if(i>0){
            tempFile->GetPage(pageBuffers[i],pagesPerRun[i-1]); 
        }
        else
        {
            tempFile->GetPage(pageBuffers[i], 0);    
        }
        if(pageBuffers[i]->GetFirst(records[i])==0){
            delete records[i]; 
            records[i] = nullptr; 
        } 
        else{
            myTree[TREENODEINDEX(i)].runNumber = 0; 
        }
    }


}

void LoserTree::cleanup(){
    tempFile->Close();
    tempPage->EmptyItOut(); 
    treeSize=0; 
    currRunNumber=0;
    delete[] records;
    delete[] myTree;
    // delete[] pageBuffers; 
}