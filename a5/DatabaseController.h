#ifndef DatabaseController_H
#define DatabaseController_H

#include "DBFile.h"
#include "Comparison.h"
#include "ParseTree.h"
#include "Defs.h"
#include "Schema.h"
#include "vector"

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

class DatabaseController{
    private:
        DBFile *myDBFile;
        void createOrderMaker(OrderMaker *toWhich,SortAttributes *fromWhich,Schema *whichSchema); 
        Schema* createSchema(SchemaAttributes *fromWhich,string schemaFile);       
    public:
        DatabaseController();
        ~DatabaseController();
        int processQuery();
        int Create();
        int Insert();
};
#endif