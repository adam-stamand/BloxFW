#include "../BloxFW/core/ManagerBox.h"
#include "messages.h"
#include <stdio.h>
#include <unistd.h>

using namespace bx;


int global_cnt = 0;
#define TEST_VALUE 100

class TestBlock : public Block
{
public:
  TestBlock(int num, std::string name) : Block(name) {testValue = num;}

  void UserInit(){
    SubscribeMessage(&TestBlock::Test, GetName(), std::to_string(testValue));
    SubscribeMessage(&TestBlock::BIG, "BIG", 0);
  }

  void Update(){
    TestPublish();
  }

  void BIG(Message & msg){
    TestMessage * test = static_cast<TestMessage*>(&msg);
    if (test->test != TEST_VALUE){
      DebugLog(BLOX_ERROR, "Publish Failed", "Block " + GetName() + " Box " + GetBox()->GetName()+ " - Expected:" + std::to_string(TEST_VALUE)+ " Got:" +  std::to_string(test->test));
    }
  }

  void TestBIG(){
    TestMessage test;
    test.test = TEST_VALUE;
    PublishMessage(test, "BIG", 0);
  }


  void TestPublish(){
    if (testValue == 0) return; // Edge Case
    TestMessage test;
    int rv = PublishMessage(test, GetName(), std::to_string(testValue-1));
    if (rv != 0) return;
    if (test.test != testValue-1){
      DebugLog(BLOX_ERROR, "Publish Failed", "Block " + GetName() + " Box " + GetBox()->GetName()+ " - Expected:" + std::to_string(testValue-1)+ " Got:" +  std::to_string(test.test));
    }
  }

private:
  int testValue = 0;

  void Test(Message & msg){
    TestMessage * test = static_cast<TestMessage*>(&msg);
    test->test = testValue;
  }
};

class TestBox : public Box
{

public:
  TestBox(std::string name) : Box(name){}

  void Update(){

    for (auto iter = box_begin(); iter != box_end(); iter++){
      TestBox *testBox = static_cast<TestBox*>(GetBox(iter->second));
      testBox->Update();
    }

    for (auto iter = block_begin(); iter != block_end(); iter++){
      Block * block = GetBlock(iter->second);
      TestBlock *testBlock = static_cast<TestBlock*>(block);
      testBlock->Update();
    }
  }
};


class TestManager : public ManagerBox
{

public:
  TestManager(std::string name) : ManagerBox(name){}
  virtual ~TestManager(){}

  void Update(){
    TestBox *testBox;
    for (auto iter = box_begin(); iter != box_end(); iter++){
      testBox = static_cast<TestBox*>(GetBox(iter->second));
      testBox->Update();
    }
    TestBlock * temp = static_cast<TestBlock*>(testBox->GetBlock(0));
    temp->TestBIG();
  }

};




void BuildTree(int num, Box * box){
  if (num <= 0) return;

  for (unsigned int i = 0; i < 3; i++){
    TestBox * tempBox = new TestBox(std::to_string(global_cnt));
    box->AddBox(tempBox);
    for (int j = 0; j < 3; j++){
      TestBlock * tempBlock = new TestBlock(global_cnt,std::to_string(j));
      tempBox->AddBlocks({tempBlock});
    }

    global_cnt++;
    BuildTree(num-1, tempBox);
  }
}

void BuildTreeBackwards(int num, Box * box){
  if (num <= 0) return;

  for (unsigned int i = 0; i < 3; i++){
    TestBox * tempBox = new TestBox(std::to_string(global_cnt));
    for (int j = 0; j < 3; j++){
      TestBlock * tempBlock = new TestBlock(global_cnt,std::to_string(j));
      tempBox->AddBlocks({tempBlock});
    }
    global_cnt++;
    BuildTree(num-1, tempBox);
    box->AddBox(tempBox);
  }
}

void DestroyTree(Box * box){
  for (auto iter = box->box_begin(); iter != box->box_end(); iter++){
    TestBox * testBox = static_cast<TestBox*>(box->GetBox(iter->second));
    DestroyTree(testBox);
  }

  for (auto iter = box->block_begin(); iter != box->block_end(); iter++){
    TestBlock * testBlock = static_cast<TestBlock*>(box->GetBlock(iter->second));
    delete(testBlock);
  }
  delete(box);
}


void DestroyTreeBackwards(Box * box){
  std::vector<TestBox*> boxes;
  std::vector<TestBlock*> blocks;

  for (auto iter = box->box_begin(); iter != box->box_end(); iter++){
    boxes.push_back(static_cast<TestBox*>(box->GetBox(iter->second)));
  }
  for (auto iter = box->block_begin(); iter != box->block_end(); iter++){
    blocks.push_back(static_cast<TestBlock*>(box->GetBlock(iter->second)));
  }

  delete(box);

  for (unsigned int i = 0; i < blocks.size(); i++){
    delete(blocks.at(i));
  }

  for (unsigned int i = 0; i < boxes.size(); i++){
    DestroyTreeBackwards(boxes.at(i));
  }

}





void Test1(int levels){
  puts( "-----------------------------------------------------\n"\
        "--------------------Test 1---------------------------\n"\
        "-----------------------------------------------------\n"\
       );
  global_cnt = 0;
  TestManager * sup = new TestManager("supervisor");
  BuildTree(levels, sup);
  sup->Update();
  DestroyTree(sup);
  global_cnt = 0;
}

void Test2(int levels){
  puts( "-----------------------------------------------------\n"\
        "--------------------Test 2---------------------------\n"\
        "-----------------------------------------------------\n"\
       );
  global_cnt = 0;
  TestManager * sup = new TestManager("supervisor");
  BuildTreeBackwards(levels, sup);
  sup->Update();
  DestroyTreeBackwards(sup);
  global_cnt = 0;
}

void Test3(int levels){
  puts( "-----------------------------------------------------\n"\
        "--------------------Test 3---------------------------\n"\
        "-----------------------------------------------------\n"\
       );
  global_cnt = 0;
  TestManager * sup = new TestManager("supervisor");
  BuildTree(levels, sup);
  sup->Update();
  DestroyTreeBackwards(sup);
  global_cnt = 0;
}

void Test4(int levels){
  puts( "-----------------------------------------------------\n"\
        "--------------------Test 4---------------------------\n"\
        "-----------------------------------------------------\n"\
       );
  global_cnt = 0;
  TestManager * sup = new TestManager("supervisor");
  BuildTreeBackwards(levels, sup);
  sup->Update();
  DestroyTree(sup);
  global_cnt = 0;
}

// Test using ID's instead of names
// Change names to blocks and boxes
// Redesign Debugger to support better messages and handle testing

int main(void){

  Test1(5);
  Test2(5);
  Test3(5);
  Test4(5);
  printf("Errors:%d\n", errors);

}
