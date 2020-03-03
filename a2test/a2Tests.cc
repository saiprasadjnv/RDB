/*
*This is a Google Test file, we perform unit test cases for all the functions related to assignment two (Loser Tree and BigQ). 
*SortTest is the Fixture Class for this test, it initializes all the required objects for each test case 
*/
#include <stdio.h>
#include <pthread.h>
#include "BigQ.h"
#include "DBFile.h"
#include "gtest/gtest.h"
class SortTest : public ::testing::Test{
  protected:
    typedef struct {
        Pipe *pipe;
        OrderMaker *order;
        bool print;
        bool write;
    }testutil;
    Pipe *in; 
    Pipe *out; 
    DBFile *sortedFile; 
    Operand *myOperand; 
    ComparisonOp *myComparisonOp;  
    OrList *myOrlist; 
    AndList *finallist;  
    Schema *mySchema; 
    OrderMaker *sortorder; 
    Record *sortedRecords; 
    ComparisonEngine ceng; 
    void SetUp(){
        Record *temp = new Record; 
        in = new Pipe(10); 
        out = new Pipe(10);  
        sortorder = new OrderMaker; 
        sortedFile = new DBFile; 
        sortedFile->Open("sortedFile.bin"); 
        sortedRecords = new Record[10]; 
        myOperand = new Operand {4, (char *)"c_phone"}; 
        myComparisonOp = new ComparisonOp {3, myOperand, myOperand};  
        myOrlist = new OrList {myComparisonOp, nullptr}; 
        finallist = new AndList {myOrlist, nullptr};  
        CNF sort_pred; 
        mySchema = new Schema ("catalog", "customer"); 
        Record literal; 
        sort_pred.GrowFromParseTree (finallist, mySchema ,literal); 
	      OrderMaker dummy;
	    	sort_pred.GetSortOrders (*sortorder, dummy);
    }
    void TearDown(){
        delete myOperand; 
        delete myComparisonOp;
        delete myOrlist; 
        delete finallist; 
        delete mySchema; 
        delete sortorder; 
    }

    static void *producer (void *arg) {
        Pipe *myPipe = (Pipe *) arg;
        Record temp;
        int counter = 0;
        DBFile *testFile = new DBFile; 
        testFile->Open("testFile.bin");
        testFile->MoveFirst();
        while (testFile->GetNext (temp) == 1) {
          counter += 1;
   
          myPipe->Insert (&temp);
        }
        testFile->Close();
        myPipe->ShutDown ();
    }
};

// TEST_F(SortTest, simple_test){  
//   pthread_t thread1;
// 	pthread_create (&thread1, NULL, producer, (void *)in);
//   BigQ *bq = new BigQ(*in, *out, *sortorder, 1); 
//   DBFile *sortedFile = new DBFile; 
//   sortedFile->Open("sortedFile.bin");
//   Record temp1; 
//   Record temp2; 
//   int res =1;  
//   while(1){
//       if(out->Remove(&temp1)==1){
//         temp1.Print(mySchema); 
//           if(sortedFile->GetNext(temp2)==0)
//               res = 0; 
//           else if(ceng.Compare(&temp1, &temp2, sortorder) != 0)
//               res =0;
//           else
//           {
//             temp1.Print(mySchema); 
//           }
          
//       }
//       else{
//         sortedFile->Close();
//         out->ShutDown();
//       }
//   }
//   EXPECT_EQ(res, 1);
// }


/*
* This is a test case for Loser Tree Initializer, After the tree is initialized, a new winner (Smallest record node 
* among the leaf nodes is generated)
*/
TEST_F(SortTest, initializeTest){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree = new LoserTree(*in, *out, 1, *sortorder); 
    myLoserTree->setupPass1(); 
    myLoserTree->initialize(); 
    long res = myLoserTree->currentWinner->recordIndex; 
    pthread_join(thread1, NULL); 
    delete myLoserTree; 
    EXPECT_EQ(res,2);
}

/*
* This test case is to compare two records corresponding to the leaf nodes in the Loser tree. We read two records from 
* the input pipe and compare. 
*/
TEST_F(SortTest, CompareRecordsRightIsSmall){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree= new LoserTree(*in, *out, 1, *sortorder); 
    Record *temp1 = new Record; 
    Record *temp2 = new Record;
    in->Remove(temp1);
    in->Remove(temp2); 
    int res = myLoserTree->compare(*temp1, *temp2); 
    pthread_join(thread1, NULL); 
    delete temp1;
    delete temp2; 
    delete myLoserTree; 
    EXPECT_EQ(res,0); 
}

