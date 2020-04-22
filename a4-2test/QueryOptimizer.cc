#include "QueryOptimizer.h"
#include <iostream>
#include <algorithm>
using namespace std; 

void printAndList1(struct AndList *currAndlist){
    // cout << "currAndList address in print: "<< currAndlist << "\n";
    if(currAndlist == NULL){
        cout << "Empty andlist\n"; 
    }
	while(currAndlist != NULL){
		struct OrList* currOrlst = currAndlist->left; 
		while(currOrlst != NULL){
			cout << "Left Operand: " << currOrlst->left->left->value << " code: " << currOrlst->left->code << " Right Operand: " << currOrlst->left->right->value << "; ";   
            // cout << "Left Operand: " << currOrlst->left->left->value << " code: " << currOrlst->left->code << " Right Operand: " << "; ";   
			currOrlst = currOrlst->rightOr; 
		}
		cout << "\n"; 
		currAndlist = currAndlist->rightAnd; 
	}
}

QueryOptimizer::QueryOptimizer(){
    extern struct TableList* tables; 
    extern struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
    extern struct AndList *boolean; // the predicate in the WHERE clause
    extern struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
    extern struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
    extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
    extern int distinctFunc; 
    myQueryParams = new queryParsedInfo; 
    myQueryParams->tables = tables;
    myQueryParams->finalFunction = finalFunction; 
    myQueryParams->boolean = boolean;
    myQueryParams->groupingAtts = groupingAtts; 
    myQueryParams->attsToSelect = attsToSelect; 
    myQueryParams->distinctAtts = distinctAtts; 
    myQueryParams->distinctFunc; 
    myStats = new Statistics; 
    // cout << "Initializing Stats\n"; 
    myStats->Read("InitalStatitics.txt"); 
    // cout << "Initializing Stats Finished!!\n";  
    // myStats->PrintStatistics(); 
    // GetAndProcessRelationInfo(); 
    // GetJoinsInfo(); 
}

QueryOptimizer::QueryOptimizer(void *args){
    myQueryParams = (queryParsedInfo*)args; 
    myStats = new Statistics; 
    myStats->Read("InitalStatitics.txt"); 
    // GetAndProcessRelationInfo(); 
    // GetJoinsInfo(); 
}


//This function gets all information regarding all the relations involved in the query. 
//Initializes the Relations map with the corresponding "Select CNF". 
//Puts nullptr if there is no CNF correspondng to a relation. It also creates the corresponding aliases in Statistics.
void QueryOptimizer::GetAndProcessRelationInfo(){
    struct  TableList* currRel = myQueryParams->tables; 
    while(currRel != NULL){ 
        string relName(currRel->tableName); 
        if(currRel->aliasAs==NULL){
            currRel->aliasAs=currRel->tableName;
        }
        string alias(currRel->aliasAs);  
        pair<string, struct AndList*> newRel = make_pair(alias, nullptr); ; 
        Relations.insert(make_pair(relName, newRel));   
        myStats->CopyRel(currRel->tableName, currRel->aliasAs);
        currRel = currRel->next; 
    }
    struct AndList* currAndList = myQueryParams->boolean; 
    while(currAndList != NULL){
        vector<string> relsInvolved; 
        relsInvolved = processOrlist(currAndList->left); 
        if(relsInvolved.size()==1){
            struct AndList* myNewAnd = new AndList; 
            myNewAnd->left = currAndList->left; 
            myNewAnd->rightAnd = NULL; 
            if(Relations[relsInvolved[0]].second != nullptr){
                Relations[relsInvolved[0]].second->rightAnd = myNewAnd; 
            }else{
                Relations[relsInvolved[0]].second = myNewAnd; 
            }
        }
        currAndList = currAndList->rightAnd; 
    } 
}


