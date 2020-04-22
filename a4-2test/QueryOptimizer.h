#ifndef QUERY_OPTIMIZER_H
#define QUERY_OPTIMIZER_H


#include <iostream>
#include "ParseTree.h"
#include "Statistics.h"
#include "Schema.h" 
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include "Function.h"
using namespace std;

enum opType {SelectFile,SelectPipe,Project,Join,DuplicateRemoval,Sum,Groupby,WriteOut};

struct queryParsedInfo{
    struct TableList* tables; 
    struct FuncOperator *finalFunction;
    struct AndList *boolean;
    struct NameList *groupingAtts; 
    struct NameList *attsToSelect;
    int distinctAtts; 
    int distinctFunc; 
}; 

struct dpEntry{
    ll size;
    ll cost;
    string expression;
    struct AndList *andList;
    map <string,string> tableList;
};

class TreeNode{
    public:
        Record *literal; // Select Pipe, Select File, Join
        CNF *selOp;  // Select Pipe, Select File, Join
        int *keepMe; // Projection
        int numAttsInput; //Projection
        int numAttsOutput;  //Projection
        Schema *nodeSchema; //Duplicate removal
        Schema* LeftinSchema; // Mandatory for single child nodes as well
        Schema* RightinSchema;
        Schema* outSchema;
        Function *nodeFunc; //Sum, Groupby
        OrderMaker *groupAtts;  //Groupby
        opType operation;
        int pipeLeft,pipeRight,pipeOut;
        TreeNode *left;
        TreeNode *right;
        TreeNode *parent;
        string leftRel; // In select file, fill leftRel with the relation name
        string rightRel;
        struct AndList *treeAndList;
    
    TreeNode();
    void printNode();
};

class QueryOptimizer{
    private: 
        struct queryParsedInfo* myQueryParams; 
        Statistics* myStats; 

        //Key: Vector of alias Names a/c to catalog
        //Value: <pair of string of TableNames, CNF Andlist> 
        map <vector <string> , pair< vector<string>, struct AndList* > > Joins; 

        //Key: Relation Name a/c to catalog
        //Value: <pair of Alias Name, CNF Andlist> 
        map <string, pair <string, struct AndList* > > Relations;

        map< vector <string> , struct dpEntry* > prevDPList; 

        map< vector <string> , struct dpEntry* > currDPList; 
        
        //This function gets all information regarding all the relations involved in the query. 
        //Initializes the Relations map with the corresponding "Select CNF". 
        //Puts nullptr if there is no CNF correspondng to a relation. It also creates the corresponding aliases in Statistics. 
        void GetAndProcessRelationInfo();

        // This function gets all the required information regarding the joins. 
        //Initializes the Joins map with the corresponding join attributes for each pair of relations. 
        //If a pair of relations has no common attributes, it initializes with nullptr. 
        void GetJoinsInfo(); 

        //This function processes the given orList, returns the vector of the involved relations. 
        vector <string> processOrlist(OrList *procesMe); 

        //Utility funtion used to find the relation to which the attribute belongs to. 
        string whichRelation(char* attribute); 

        vector<string> performSetDifference(vector<string> setFrom,vector<string> setFrom2);

        struct AndList* appendAndList(struct AndList* from,struct AndList* to);

        string getRelationNameFromAlias(string alias);


    public: 
        TreeNode *rootNode;
        QueryOptimizer(); 
        QueryOptimizer(void *args); 

        void optimizeQuery();

        void PrintMaps(); 

        void printDPList(map< vector <string> , struct dpEntry* > toPrint);
        
        //Initializes the node with the required information a/c to opType. 
        void InitTreeNode(TreeNode *thisNode); 

        void constructQueryPlanTree();

}; 
#endif 