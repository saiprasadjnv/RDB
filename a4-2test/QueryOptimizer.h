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
using namespace std;

struct queryParsedInfo{
    struct TableList* tables; 
    struct FuncOperator *finalFunction;
    struct AndList *boolean;
    struct NameList *groupingAtts; 
    struct NameList *attsToSelect;
    int distinctAtts; 
    int distinctFunc; 
}; 

class QueryOptimizer{
    private: 
        struct queryParsedInfo* myQueryParams; 
        Statistics* myStats; 

        //Key: Vector of Relation Names a/c to catalog
        //Value: <pair of string of aliases, CNF Andlist> 
        map <vector <string> , pair< vector<string>, struct AndList* > > Joins; 

        //Key: Relation Name a/c to catalog
        //Value: <pair of Alias Name, CNF Andlist> 
        map <string, pair <string, struct AndList* > > Relations; 
        
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

    public: 
        QueryOptimizer(); 
        QueryOptimizer(void *args); 

        void PrintMaps(); 

}; 
#endif 