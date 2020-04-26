#include <stdio.h>
#include "DBFile.h"
#include "File.h"
#include "test.h" 
#include "gtest/gtest.h"
#include "SortedFile.h"
#include "RelOp.h"

TEST(RelopTest,SelectPipeTest){
  Pipe in(2);
  Pipe out(2);
  Record temp;
  Record temp1;
  Schema schema("catalog","nation");
  char *pred_ps="(n_nationkey < 1 )";
  CNF cnf;
  Record literal;
  get_cnf(pred_ps,&schema,cnf,literal);
  SelectPipe sp; 
  char fname[20];
  sprintf(fname,"%s.bin","nation");
  DBFile testFile;
  testFile.Open(fname);
  testFile.MoveFirst();
  testFile.GetNext(temp1); 
  in.Insert(&temp1); 
  in.ShutDown();
  sp.Run(in, out, cnf, literal); 
  out.Remove(&temp);
  testing::internal::CaptureStdout();
  temp.Print(&schema);
  ASSERT_EQ("n_nationkey: [0], n_name: [ALGERIA], n_regionkey: [0], n_comment: [ haggle. carefully final deposits detect slyly agai]\n",testing::internal::GetCapturedStdout());
  testFile.Close();
  
}


TEST(RelopTest,SelectFileTest){
  DBFile testFile;
  Pipe out(100); 
  Record *temp = new Record;
  Schema mySchema ("catalog", "nation");
	char *pred_ps = "(n_nationkey < 1)"; 
  CNF cnf1; 
  Record literal; 
  get_cnf(pred_ps, &mySchema, cnf1, literal);
  testFile.Open("nation.bin");
  // testFile.MoveFirst();
  SelectFile sf; 
  sf.Run(testFile, out, cnf1, literal);
  sf.Use_n_Pages(8); 
  out.Remove(temp);
  testing::internal::CaptureStdout();
  temp->Print(&mySchema);
  ASSERT_EQ("n_nationkey: [0], n_name: [ALGERIA], n_regionkey: [0], n_comment: [ haggle. carefully final deposits detect slyly agai]\n",testing::internal::GetCapturedStdout());
  testFile.Close();
  delete temp;
  delete o;
}

TEST(RelopTest,DuplicateRemovalTest){
    DBFile testFile;
    Pipe in(10); 
    Pipe out(10); 
    Record temp; 
    DuplicateRemoval distinct; 
    Schema nation1("catalog", "nation"); 
    testFile.Open("nation.bin"); 
    testFile.MoveFirst();
    testFile.GetNext(temp); 
    in.Insert(&temp); 
    testFile.GetNext(temp); 
    in.Insert(&temp);
    in.ShutDown(); 
    distinct.Run(in, out, nation1); 
    out.Remove(&temp); 
    ASSERT_EQ(0,out.Remove(&temp)); 
    testFile.Close();
}

TEST(RelopTest,SumTest){
    DBFile testFile;
    Pipe in(10); 
    Pipe out(10); 
    Record temp; 
    Sum getSum; 
    Function func;
    Schema nation1("catalog", "nation"); 
    char *str_sum = "(n_nationkey + 1.05)";
    get_cnf (str_sum, &nation1, func);
    testFile.Open("nation.bin");
    testFile.MoveFirst(); 
    testFile.GetNext(temp); 
    in.Insert(&temp); 
    testFile.GetNext(temp); 
    in.Insert(&temp); 
    in.ShutDown();
    getSum.Run(in, out, func); 
    out.Remove(&temp); 
    testing::internal::CaptureStdout();
    Attribute DA1 = {"double", Double};
    Schema sch("sum_sch",1,&DA1);
    temp.Print(&sch);
    ASSERT_EQ("double: [2.1]\n",testing::internal::GetCapturedStdout());
    testFile.Close();
}


TEST(RelopTest,ProjectTest){
    DBFile testFile;
    Pipe in(10); 
    Pipe out(10); 
    Record temp; 
    Project projectthis; 
    Schema nation1("catalog", "nation"); 
    testFile.Open("nation.bin");
    testFile.MoveFirst(); 
    testFile.GetNext(temp); 
    in.Insert(&temp); 
    testFile.GetNext(temp); 
    in.Insert(&temp); 
    in.ShutDown();
    int attstoKeep[3] = {0,1,2}; 
    Attribute IA = {"n_nationkey", Int}; 
    Attribute SA = {"n_name", String}; 
    Attribute IA2 = {"n_regionkey", Int}; 
    Attribute atts[3] = {IA, SA, IA2}; 
    Schema nation2("nation2", 3, atts);
    projectthis.Run(in, out, attstoKeep, 4, 3); 
    out.Remove(&temp); 
    testing::internal::CaptureStdout();
    temp.Print(&nation2);
    ASSERT_EQ("n_nationkey: [0], n_name: [ALGERIA], n_regionkey: [0]\n",testing::internal::GetCapturedStdout());
    testFile.Close();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}