// This function gets all the required information regarding the joins. 
// Initializes the Joins map with the corresponding join attributes for each pair of relations. 
// If a pair of relations has no common attributes, it initializes with nullptr. 
void QueryOptimizer::GetJoinsInfo(){
    if(Relations.size()<=1){
        return; 
    }
    for(auto it1 = Relations.begin(); it1 != Relations.end(); it1++){ 
        for(auto it2 = it1; it2 != Relations.end(); it2++ ){ 
            if(it1->first.compare(it2->first)==0){
                continue; 
            }
            vector <string> joinPair; 
            joinPair.push_back(it1->second.first); 
            joinPair.push_back(it2->second.first);  
            vector <string> tableNames; 
            tableNames.push_back(it1->first);
            tableNames.push_back(it2->first);
            Joins.insert(make_pair(joinPair, make_pair(tableNames, nullptr))); 
            tableNames.clear(); 
            joinPair.clear(); 
        }
    }
    
    struct AndList* currAndList = myQueryParams->boolean; 
    while(currAndList != NULL){
        vector<string> relsInvolved; 
        vector<string> aliases; 
        relsInvolved = processOrlist(currAndList->left); 
        for(auto it=relsInvolved.begin(); it!=relsInvolved.end(); it++){
            aliases.push_back(Relations[*it].first); 
        }
        sort(aliases.begin(),aliases.end()); 
        if(aliases.size()==2){
            struct AndList* myNewAnd = new AndList; 
            myNewAnd->left = currAndList->left; 
            myNewAnd->rightAnd = NULL; 
            if(Joins[aliases].second != nullptr){
                Joins[aliases].second->rightAnd = myNewAnd; 
            }else{
                Joins[aliases].second = myNewAnd; 
            }
        }
        currAndList = currAndList->rightAnd; 
    } 
}



vector <string> QueryOptimizer::processOrlist(struct OrList* processMe){
    vector <string> relsInvolved; 
    OrList *currOrList = processMe; 
    ComparisonOp *currOperation;  
    while (currOrList != NULL){  
        currOperation = currOrList->left; 
        if(currOperation->left->code == NAME){
            string res = whichRelation(currOperation->left->value); 
            if(find(relsInvolved.begin(), relsInvolved.end(), res) == relsInvolved.end()){
                relsInvolved.push_back(res); 
            }
        }
        if(currOperation->right->code == NAME){
            string res = whichRelation(currOperation->right->value); 
            if(find(relsInvolved.begin(), relsInvolved.end(), res) == relsInvolved.end()){
                relsInvolved.push_back(res); 
            }
        }
        currOrList =  currOrList->rightOr; 
    }
    sort(relsInvolved.begin(), relsInvolved.end());  
    return relsInvolved;  
}


string QueryOptimizer::whichRelation(char* attribute){
    char relations[10][10] = {"supplier", "partsupp","part","nation","customer","orders","region","lineitem"}; 
    //To delete the alias part.. To search in the schema..
    // May be possible to search using the statisticsTable directly using the actual attribute name..
    int i = strlen(attribute)-1; 
    while(i>=0 && attribute[i]!='.'){
        i-=1; 
    }
    attribute = attribute+i+1; 
    string newString;   
    for(int i=0; i<8; i++){
        Schema newSchema("catalog",relations[i]); 
        if(newSchema.Find(attribute) != -1){
            newString.append(string(relations[i]));
            return newString;
        }
    }

    return newString;
}



void QueryOptimizer::PrintMaps(){ 
    cout << "\nPrinting the maps of the Query Optimizer:\n"; 
    cout << "Relations: \n";
    for(auto it = Relations.begin(); it!=Relations.end(); it++){
        cout << "Relation " << it->first << "\n"; 
        cout << "Alias: " << it->second.first << " AndList address: "<< it->second.second << "\n";  
    }
    cout << "\nJoins: \n"; 
    for (auto it = Joins.begin(); it != Joins.end(); it++)
    {
        for(int i = 0; i < it->first.size(); i++){
            cout << it->first[i] << " "; 
        }
        cout << ":\n";
        cout << "aliases:\n";
        for(int i=0; i< it->second.first.size(); i++){
            cout << it->second.first[i] << " "; 
        }
        cout << "\n";
        cout << "And list address: " <<  it->second.second << "\n"; 
    }
}

