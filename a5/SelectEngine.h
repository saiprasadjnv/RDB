#ifndef SELECTENGINE_H
#define SELECTENGINE_H 
#include <iostream>
#include <string>
#include <vector> 
#include "QueryOptimizer.h" 
#include "Pipe.h" 
#include "DBFile.h" 
#include "RelOp.h"

#define MY_BIN ".bin"
#define MY_SCHM ".schema" 
using namespace std; 

extern struct TableList* tables; 
extern struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
extern struct AndList *boolean; // the predicate in the WHERE clause
extern struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
extern struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
extern int distinctFunc;  // 1
extern char *fileType; // filetype HEAP or SORTED
extern struct SchemaAttributes *schemaAttributes; // schema attributes for create query
extern struct SortAttributes *sortAttributes; // sort attributes to be used in sortedfile
extern char *fileName; // fileName for query specific if needed
extern int operationType; // operation code select-1, create-2 and so on (defined in parseTree.h)

class SelectEngine{
    friend class DatabaseController; 
    private: 
        vector <Pipe*> allPipes; 
        TreeNode *queryTree; 
        QueryOptimizer *myQueryOptimizer; 
        string redirectOutputTo; 

        void ExecuteQuery(TreeNode *currNode); 
    public:
        SelectEngine();
        ~SelectEngine(); 
        int Execute(); 

}; 

#endif