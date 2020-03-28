#include <stdio.h>
#include "DBFile.h"
#include "File.h"
#include "gtest/gtest.h"
#include "SortedFile.h"

TEST(SortedFileTest,EmptyFileCreationWithHeapFileTypeTest){
    DBFile testFile;
    ASSERT_EQ(1,testFile.Create("testfile.bin",heap,NULL));
    ASSERT_EQ(1,testFile.Close());
    remove("testfile.bin");
    remove("testfile.bin.meta");
}

TEST(SortedFileTest,EmptyFileCreationWitheSortedFileTest){
    DBFile testFile;
    int attrs[1] = {0};
    Type attTypes[1] = {Int}; 
    OrderMaker *o = new OrderMaker; 
    o->setAttributes(attrs,attTypes,1);
    struct {OrderMaker *o; int l;} startup = {o, 1};
    ASSERT_EQ(1,testFile.Create("testfile1.bin",sorted,&startup));
    testFile.Close();
    remove("testfile1.bin");
    remove("testfile1.bin.meta");
    delete o;
}

TEST(SortedFileTest,SortedFileMetaData){
    DBFile testFile; 
    int attrs[1] = {0};
    Type attTypes[1] = {Int}; 
    OrderMaker *o = new OrderMaker; 
    o->setAttributes(attrs,attTypes,1);
    struct {OrderMaker *o; int l;} startup = {o, 1};
    testFile.Create("testfile.bin",sorted, &startup);
    FILE *sortedFile = fopen("testfile.bin.meta","r"); 
    char *fileType = new char[100];
    fscanf(sortedFile, "%s", fileType); 
    ASSERT_EQ(0, strcmp(fileType, "sorted"));
    testFile.Close(); 
    fclose(sortedFile);
    delete[] fileType;
    delete o;
    remove("testfile.bin"); 
    remove("testfile.bin.meta");
}


TEST(SortedFileTest,FileOpenTest){
  DBFile testFile;
  int attrs[1] = {0};
  Type attTypes[1] = {Int}; 
  OrderMaker *o = new OrderMaker; 
  o->setAttributes(attrs,attTypes,1);
  struct {OrderMaker *o; int l;} startup = {o, 1};
  testFile.Create("testfile.bin",sorted, &startup);
  testFile.Close(); 
  ASSERT_EQ(1,testFile.Open("testfile.bin"));
  testFile.Close();
  delete o;
  remove("testfile.bin"); 
  remove("testfile.bin.meta");
}

TEST(SortedFileTest,GetNextSortedFile){
DBFile *testFile= new DBFile;
Record *temp = new Record;
Schema mySchema ("catalog", "nation");
int attrs[1] = {0};
  Type attTypes[1] = {Int}; 
  OrderMaker *o = new OrderMaker; 
  o->setAttributes(attrs,attTypes,1);
  struct {OrderMaker *o; int l;} startup = {o, 1};
  testFile->Create("testfile.bin",sorted, &startup);
  testFile->Load(mySchema, "nation.tbl");
  testFile->Close(); 
  delete testFile; 
  testFile = new DBFile; 
  testFile->Open("testfile.bin");
  testFile->MoveFirst(); 
  testFile->GetNext(*temp);
  testing::internal::CaptureStdout();
  temp->Print(&mySchema);
  ASSERT_EQ("n_nationkey: [0], n_name: [ALGERIA], n_regionkey: [0], n_comment: [ haggle. carefully final deposits detect slyly agai]\n",testing::internal::GetCapturedStdout());
  testFile->Close();
  delete testFile; 
  delete temp;
  delete o;
  remove("testfile.bin"); 
  remove("testfile.bin.meta");
}


TEST(DBFileHeapTest,FileOpenTest){
  DBFile testFile;
  testFile.Create("testfile.bin",heap,NULL);
  ASSERT_EQ(1,testFile.Open("testfile.bin"));
  testFile.Close();
  remove("testfile.bin");
  remove("testfile.bin.meta");
}

TEST(DBFileHeapTest,GetNextAfterHardCloseTest){
DBFile *testFile = new DBFile;
Record temp;
Schema mySchema ("catalog", "nation");
testFile->Create("heapfile.bin",heap,NULL);
testFile->Load(mySchema,"nation.tbl");
testFile->Close();
delete testFile; 
testFile = new DBFile; 
testFile->Open("heapfile.bin");
testFile->MoveFirst();
testFile->GetNext(temp);
testing::internal::CaptureStdout();
temp.Print(&mySchema);
ASSERT_EQ("n_nationkey: [0], n_name: [ALGERIA], n_regionkey: [0], n_comment: [ haggle. carefully final deposits detect slyly agai]\n",testing::internal::GetCapturedStdout());
testFile->Close(); 
delete testFile; 
remove("heapfile.bin");
remove("heapfile.bin.meta");
}

TEST(HandlerClass,ReadAndWriteTransitionsTest){
  HeapFileHandler handler;
  File file;
  Page page;
  off_t whichPage=0;
  //ReadToWrite Transition
  file.Open(0,(char *)"testfile.bin");
  char val=handler.getCurrentState();
  ASSERT_EQ('r',val);
  handler.writeHandler(file,page,whichPage);
  val=handler.getCurrentState();
  ASSERT_EQ('w',val);

  //WriteToRead Transition
  int currentRecord=0;
  handler.readHandler(&file,page,whichPage,currentRecord);
  ASSERT_EQ('r',handler.getCurrentState());
  ASSERT_EQ(2,file.GetLength());
  file.Close(); 
  remove("testfile.bin");
}

TEST(HandlerClass,ReadToWriteTransitionFollowedbyTearDownTest){
  HeapFileHandler handler;
  File file;
  Page page;
  off_t whichPage=0;
  //ReadToWrite Transition
  file.Open(0,(char *)"test1.bin");
  char val=handler.getCurrentState();
  ASSERT_EQ('r',val);
  handler.writeHandler(file,page,whichPage);
  val=handler.getCurrentState();
  ASSERT_EQ('w',val);

  //TearDown
  handler.tearDown(file,page,whichPage);
  ASSERT_EQ('r',handler.getCurrentState());
  remove("test1.bin");

}

TEST(HandlerClass,ReadAndWriteTransitionsWithInvalidLastReadRecordTest){
  HeapFileHandler handler;
  File file;
  Page page;
  off_t whichPage=0;
  //ReadToWrite Transition
  file.Open(0,(char *)"test1.bin");
  char val=handler.getCurrentState();
  ASSERT_EQ('r',val);
  handler.writeHandler(file,page,whichPage);
  val=handler.getCurrentState();
  ASSERT_EQ('w',val);

  //WriteToRead Transition
  int lastReadRecord=4;
  handler.readHandler(&file,page,whichPage,lastReadRecord);
  ASSERT_EQ('r',handler.getCurrentState());
  ASSERT_EQ(2,file.GetLength());
  remove("test1.bin");
}

TEST(HandlerClass,PopRecordsFromPageWithValidNoOfRecordsTest){
  HeapFileHandler handler;
  Page page;
  ASSERT_EQ(1,handler.popRecordsFromCurPage(page,0));
}

TEST(HandlerClass,PopRecordsFromPageWithInValidNoOfRecordsTest){
  HeapFileHandler handler;
  Page page;
  ASSERT_EQ(0,handler.popRecordsFromCurPage(page,5));
}

TEST(HandlerClass,InitializationTest){
  HeapFileHandler handler;
  Page page;
  off_t whichPage=0;
  int currentRecord=0;
  handler.init(page,whichPage,currentRecord);
  ASSERT_EQ('r',handler.getCurrentState());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}