/* Method to optimize the given query and generate the best possible query plan.
*/
void QueryOptimizer::optimizeQuery(){
    GetAndProcessRelationInfo(); 
    GetJoinsInfo();

    printf("Calling construct query\n"); // << "Calling Construct Query Plan\n"; 
    //Applying the selection predicate on the relations
    // myStats->PrintStatistics();
    for(auto it=Relations.begin();it!=Relations.end();it++){
        if(it->second.second!=NULL){
            vector<char*> rels;
            if(it->second.first.size()!=0){
                rels.push_back((char*)it->second.first.c_str());
            }else{
                rels.push_back((char*)it->first.c_str());
            }
            char **relNames=rels.data();
            myStats->Apply(it->second.second,relNames,1);
            rels.clear();
        }
    }
    myStats->Write("Statistics.txt");
    //Select query
    if(Relations.size()==1){
        vector<string> table;
        vector<char*> tmpTable;
        dpEntry *entry=new dpEntry();
        for(auto it=Relations.begin();it!=Relations.end();it++){
            if(it->second.first.size()!=0){
                table.push_back(it->second.first);
                tmpTable.push_back((char*)it->second.first.c_str());
            }else{
                table.push_back(it->first);
                tmpTable.push_back((char*)it->first.c_str());
            }
            char **relnames=tmpTable.data();
            entry->cost=0;
            if(it->second.second!=NULL){
                entry->size=myStats->Estimate(it->second.second,relnames,1);
            }else{
                //Need to revisit
            }
            entry->expression=string("(").append(tmpTable[0]).append(")");
            entry->andList=it->second.second;
            entry->tableList.insert(make_pair(it->second.first,it->first));
        }
        currDPList.insert(make_pair(table,entry));
        // constructQueryPlanTree();
        printDPList(currDPList);
    }else{
    // Join query 
     for(auto it1=Joins.begin();it1!=Joins.end();it1++){
         map <string,string> tableList;
         vector<char*> tables;
         vector<string> dtables;
         for(int i=0;i<it1->first.size();i++){
            tables.push_back((char*)it1->first[i].c_str());
            dtables.push_back(it1->first[i]);
            tableList.insert(make_pair(it1->first[i],it1->second.first[i]));
         }
         ll tsize1=myStats->getSizeofRelation(vector<string>{it1->first[0]});
         ll tsize2=myStats->getSizeofRelation(vector<string>{it1->first[1]});
         dpEntry *tmpdpentry=new dpEntry();
         tmpdpentry->cost=0;
         if(it1->second.second==NULL){
             tmpdpentry->size=max<ll>(tsize1*tsize2,0);
         }else{
             tmpdpentry->size=myStats->Estimate(it1->second.second,tables.data(),tables.size());
         }
         string expr("(");
         if(tsize1!=-1 && tsize1>tsize2){
             expr.append(it1->first[1]).append(",").append(it1->first[0]).append(")");
         }else if(tsize2!=-1){
             expr.append(it1->first[0]).append(",").append(it1->first[1]).append(")");
         }else{
             cerr << "Obtained size for one of the relation in the query is -1\n";
             exit(1);
         }
         tmpdpentry->expression=expr;
         tmpdpentry->andList=it1->second.second;
         prevDPList.insert(make_pair(dtables,tmpdpentry));
         tables.clear();
         dtables.clear();
         tableList.clear();
     }
     vector<string> allRelations;
     for(auto it=Relations.begin();it!=Relations.end();it++){
         allRelations.push_back(it->second.first);
     }
     sort(allRelations.begin(),allRelations.end());
    while(prevDPList.begin()->first.size()!=Relations.size()){
        for(auto it1=prevDPList.begin();it1!=prevDPList.end();it1++){
            vector<string> setdiff;
            setdiff=performSetDifference(allRelations,it1->first);
            for(int i=0;i<setdiff.size();i++){
                struct AndList* activeList = NULL;
                activeList =  appendAndList(it1->second->andList,activeList); 
                vector<string> trels;
                vector<char*> relNames;
                for(int j=0;j<it1->first.size();j++){
                    trels.push_back(it1->first[j]);
                    trels.push_back(setdiff[i]);
                    sort(trels.begin(),trels.end());
                    if(Joins.find(trels)!=Joins.end() && Joins[trels].second!=NULL){
                        activeList = appendAndList(Joins[trels].second,activeList);

                    }
                    trels.clear();
                }
                vector<string> newKey=it1->first;
                for(int l=0;l<it1->first.size();l++){
                    relNames.push_back((char*)it1->first[l].c_str());
                }
                relNames.push_back((char*)setdiff[i].c_str());
                double estimatedres=myStats->Estimate(activeList,relNames.data(),relNames.size());
                newKey.push_back(setdiff[i]);
                sort(newKey.begin(),newKey.end());
                dpEntry *tmp1=new dpEntry;
                tmp1->size=estimatedres;
                tmp1->cost=it1->second->size+it1->second->cost;
                tmp1->andList=activeList;
                tmp1->expression=string("(").append(it1->second->expression).append(",").append(setdiff[i]).append(")");
                tmp1->tableList=it1->second->tableList;
                tmp1->tableList.insert(make_pair(setdiff[i],string("x")));
                if(currDPList.find(newKey)==currDPList.end()){
                    currDPList.insert(make_pair(newKey,tmp1));
                }else if(currDPList[newKey]->cost>it1->second->cost + it1->second->size){
                    currDPList[newKey]=tmp1;
                }
            }
           
        }
        prevDPList=currDPList;
        currDPList.clear();
    }
    printDPList(prevDPList);
    currDPList = prevDPList; 
    }
    cout << "Calling construct query plan\n"; 
    constructQueryPlanTree(); 
    // myStats->PrintStatistics();
}

