#include "gtest/gtest.h"
#include <iostream>
#include "QueryOptimizer.h"
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


TEST(QueryOptimizerTest,GetJoinsFunction){
   char* query =  "SELECT n.n_name FROM nation AS n, region AS r WHERE (n.n_regionkey = r.r_regionkey) AND (n.n_nationkey > 5)"; 
    yy_scan_string(query); 
    yyparse();
    string expression = "(r,n)"; 
    QueryOptimizer myQO; 
    int PipeNumber; 
    myQO.optimizeQuery();
    TreeNode *rootJoin = myQO.getJoinNodes(expression, PipeNumber); 
    ASSERT_EQ(rootJoin->leftRel.compare(string("r")),0); 
}


TEST(QueryOptimizerTest,GetSelectFunctionWithCNF){
   char* query =  "SELECT n.n_name FROM nation AS n, region AS r WHERE (n.n_regionkey = r.r_regionkey) AND (n.n_nationkey > 5)"; 
    yy_scan_string(query); 
    yyparse();
    string expression = "(r,n)"; 
    QueryOptimizer myQO; 
    int PipeNumber; 
    myQO.optimizeQuery();
    TreeNode *rootNode = myQO.selectFileNode("nation", PipeNumber); 
    ASSERT_EQ(rootNode->operation,SelectPipe);  
}

TEST(QueryOptimizerTest,GetSelectFunctionWithoutCNF){
    char* query =  "SELECT n.n_name FROM nation AS n, region AS r WHERE (n.n_regionkey = r.r_regionkey) AND (n.n_nationkey > 5)"; 
    yy_scan_string(query); 
    yyparse();
    string expression = "(r,n)"; 
    QueryOptimizer myQO; 
    int PipeNumber; 
    myQO.optimizeQuery();
    TreeNode *rootNode = myQO.selectFileNode("region", PipeNumber); 
    ASSERT_EQ(rootNode->operation,SelectFile);   
}

TEST(QueryOptimizerTest, ProjectSelectQuery){
    char* query = "SELECT n.n_nationkey FROM nation AS n WHERE (n.n_name = 'UNITED STATES')"; 
    yy_scan_string(query); 
    yyparse(); 
    QueryOptimizer myQO; 
    myQO.optimizeQuery(); 
    opType operation = myQO.rootNode->operation; 
    ASSERT_EQ(operation, Project);  
}

TEST(QueryOptimizerTest,processOrListFunction){
    char* query = "SELECT n.n_nationkey FROM nation AS n WHERE (n.n_name = 'UNITED STATES')"; 
    yy_scan_string(query); 
    yyparse(); 
    QueryOptimizer myQO; 
    myQO.optimizeQuery(); 
    vector <string> relsInvolved = myQO.processOrlist(boolean->left); 
    ASSERT_EQ(relsInvolved[0].compare("nation"),0);   
}


TEST(QueryOptimizerTest,InitTreeNodeJoinTest){
    char* query =  "SELECT n.n_name FROM nation AS n, region AS r WHERE (n.n_regionkey = r.r_regionkey) AND (n.n_nationkey > 5)"; 
    yy_scan_string(query); 
    yyparse();
    string expression = "(r,n)"; 
    QueryOptimizer myQO; 
    int PipeNumber; 
    myQO.optimizeQuery();
    TreeNode JoinNode; 
    Schema nationSchema("catalog", "n.nation"); 
    Schema regionSchema("catalog", "r.region"); 
    JoinNode.operation = Join; 
    JoinNode.LeftinSchema = &nationSchema; 
    JoinNode.RightinSchema = &regionSchema; 
    JoinNode.leftRel = "r"; 
    JoinNode.rightRel = "l"; 
    myQO.InitTreeNode(&JoinNode); 
    ASSERT_EQ(JoinNode.outSchema->GetNumAtts(), 7);

}

TEST(QueryOptimizerTest,InitTreeNodeSelectPipeTest){
    char* query =  "SELECT n.n_name FROM nation AS n, region AS r WHERE (n.n_regionkey = r.r_regionkey) AND (n.n_nationkey > 5)"; 
    yy_scan_string(query); 
    yyparse();
    string expression = "(r,n)"; 
    QueryOptimizer myQO; 
    int PipeNumber; 
    myQO.optimizeQuery();
    Schema nationSchema("catalog", "n.nation"); 
    TreeNode selectPipe; 
    selectPipe.operation = SelectPipe; 
    selectPipe.LeftinSchema = &nationSchema; 
    selectPipe.leftRel = "n"; 
    myQO.InitTreeNode(&selectPipe); 
    ASSERT_NE(selectPipe.selOp,nullptr); 
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}