/*
* This test case is to compare two records corresponding to the leaf nodes in the Loser tree. We read two records from 
* the input pipe and compare. 
*/
TEST_F(SortTest, CompareRecordsLeftIsSmall){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree= new LoserTree(*in, *out, 1, *sortorder); 
    Record *temp1 = new Record; 
    Record *temp2 = new Record;
    in->Remove(temp1);
    in->Remove(temp2); 
    int res = myLoserTree->compare(*temp2, *temp1); 
    pthread_join(thread1, NULL); 
    delete temp1;
    delete temp2; 
    delete myLoserTree; 
    EXPECT_EQ(res,1); 
}

/*
* This test case is to compare two records corresponding to the leaf nodes in the Loser tree. We read two records from 
* the input pipe and compare. 
*/
TEST_F(SortTest, CompareRecordsAreEqual){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree= new LoserTree(*in, *out, 1, *sortorder); 
    Record *temp1 = new Record; 
    in->Remove(temp1);
    int res = myLoserTree->compare(*temp1, *temp1); 
    pthread_join(thread1, NULL); 
    delete temp1;
    delete myLoserTree; 
    EXPECT_EQ(res,0); 
}

/*
* This test case is to compare two tree Nodes in the Loser tree. We perform setup tree to create the tree and compare 
* two tree nodes. 
*/
TEST_F(SortTest, CompareTreeNodesRightisSmall){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree = new LoserTree(*in, *out, 1, *sortorder); 
    myLoserTree->setupPass1(); 
    int index1 =0, index2 = 1; 
    LoserTree::LoserNode node1 = myLoserTree->myTree[index1 + 4];
    LoserTree::LoserNode node2 = myLoserTree->myTree[index2 + 4];
    int res = myLoserTree->compare(node1, node2); 
    pthread_join(thread1, NULL); 
    delete myLoserTree; 
    EXPECT_EQ(res, 1); 
}

/*
* This test case is to compare two tree Nodes in the Loser tree. We perform setup tree to create the tree and compare 
* two tree nodes. 
*/
TEST_F(SortTest, CompareTreeNodesLeftisSmall){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree = new LoserTree(*in, *out, 1, *sortorder); 
    myLoserTree->setupPass1(); 
    int index1 =0, index2 = 1; 
    LoserTree::LoserNode node1 = myLoserTree->myTree[index1 + 4];
    LoserTree::LoserNode node2 = myLoserTree->myTree[index2 + 4];
    int res = myLoserTree->compare(node2, node1); 
    pthread_join(thread1, NULL); 
    delete myLoserTree; 
    EXPECT_EQ(res, 1); 
}

/*
* This test case is to compare two tree Nodes in the Loser tree. We perform setup tree to create the tree and compare 
* two tree nodes. 
*/
TEST_F(SortTest, CompareTreeNodesAreEqual){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree = new LoserTree(*in, *out, 1, *sortorder); 
    myLoserTree->setupPass1(); 
    int index1 =0; 
    LoserTree::LoserNode node1 = myLoserTree->myTree[index1 + 4];
    int res = myLoserTree->compare(node1, node1); 
    pthread_join(thread1, NULL); 
    delete myLoserTree; 
    EXPECT_EQ(res, 1); 
}


/*
* This test case is perform a "PLAY" operation on the nodes. Each time a match is played at leaf node, a new winner 
* emerges from the tree as the smallest record in the tree. 
*/
TEST_F(SortTest, PlayaMatch){
    pthread_t thread1;
	  pthread_create (&thread1, NULL, producer, (void *)in);
    LoserTree *myLoserTree = new LoserTree(*in, *out, 1, *sortorder); 
    myLoserTree->setupPass1(); 
    myLoserTree->initialize(); 
    long treeSize = myLoserTree->treeSize; 
    long firstWinner = TREENODEINDEX(myLoserTree->currentWinner->recordIndex);
    myLoserTree->myTree[firstWinner].runNumber = LONG_MAX; 
    long winner = myLoserTree->play(PARENT(firstWinner), myLoserTree->myTree[firstWinner]); 
    printf("newWinner position %ld treeSize: %ld \n", winner, myLoserTree->treeSize);
    long newWinner = myLoserTree->myTree[winner].recordIndex;
    EXPECT_EQ(newWinner, 4); 
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