vector<string> QueryOptimizer::performSetDifference(vector<string> setFrom,vector<string> setFrom2){
    vector<string> result;
    for(int i=0;i<setFrom.size();i++){
        if(find(setFrom2.begin(),setFrom2.end(),setFrom[i])==setFrom2.end()){
            result.push_back(setFrom[i]);
        }
    }
    return result;
}

void QueryOptimizer::printDPList(map< vector <string> , struct dpEntry* > toPrint){
    cout << "Printing the DP List" << "\n";
    for(auto it=toPrint.begin();it!=toPrint.end();it++){
        cout << "--------------------\n";
        cout << "tables: ";
        for(int i=0;i<it->first.size();i++){
            cout << it->first[i] << " ";
        }
        cout << "\n";
        cout << "cost: " << it->second->cost << " Size: " << it->second->size << " AndList address: "\
        << it->second->andList << " Expression: " << it->second->expression<<"\n";
        printAndList1(it->second->andList);
        cout << "--------------------\n";
    }
}

struct AndList* QueryOptimizer::appendAndList(struct AndList* from,struct AndList* to){
    if(from==NULL){
        return to;
    }
    struct AndList* cur;
    if(to==NULL){
        to=new AndList;
        to->rightAnd = NULL; 
        cur=to;
    }else{
        cur=to;
        while(cur->rightAnd!=NULL){
            cur=cur->rightAnd;
        }
        cur->rightAnd=new AndList;
        cur=cur->rightAnd;
        cur->rightAnd=NULL;
    }
    struct AndList* cur1=from;
    struct AndList* tmp2;
    struct AndList* prev;
    while(cur1 !=NULL){
        prev = cur; 
        cur->left=cur1->left;
        tmp2 =new AndList;
        tmp2->rightAnd= NULL; 
        cur->rightAnd=tmp2;
        cur=cur->rightAnd;
        cur1=cur1->rightAnd;
    }
    prev->rightAnd=NULL;
    return to; 
}

string QueryOptimizer::getRelationNameFromAlias(string alias){
    string name="";
    if(alias.size()==0){
        return name;
    }
    for(auto it=Relations.begin();it!=Relations.end();it++){
        if(it->second.first.compare(alias)==0){
            name=it->first;
            break;
        }
    }
    return name;
}

TreeNode::TreeNode(){
    literal=nullptr;
    selOp=nullptr;
    keepMe=nullptr;
    nodeSchema=nullptr;
    LeftinSchema= nullptr; 
    RightinSchema= nullptr; 
    outSchema = nullptr; 
    nodeFunc=nullptr;
    groupAtts=nullptr;
    left=nullptr;
    right=nullptr;
    parent=nullptr;
    leftRel = "";
    rightRel="";
    treeAndList=NULL; 
}

