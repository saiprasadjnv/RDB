#include <stdio.h>
#include "DBFile.h"
#include "File.h"
#include "gtest/gtest.h"

TEST(DBFileHeapTest,EmptyFileCreationWithHeapFileTypeTest){
    DBFile testFile;
    ASSERT_EQ(1,testFile.Create("testfile.bin",heap,NULL));
    ASSERT_EQ(0,testFile.Close());
}

TEST(DBFileHeapTest,EmptyFileCreationWithInvalidFileTypeSortedTest){
    DBFile testFile;
    ASSERT_EQ(0,testFile.Create("testfile.bin",sorted,NULL));
    testFile.Close();
}

TEST(DBFileHeapTest,FileOpenTest){
  DBFile testFile;
  testFile.Create("testfile.bin",heap,NULL);
  ASSERT_EQ(1,testFile.Open("testfile.bin"));
  testFile.Close();
}

TEST(DBFileHeapTest,InvalidFileOpenTest){
  DBFile testFile;
  ASSERT_DEATH(testFile.Open("testtt.bin"),"BAD!  Open did not work for testtt.bin");
}

TEST(DBFileHeapTest,GetNextAfterHardCloseTest){
DBFile testFile;
Record temp;
Schema mySchema ("catalog", "nation");
testFile.Create("testfile.bin",heap,NULL);
testFile.Load(mySchema,"test.tbl");
testFile.Close();
testFile.Open("testfile.bin");
testFile.GetNext(temp);
testing::internal::CaptureStdout();
temp.Print(&mySchema);
ASSERT_EQ("n_nationkey: [0], n_name: [ALGERIA], n_regionkey: [0], n_comment: [ haggle. carefully final deposits detect slyly agai]\n",testing::internal::GetCapturedStdout());
//1111111111111111111111111111111111111111111111111111111110110000
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}