#include "QueryOptimizer.h"
#include <iostream>
#include <algorithm>
using namespace std; 

void printAndList1(struct AndList *currAndlist){
    // cout << "currAndList address in print: "<< currAndlist << "\n";
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
    GetAndProcessRelationInfo(); 
    GetJoinsInfo(); 
}

QueryOptimizer::QueryOptimizer(void *args){
    myQueryParams = (queryParsedInfo*)args; 
    myStats = new Statistics; 
    myStats->Read("InitalStatitics.txt"); 
    GetAndProcessRelationInfo(); 
    GetJoinsInfo(); 
}


//This function gets all information regarding all the relations involved in the query. 
//Initializes the Relations map with the corresponding "Select CNF". 
//Puts nullptr if there is no CNF correspondng to a relation. It also creates the corresponding aliases in Statistics.
void QueryOptimizer::GetAndProcessRelationInfo(){
    struct  TableList* currRel = myQueryParams->tables; 
    while(currRel != NULL){ 
        string relName(currRel->tableName); 
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
            joinPair.push_back(it1->first); 
            joinPair.push_back(it2->first);  
            vector <string> aliases; 
            aliases.push_back(it1->second.first);
            aliases.push_back(it2->second.first);
            Joins.insert(make_pair(joinPair, make_pair(aliases, nullptr))); 
            aliases.clear(); 
            joinPair.clear(); 
        }
    }
    
    struct AndList* currAndList = myQueryParams->boolean; 
    while(currAndList != NULL){
        vector<string> relsInvolved; 
        relsInvolved = processOrlist(currAndList->left); 
        if(relsInvolved.size()==2){
            struct AndList* myNewAnd = new AndList; 
            myNewAnd->left = currAndList->left; 
            myNewAnd->rightAnd = NULL; 
            if(Joins[relsInvolved].second != nullptr){
                Joins[relsInvolved].second->rightAnd = myNewAnd; 
            }else{
                Joins[relsInvolved].second = myNewAnd; 
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
    // cout << "Processed OrList, resulted in the below relations:\n"; 
    // for(int i=0; i<relsInvolved.size();i++){
    //     cout << relsInvolved[i] << " "; 
    // }
    // cout << "\n"; 
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
    // cout << "Searching Attribute " << attribute << "\n";  
    for(int i=0; i<8; i++){
        Schema newSchema("catalog",relations[i]); 
        string newString(relations[i]); 
        if(newSchema.Find(attribute) != -1){
            // cout << "Returning " << newString << "  " << relations[i] << " Matched\n"; 
            return newString;
        }
    }
    return nullptr;
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
}

void TreeNode::printNode(){

}

void QueryOptimizer::InitTreeNode(TreeNode *treeNode){
    opType nodeOperation = treeNode->operation;
    
    if(nodeOperation==SelectFile){
        if(treeNode->leftRel.size()==0){
            cerr << "Invalid file for select !!\n"; 
            exit(1); 
        }
        if(Relations.find(treeNode->leftRel)==Relations.end()){
            cerr << "Relation not found in the map!!\n"; 
            exit(1); 
        }
        treeNode->outSchema = new Schema("catalog",(char*)treeNode->leftRel.c_str()); 
        treeNode->outSchema->RenameAliasAttrbts((char*)Relations[treeNode->leftRel].first.c_str()); 
        Attribute *mySchemaAtts = treeNode->outSchema->GetAtts();
        Operand leftOperand; 
        Operand rightOperand; 
        leftOperand.code=3; 
        leftOperand.value= mySchemaAtts[0].name;  
        rightOperand.code=3; 
        rightOperand.value= mySchemaAtts[0].name; 
        ComparisonOp tempOp; 
        tempOp.code= Equals; 
        tempOp.left = &leftOperand; 
        tempOp.right = &rightOperand; 
        OrList tempOrlist; 
        tempOrlist.left = &tempOp; 
        tempOrlist.rightOr = NULL; 
        AndList tempAndlist; 
        tempAndlist.left = &tempOrlist; 
        tempAndlist.rightAnd = NULL; 
        treeNode->selOp = new CNF; 
        treeNode->selOp->GrowFromParseTree(&tempAndlist, treeNode->outSchema, *treeNode->literal); 
    }

}