void TreeNode::printNode(){
   
    cout << "\n------------------------------------------------------------------------------------------------\n";
    cout << "Left Rel: "<< leftRel <<"; Right Rel: "<<rightRel << "; Operarion type=" << operation << "\n";  
    cout << "Left Pipe: "<<pipeLeft << "; Right Pipe: "<< pipeRight <<"; outpipe: "<<pipeOut << "\n"; 

    if(selOp!=nullptr || selOp!=NULL){
        cout << "Printing CNF\n"; 
        selOp->Print();     
    }
    if(outSchema!=nullptr){
        cout << "OutSchema:\n"; 
        outSchema->Print(); 
    } 
    if(LeftinSchema!=nullptr){
        cout << "LeftinSchema:\n"; 
        LeftinSchema->Print(); 
    } 
    if(RightinSchema!=nullptr){
        cout << "LeftinSchema:\n"; 
        RightinSchema->Print(); 
    } 
    if(groupAtts!= nullptr){
        cout << "Grouping Attributes\n"; 
        groupAtts->Print(); 
    }
    
    if(treeAndList!=NULL){
        cout << "Printing AndList\n"; 
        printAndList1(treeAndList); 
    }

    if(keepMe != nullptr){
        cout << "Number of input attributes = "<<numAttsInput <<"; Number of output attributes = "<<numAttsOutput <<"\n"; 
        cout << "Atts To keep: \n"; 
        for(int i=0; i<numAttsOutput; i++){
            cout << keepMe[i] << " "; 
        }
        cout << "\n"; 
    }
     cout << "------------------------------------------------------------------------------------------------\n";
}

