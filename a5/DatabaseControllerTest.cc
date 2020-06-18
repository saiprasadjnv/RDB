#include "gtest/gtest.h"
#include <iostream>
#include "QueryOptimizer.h"
#include "DatabaseController.h"
#include "ParseTree.h"
#include "vector"
using namespace std;
extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char*);
extern "C" int yyparse(void);

extern struct TableList* tables; 
extern struct FuncOperator *finalFunction; // the aggregate function (NULL if no agg)
extern struct AndList *boolean; // the predicate in the WHERE clause
extern struct NameList *groupingAtts; // grouping atts (NULL if no grouping)
extern struct NameList *attsToSelect; // the set of attributes in the SELECT (NULL if no such atts)
extern int distinctAtts; // 1 if there is a DISTINCT in a non-aggregate query 
extern int distinctFunc; 


TEST(DatabaseControllerTest,processQuery){
    char* query =  "CREATE TABLE nation (n_nationkey INTEGER, n_name STRING, n_regionkey INTEGER, n_comment STRING) AS SORTED ON n_regionkey;"; 
    yy_scan_string(query); 
    yyparse();
    DatabaseController myDC;   
    ASSERT_EQ(1,myDC.processQuery());     
}


TEST(DatabaseControllerTest,Create){
    char* query =  "CREATE TABLE nation (n_nationkey INTEGER, n_name STRING, n_regionkey INTEGER, n_comment STRING) AS SORTED ON n_regionkey;"; 
    yy_scan_string(query); 
    yyparse();
    char* fileName = "nation.bin";  
    DatabaseController myDC;   
    myDC.Create();
    FILE* nationFile = fopen(fileName, "r"); 
    ASSERT_NE(nullptr, nationFile); 
    fclose(nationFile); 
}

TEST(DatabaseControllerTest,Drop){
    char* query =  "CREATE TABLE nation (n_nationkey INTEGER, n_name STRING, n_regionkey INTEGER, n_comment STRING) AS SORTED ON n_regionkey;"; 
    yy_scan_string(query); 
    yyparse();
    char* fileName = "nation.bin";  
    DatabaseController myDC;   
    myDC.Create();
    query =  "DROP TABLE nation;"; 
    yy_scan_string(query); 
    yyparse();
    myDC.Drop(); 
    FILE* nationFile = fopen(fileName, "r"); 
    ASSERT_EQ(nullptr, nationFile); 
    fclose(nationFile); 
}

TEST(DatabaseControllerTest, Insert){
    char* query =  "CREATE TABLE nation (n_nationkey INTEGER, n_name STRING, n_regionkey INTEGER, n_comment STRING) AS SORTED ON n_regionkey;"; 
    yy_scan_string(query); 
    yyparse();
    char* fileName = "nation.bin";  
    DatabaseController myDC;   
    myDC.Create();
    query =  "INSERT 'nation.tbl' INTO nation;"; 
    yy_scan_string(query); 
    yyparse();
    ASSERT_EQ(myDC.Insert(),1); 
    FILE* nationFile = fopen(fileName, "r"); 
    ASSERT_NE(nullptr, nationFile);
    fclose(nationFile); 
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}