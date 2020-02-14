#include <iostream>
#include "Loser.h"
using namespace std; 

LoserTree::LoserTree(Record *recs, int runLenInPages, OrderMaker &mysortorder, Pipe input, Pipe output){
    printf("********Iniside tree constructor*********\n");
    whichPage =0; 
    records = recs; 
    sortorder = &mysortorder; 
    runLen = runLenInPages * 610; 
    treeOrder = runLen/2; 
    tempFile.Open(0, "tempFile.bin");
    tempFile.Close();
    tempFile.Open(1,"tempFile.bin"); 
    printf("File length after open file : %d \n", tempFile.GetLength());
    myTree = new LoserNode[runLen -1]; 
    pagesForRun = new int[150];
    in = &input; 
    out = &output; 
    Record temp; 
    printf("Reading from the input pipe after temp file is created\n");
    in->Remove(&temp);  
    Schema mySchema("catalog", "orders"); 
    temp.Print(&mySchema);
    for(int i=0; i<150; i++)
        pagesForRun[i] = 0; 
}

void LoserTree::initialize(){
    printf("**********Initialize Tree************\n");
    for (int i = 0; i < treeOrder-1; i++)
    {
        myTree[i].index = -1;
        myTree[i].runNumber = 0; 
    }  
    for(int i= treeOrder-1; i<runLen-1; i++){
        printf(" %d----%d,", i, i-treeOrder +1);
        myTree[i].index = i - treeOrder +1 ; 
        myTree[i].runNumber = 0; 
        currentWinner = play(PARENT(i), myTree[i]);
    }
    printf("\n\n");
    for(int i=0; i<runLen; i++)
        printf(" %d-%d", i, myTree[i].index);
    printf("\n********After printing the tree*********\n");
    Schema mySchema ("catalog", "orders");
    records[currentWinner.index].Print(&mySchema);
}

LoserTree::LoserNode LoserTree::play(int j, LoserTree::LoserNode player2){
    LoserNode loser;
    LoserNode winner; 
    if (j == 0)
    {
        if (myTree[j].index == -1)
        {
            myTree[j] = player2; 
            winner = player2; 
        }

        else if(compare(myTree[j],player2)){
            winner = myTree[j]; 
            myTree[j] = player2; 
        }
        else
        {
            winner = player2; 
        }
        return winner;         
    }

    if (myTree[j].index == -1)
    {
        winner = player2; 
        myTree[j] = player2; 
        return player2; 
    }

    else{
        if(compare(myTree[j], player2)){
            winner = myTree[j]; 
            myTree[j] = player2; 
            winner = play(PARENT(j), winner); 
        }
        else{
            winner = play(PARENT(j), player2); 
        }
    }
     return winner; 
}

void LoserTree::sort(){
    initialize(); 
    pass1(); 
   // pass2();
}

void LoserTree::pass1(){
    printf("\n**********Inside pass1*************\n");
    int currRunNumber = 0; 
    int numRecsInRun = 0; 
    int index = currentWinner.index; 
    printf("******printing index - %d *****", index);
    Schema mySchema("catalog", "orders"); 
    while (1)
    {
        printf("************Inside while loop *********\n");
        printf("CurrWinner %d - %d \n", currentWinner.index, currentWinner.runNumber);
        if(numRecsInRun == runLen){
            printf("************Inside first if  *********\n");
            tempFile.AddPage(&tempPage, whichPage);
            whichPage++; 
            pagesForRun[currRunNumber] += 1; 
            tempPage.EmptyItOut();  
            tempPage.Append(&records[currentWinner.index]);
            currRunNumber++; 
            pagesForRun[currRunNumber] = whichPage +1; 
            numRecsInRun =0; 
        }
        if(currentWinner.runNumber > currRunNumber){
            printf("************Inside second if  *********\n");
            tempFile.AddPage(&tempPage, whichPage);
            whichPage++;
            pagesForRun[currRunNumber] += 1; 
            tempPage.EmptyItOut();  
            tempPage.Append(&records[currentWinner.index]);
            currRunNumber++; 
            pagesForRun[currRunNumber] = whichPage +1; 
            numRecsInRun = 0; 
            
        }
        printf("\n*******Before Append ***********\n");
        if(tempPage.Append(&records[currentWinner.index])==0){
            printf("\n *********Adding new Page to the temp file*********\n"); 
            tempFile.AddPage(&tempPage, whichPage); 
            whichPage++;
            pagesForRun[currRunNumber] += 1; 

            tempPage.EmptyItOut();
            tempPage.Append(&records[currentWinner.index]); 
        }
          printf("\n*******After Append ***********\n");
        numRecsInRun++; 

        printf("************After adding first record  *********\n");
        Record currWinnerRecord = records[index]; 
        //SuckNewNode here into  records[index] 
        //If there are no records left, break this loop... 
         printf("************Before next read *********\n");
         Record temp1;
         int res1 = in->Remove(&temp1); 
         printf("res : %d\n", res1);
         if(res1==0)
            break; 
         temp1.Print(&mySchema); 
         records[index].Consume(&temp1); 
         records[index].Print(&mySchema);
         printf("************After next read *********\n");
        LoserNode newNode = {currentWinner.index,currentWinner.runNumber};
       // printf("---newNode %d - %d \n",newNode.index,newNode.runNumber);
        // printf("-- %d",5000);
        int treeIndex = index + treeOrder -1; 
        if(compare(records[index], currWinnerRecord)){
            newNode.runNumber = currRunNumber +1; 
            myTree[treeIndex] = newNode; 
        }


        // currWinnerRecord.Print(&mySchema); 
        play(PARENT(treeIndex), newNode); 
        printf("\n********Added the first Record**********\n");

    }

}

/*
*This method compares two nodes in the Loser Tree, returns 1 if node1<=node2, otherwise 0. 
*/
int LoserTree::compare(LoserTree::LoserNode node1, LoserTree::LoserNode node2){
    ComparisonEngine comp; 
    if(node2.runNumber > node1.runNumber){
        return 1; 
    }
    int res = comp.Compare(&records[node1.index], &records[node2.index], sortorder); 
    if(res<1){
        return 1; 
    }
    else{
        return 0; 
    }
}

/*
* This method compares two Records, returns 1 if rec1<=rec2, otherwise 0. 
*/
int LoserTree::compare(Record &rec1, Record &rec2){
    ComparisonEngine comp; 
    int res = comp.Compare(&rec1, &rec2, sortorder); 
    if(res<1){
        return 1; 
    }
    else{
        return 0; 
    }
}