void QueryOptimizer::InitTreeNode(TreeNode *treeNode){
    opType nodeOperation = treeNode->operation;
    
    if(nodeOperation==SelectFile){
        //Select File
        if(treeNode->leftRel.size()==0){
            cerr << "Invalid file for select !!\n"; 
            exit(1); 
        }
        if(Relations.find(treeNode->leftRel)==Relations.end()){
            cerr << "Relation not found in the map!!\n"; 
            exit(1); 
        }
        treeNode->outSchema = new Schema("catalog",(char*)treeNode->leftRel.c_str()); 
        if(strcmp(treeNode->leftRel.c_str(),Relations[treeNode->leftRel].first.c_str())!=0){
            treeNode->outSchema->RenameAliasAttrbts((char*)Relations[treeNode->leftRel].first.c_str()); 
        }
        Attribute *mySchemaAtts = treeNode->outSchema->GetAtts();
        Operand leftOperand; 
        Operand rightOperand; 
        leftOperand.code=3; 
        leftOperand.value= mySchemaAtts[0].name;  
        rightOperand.code=3; 
        rightOperand.value= mySchemaAtts[0].name; 
        ComparisonOp tempOp; 
        tempOp.code= EQUALS; 
        tempOp.left = &leftOperand; 
        tempOp.right = &rightOperand; 
        OrList tempOrlist; 
        tempOrlist.left = &tempOp; 
        tempOrlist.rightOr = NULL; 
        AndList tempAndlist; 
        tempAndlist.left = &tempOrlist; 
        tempAndlist.rightAnd = NULL; 
        treeNode->selOp = new CNF; 
        treeNode->literal=new Record();
        treeNode->selOp->GrowFromParseTree(&tempAndlist, treeNode->outSchema, *treeNode->literal); 
    }else if(nodeOperation==SelectPipe){
        //Select Pipe
        if(treeNode->leftRel.size()==0){
            cerr << "Invalid Relation name given for select !!\n"; 
            exit(1); 
        }
        // if(Relations.find(treeNode->leftRel)==Relations.end()){
        //     cerr << "Relation not found in the map!!\n"; 
        //     exit(1); 
        // }
        if(treeNode->LeftinSchema==nullptr){
            cerr << "Mandatory input schema is missing!!\n";
            exit(1);
        }
        treeNode->outSchema=treeNode->LeftinSchema;
        treeNode->literal=new Record();
        treeNode->selOp=new CNF();
        string tableName=getRelationNameFromAlias(treeNode->leftRel);
        if(tableName.size()!=0){
            treeNode->selOp->GrowFromParseTree(Relations[tableName].second,treeNode->outSchema,*treeNode->literal);
        }
        
    }else if(nodeOperation==DuplicateRemoval){
        //Duplicate Removal
        if(treeNode->LeftinSchema==nullptr){
            cerr << "Mandatory input schema is missing!!\n";
            exit(1);
        }
        treeNode->outSchema=treeNode->LeftinSchema;
    }else if(nodeOperation==Join){
        //Join operation
        if(treeNode->leftRel.size()==0 || treeNode->rightRel.size()==0){
            cerr << "One of the two input relation names are missing!!\n";
            exit(1);
        }
        if(treeNode->LeftinSchema==nullptr || treeNode->RightinSchema==nullptr){
            cerr << "One of the input schemas is missing!!\n";
            exit(1);
        }
        Attribute *leftRelAtts= treeNode->LeftinSchema->GetAtts();
        Attribute *rightRelAtts=treeNode->RightinSchema->GetAtts();
        int leftNumAtts=treeNode->LeftinSchema->GetNumAtts();
        int rightNumAtts=treeNode->RightinSchema->GetNumAtts();
        Attribute *outSchemaAtts=new Attribute[leftNumAtts+rightNumAtts];
        int newidx=0;
        for(int i=0;i<leftNumAtts;i++){
            outSchemaAtts[newidx].myType=leftRelAtts[i].myType;
            cout << "leftRelAtt: " << leftRelAtts[i].name << "\n"; 
            outSchemaAtts[newidx].name = new char[30]; 
            strcpy(outSchemaAtts[newidx].name,leftRelAtts[i].name);
            newidx++;
        }
        for(int j=0;j<rightNumAtts;j++){
            outSchemaAtts[newidx].myType=rightRelAtts[j].myType;
            outSchemaAtts[newidx].name = new char[30];  
            strcpy(outSchemaAtts[newidx].name,rightRelAtts[j].name);
            newidx++;
        }
        treeNode->outSchema=new Schema("Join_schema",leftNumAtts+rightNumAtts,outSchemaAtts);
        //Parsing leftRel - a,b,c..
        char *currLeftRel=new char[30];
        strcpy(currLeftRel,treeNode->leftRel.c_str());
        char* currRel;
        currRel=strtok(currLeftRel,",");
        while(currRel!=NULL){
            vector<string> relNames;
            relNames.push_back(string(currRel));
            relNames.push_back(treeNode->rightRel);
            treeNode->treeAndList=appendAndList(Joins[relNames].second,treeNode->treeAndList);
            relNames.clear();
            currRel=strtok(NULL,",");
        }
        treeNode->literal=new Record();
        treeNode->selOp=new CNF();
        treeNode->selOp->GrowFromParseTree(treeNode->treeAndList,treeNode->LeftinSchema,treeNode->RightinSchema,*treeNode->literal);
    }

}

void QueryOptimizer::constructQueryPlanTree(){
    cout << "Current DPlist size: " << currDPList.size() <<  "\n";
    if(currDPList.size()!=0){
        vector<string> finalInvolvedRels = currDPList.begin()->first; 
        string expression = currDPList.begin()->second->expression;
        int PipeNumber=0; 
        // cout << "Size of finalInvolvedRels: " << finalInvolvedRels.size() <<"\n"; 
        if(finalInvolvedRels.size()==1){
            //SelectOperation, No joins involved
            cout << "Relation" << finalInvolvedRels[0] << "\n"; 
            string tableName = currDPList[finalInvolvedRels]->tableList[finalInvolvedRels[0]]; 
            TreeNode *rootNode = selectFileNode(tableName, PipeNumber);  
        }else{
            //Join Operation is involved 
            TreeNode* rootJoin;  
            rootJoin = getJoinNodes(expression, PipeNumber); 
            rootNode = rootJoin; 
        }
    }


}   

