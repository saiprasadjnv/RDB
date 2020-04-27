#include <iostream>
#include <vector>
#include <string>
#include "SelectEngine.h" 
using namespace std; 

SelectEngine::SelectEngine(){

}

SelectEngine::~SelectEngine(){

}

int SelectEngine::Execute(){
    myQueryOptimizer = new QueryOptimizer(); 
    myQueryOptimizer->optimizeQuery(); 
    if(redirectOutputTo.compare("NONE")==0){
        myQueryOptimizer->printQueryPlanTree(); 
        return 1; 
    }
    queryTree = myQueryOptimizer->rootNode; 
    Pipe* tempPipe; 
    for(int i=0; i<=queryTree->pipeOut; i++){
        allPipes.push_back(new Pipe(100)); 
    }
    ExecuteQuery(queryTree); 
    if(redirectOutputTo.compare("STDOUT") != 0){
        WriteOut *myWriteout = new WriteOut; 
        FILE *outPutFile = fopen(redirectOutputTo.c_str(), "a"); 
        myWriteout->Use_n_Pages(8); 
        myWriteout->Run(*allPipes[queryTree->pipeOut], outPutFile, *queryTree->outSchema);  
        myWriteout->WaitUntilDone();
    }else{
        Record temp;
        Pipe *outPipe=allPipes[queryTree->pipeOut];
        Schema *outsch = queryTree->outSchema;  
        // queryTree->outSchema->Print();
        while(outPipe->Remove(&temp)!=0){
            temp.Print(outsch);
        }

    }
    return 1;
}

void SelectEngine::ExecuteQuery(TreeNode *currNode){ 
    if(currNode == nullptr || currNode==NULL){
        return; 
    }
    ExecuteQuery(currNode->left); 
    ExecuteQuery(currNode->right); 
    string tableName; 
    string schemaName;
    Schema* mySchema; 
    DBFile* myDBFile;  
    if(currNode->operation == SELECTFILE){
            SelectFile *mySelectFile = new SelectFile;  
            tableName = currNode->leftRel; 
            tableName.append(MY_BIN);
            schemaName = currNode->leftRel;
            schemaName.append(MY_SCHM); 
            // cout << schemaName << "---------------" << currNode->leftRel<< "\n"; 
            mySchema = new Schema((char*)schemaName.c_str(), (char*)currNode->leftRel.c_str()); 
            myDBFile = new DBFile(); 
            myDBFile->Open(tableName.c_str());
            mySelectFile->Use_n_Pages(8); 
            mySelectFile->Run(*myDBFile, *allPipes[currNode->pipeOut], *currNode->selOp, *currNode->literal); 
    }else if(currNode->operation == SELECTPIPE){
            SelectPipe *mySelectPipe = new SelectPipe; 
            mySelectPipe->Use_n_Pages(8); 
            mySelectPipe->Run(*allPipes[currNode->pipeLeft], *allPipes[currNode->pipeOut], *currNode->selOp, *currNode->literal); 
    }else if(currNode->operation== JOIN){
            Join *myJoin = new Join; 
            myJoin->Use_n_Pages(8); 
            myJoin->Run(*allPipes[currNode->pipeLeft],*allPipes[currNode->pipeRight],*allPipes[currNode->pipeOut], *currNode->selOp, *currNode->literal); 
    }else if( currNode->operation ==  DUPLICATEREMOVAL){
            DuplicateRemoval *mydistinct = new DuplicateRemoval; 
            mydistinct->Use_n_Pages(8); 
            mydistinct->Run(*allPipes[currNode->pipeLeft], *allPipes[currNode->pipeOut], *currNode->LeftinSchema); 
    }else if(currNode->operation == SUM){
            Sum *mySum = new Sum; 
            mySum->Use_n_Pages(8); 
            mySum->Run(*allPipes[currNode->pipeLeft], *allPipes[currNode->pipeOut], *currNode->nodeFunc); 
     }else if(currNode->operation == GROUPBY){ 
            GroupBy *myGroupby = new GroupBy; 
            myGroupby->Use_n_Pages(8); 
            myGroupby->Run(*allPipes[currNode->pipeLeft], *allPipes[currNode->pipeOut],*currNode->groupAtts, *currNode->nodeFunc); 
      }else if(currNode->operation == PROJECT){ 
            Project *myProject = new Project; 
            myProject->Use_n_Pages(8); 
            myProject->Run(*allPipes[currNode->pipeLeft], *allPipes[currNode->pipeOut], currNode->keepMe, currNode->numAttsInput, currNode->numAttsOutput); 
       }else if(currNode->operation == WRITEOUT){ 
              //No operation for now 
       }else{
            cerr << "Invalid Operation received for select !!\n"; 
            exit(1); 
       }
}