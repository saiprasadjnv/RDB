#include "gtest/gtest.h"
#include <iostream>
#include "Statistics.h"
#include "ParseTree.h"
#include "vector"
using namespace std;
extern "C" struct YY_BUFFER_STATE *yy_scan_string(const char*);
extern "C" int yyparse(void);
extern struct AndList *final;

TEST(StatisticsTest,EstimateSelectEqualityTest){
    char *relName[] = {"test"};
    Statistics s;
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    char* cnf = "(key=10)";	
	yy_scan_string(cnf);
	yyparse();
    double res=s.Estimate(final,relName,1);
    ASSERT_EQ(100.0,res);
}

TEST(StatisticsTest,EstimateSelectInEqualityTest){
    Statistics s;
    char *relName[] = {"orders","customer","nation"};
	s.AddRel(relName[0],1500000);
	s.AddAtt(relName[0], "o_custkey",150000);

	s.AddRel(relName[1],150000);
	s.AddAtt(relName[1], "c_custkey",150000);
	s.AddAtt(relName[1], "c_nationkey",25);
	
	s.AddRel(relName[2],25);
	s.AddAtt(relName[2], "n_nationkey",25);
    char* cnf = "(o_custkey>100)";	
	yy_scan_string(cnf);
	yyparse();
    char *rels[]={"orders"};
    double res=s.Estimate(final,rels,1);
    ASSERT_EQ((double)(1500000/3),res);
}

TEST(StatisticsTest,EstimateJoinTestWithTwoTables){
    Statistics s;
    char *relName[] = {"orders","customer"};
	s.AddRel(relName[0],1500000);
	s.AddAtt(relName[0], "o_custkey",150000);

	s.AddRel(relName[1],150000);
	s.AddAtt(relName[1], "c_custkey",150000);
	s.AddAtt(relName[1], "c_nationkey",25);
	
	// s.AddRel(relName[2],25);
	// s.AddAtt(relName[2], "n_nationkey",25);
    char* cnf = "(o_custkey=c_custkey)";	
	yy_scan_string(cnf);
	yyparse();
    double res=s.Estimate(final,relName,2);
    ASSERT_EQ((double)(1500000),res);
}

TEST(StatisticsTest,EstimateJoinTestWithThreeTables){
    Statistics s;
    char *relName[] = {"orders","customer","nation"};
	s.AddRel(relName[0],1500000);
	s.AddAtt(relName[0], "o_custkey",150000);

	s.AddRel(relName[1],150000);
	s.AddAtt(relName[1], "c_custkey",150000);
	s.AddAtt(relName[1], "c_nationkey",25);
	
	s.AddRel(relName[2],25);
	s.AddAtt(relName[2], "n_nationkey",25);
    char* cnf = "(o_custkey=c_custkey) AND (n_nationkey>10)";	
	yy_scan_string(cnf);
	yyparse();
    double res=s.Estimate(final,relName,3);
    ASSERT_EQ((double)(12500000),res);
}

TEST(StatisticsTest,ApplySelectTest){
    Statistics s;
    char *relName[] = {"orders","customer","nation"};
	s.AddRel(relName[0],1500000);
	s.AddAtt(relName[0], "o_custkey",150000);

	s.AddRel(relName[1],150000);
	s.AddAtt(relName[1], "c_custkey",150000);
	s.AddAtt(relName[1], "c_nationkey",25);
	
	s.AddRel(relName[2],25);
	s.AddAtt(relName[2], "n_nationkey",25);
    char* cnf = "(o_custkey>100)";	
	yy_scan_string(cnf);
	yyparse();
    char *rels[]={"orders"};
    s.Apply(final,rels,1);
    double res=s.Estimate(final,rels,1);
    ASSERT_EQ((double)(1500000/3),res);
}

TEST(StatisticsTest,CheckIfRelsExists){
    char *relName[] = {"test1"};
    Statistics s;
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    vector<vector<char*>> partition;
    int res=s.CheckifRelsExist(relName,1,partition);
    ASSERT_EQ(-1,res);
}

TEST(StatisticsTest,CheckAndGetAttValSuccessTest){
    Statistics s;
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    vector<vector<char*>> partition;
    vector<char*> subPartition;
    subPartition.push_back("test");
    partition.push_back(subPartition);
    int whichPartition;
    int res=s.checkAndGetAttVal(partition,"key");
    ASSERT_EQ(10,res);
}

TEST(StatisticsTest,CheckAndGetAttValFailureTest){
    Statistics s;
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    vector<vector<char*>> partition;
    vector<char*> subPartition;
    subPartition.push_back("test1");
    partition.push_back(subPartition);
    int whichPartition;
    int res=s.checkAndGetAttVal(partition,"key");
    ASSERT_EQ(-2,res);
}

TEST(StatisticsTest,ReadAndWriteTest){
    Statistics s;
    char *relName[] = {"test"};
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    char *filename="readandwritetest.txt";
    s.Write(filename);
    Statistics s1;
    s1.Read(filename);
    char* cnf = "(key=9)";	
	yy_scan_string(cnf);
	yyparse();
    double res=s1.Estimate(final,relName,1);
    ASSERT_EQ(100,res);
    remove(filename);
}

TEST(StatisticsTest,EstimateWithWrongRelationDeathTest){
    char *relName[] = {"test1"};
    Statistics s;
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    char* cnf = "(key=10)";	
	yy_scan_string(cnf);
	yyparse();
    ASSERT_DEATH(s.Estimate(final,relName,1),"Given relations cannot be used for estimation!!!");
}

TEST(StatisticsTest,ReadWithWrongFileNameDeathTest){
    Statistics s;
    char *relName[] = {"test"};
    s.AddRel("test",1000);
    s.AddAtt("test","key",10);
    char *filename="readandwritetest.txt";
    s.Write(filename);
    char *filename1="readandwritetest1.txt";
    Statistics s1;
    ASSERT_DEATH(s1.Read(filename1),"File readandwritetest1.txt doesn't exist!!");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}