TreeNode* QueryOptimizer::getJoinNodes(string expression, int &PipeNumber){ 
    cout << "Expression : " << expression << "\n"; 
    if(expression[0]!= '('){
        size_t index = expression.find_first_of(",");
        string leftRel = expression.substr(0, index); 
        string rightRel = expression.substr(index+1, expression.size()- (index+1)); 
        string leftRelName = getRelationNameFromAlias(leftRel); 
        string rightRelName = getRelationNameFromAlias(rightRel);
        TreeNode *leftRoot = selectFileNode(leftRelName, PipeNumber); 
        TreeNode *rightRoot = selectFileNode(rightRelName, PipeNumber); 
        TreeNode *JoinNode = new TreeNode; 
        JoinNode->operation = Join; 
        JoinNode->left = leftRoot; 
        JoinNode->right = rightRoot; 
        JoinNode->LeftinSchema = leftRoot->outSchema; 
        JoinNode->RightinSchema = rightRoot->outSchema; 
        JoinNode->pipeLeft = leftRoot->pipeOut; 
        JoinNode->pipeRight = rightRoot->pipeOut; 
        JoinNode->pipeOut = PipeNumber++; 
        JoinNode->leftRel = leftRel; 
        JoinNode->rightRel = rightRel;   
        InitTreeNode(JoinNode); 
        JoinNode->printNode(); 
        return JoinNode; 
    }else{
        string innerExpression = expression.substr(1,expression.size()-2); 
        cout << "Inner expression: " << innerExpression << "\n"; 
        if(innerExpression.find_last_of(')')== -1){
            cout << "Inside first if"; 
            return getJoinNodes(innerExpression, PipeNumber); 
        }else{
            size_t lastindex = innerExpression.find_last_of(')'); 
            string childJoinExpression = innerExpression.substr(1,innerExpression.size()-1 + lastindex); 
            string rightRel = innerExpression.substr(lastindex+1, innerExpression.size()-(lastindex+1)); 
            string rightRelName = getRelationNameFromAlias(rightRel); 
            TreeNode *JoinNode = new TreeNode; 
            TreeNode *leftRoot = getJoinNodes(childJoinExpression, PipeNumber); 
            TreeNode *rightRoot = selectFileNode(rightRelName, PipeNumber); 
            string myleftRel = leftRoot->leftRel; 
            myleftRel.append(","); 
            myleftRel.append(leftRoot->rightRel); 
            JoinNode->operation = Join; 
            JoinNode->left = leftRoot; 
            JoinNode->right = rightRoot; 
            JoinNode->LeftinSchema = leftRoot->outSchema; 
            JoinNode->RightinSchema = rightRoot->outSchema; 
            JoinNode->pipeLeft = leftRoot->pipeOut; 
            JoinNode->pipeRight = rightRoot->pipeOut; 
            JoinNode->pipeOut = PipeNumber++; 
            JoinNode->leftRel = myleftRel; 
            JoinNode->rightRel = rightRel;   
            InitTreeNode(JoinNode); 
            JoinNode->printNode(); 
            return JoinNode; 
        }
    }

}

TreeNode* QueryOptimizer::selectFileNode(string tableName, int &PipeNumber){
    TreeNode *tempNode = new TreeNode; 
    TreeNode *rootForSelect; 
    tempNode->operation = SelectFile; 
    tempNode->leftRel = tableName;  
    tempNode->pipeOut = PipeNumber++; 
    tempNode->left = nullptr; 
    tempNode->right = nullptr;  
    cout << "***************tableName: "<< tableName << "\n"; 
    InitTreeNode(tempNode); 
    rootForSelect = tempNode; 
    tempNode->printNode(); 
    if(Relations[tableName].second!=NULL){
        TreeNode *pipeNode = new TreeNode; 
        pipeNode->operation = SelectPipe; 
        pipeNode->leftRel = Relations[tableName].first; 
        pipeNode->LeftinSchema = tempNode->outSchema; 
        pipeNode->left = tempNode; 
        pipeNode->right = nullptr; 
        pipeNode->pipeOut = PipeNumber++; 
        pipeNode->pipeLeft = tempNode->pipeOut; 
        InitTreeNode(pipeNode); 
        pipeNode->printNode();
        rootForSelect = pipeNode; 
    }
    return rootForSelect; 
}
