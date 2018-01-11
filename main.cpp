#include "core/manager.h"
#include "messages.h"
#include <stdio.h>
#include <unistd.h>

using namespace bx;


int global_cnt = 0;
#define TEST_VALUE 100

class TestComponent : public Component
{
public:
  TestComponent(int num, std::string name) : Component(name) {testValue = num;}

  void UserInit(){
    SubscribeToContainerMessage(&TestComponent::Test, GetName(), std::to_string(testValue));
    SubscribeToContainerMessage(&TestComponent::BIG, "BIG", 0);
  }

  void Update(){
    TestPublish();
  }

  void BIG(Message & msg){
    TestMessage * test = static_cast<TestMessage*>(&msg);
    if (test->test != TEST_VALUE){
      DebugLog(BLOX_ERROR, "Publish Failed", "Component " + GetName() + " Parent " + GetParent()->GetName()+ " - Expected:" + std::to_string(TEST_VALUE)+ " Got:" +  std::to_string(test->test));
    }
  }

  void TestBIG(){
    TestMessage test;
    test.test = TEST_VALUE;
    PublishMessageToContainer(test, "BIG", 0);
  }


  void TestPublish(){
    if (testValue == 0) return; // Edge Case
    TestMessage test;
    int rv = PublishMessageToContainer(test, GetName(), std::to_string(testValue-1));
    if (rv != 0) return;
    if (test.test != testValue-1){
      DebugLog(BLOX_ERROR, "Publish Failed", "Component " + GetName() + " Parent " + GetParent()->GetName()+ " - Expected:" + std::to_string(testValue-1)+ " Got:" +  std::to_string(test.test));
    }
  }

private:
  int testValue = 0;

  void Test(Message & msg){
    TestMessage * test = static_cast<TestMessage*>(&msg);
    test->test = testValue;
  }
};

class TestContainer : public Container
{

public:
  TestContainer(std::string name) : Container(name){}

  void Update(){

    for (auto iter = cont_begin(); iter != cont_end(); iter++){
      TestContainer *testCont = static_cast<TestContainer*>(GetContainer(iter->second));
      testCont->Update();
    }

    for (auto iter = comp_begin(); iter != comp_end(); iter++){
      Component * comp = GetComponent(iter->second);
      TestComponent *testComp = static_cast<TestComponent*>(comp);
      testComp->Update();
    }
  }
};


class TestManager : public Manager
{

public:
  TestManager(std::string name) : Manager(name){}
  virtual ~TestManager(){}

  void Update(){
    TestContainer *testCont;
    for (auto iter = cont_begin(); iter != cont_end(); iter++){
      testCont = static_cast<TestContainer*>(GetContainer(iter->second));
      testCont->Update();
    }
    TestComponent * temp = static_cast<TestComponent*>(testCont->GetComponent(0));
    temp->TestBIG();
  }

};




void BuildTree(int num, Container * cont){
  if (num <= 0) return;

  for (int i = 0; i < 3; i++){
    TestContainer * tempCont = new TestContainer(std::to_string(global_cnt));
    cont->AddContainer(tempCont);
    for (int j = 0; j < 3; j++){
      TestComponent * tempComp = new TestComponent(global_cnt,std::to_string(j));
      tempCont->AddComponents({tempComp});
    }

    global_cnt++;
    BuildTree(num-1, tempCont);
  }
}

void BuildTreeBackwards(int num, Container * cont){
  if (num <= 0) return;

  for (int i = 0; i < 3; i++){
    TestContainer * tempCont = new TestContainer(std::to_string(global_cnt));
    for (int j = 0; j < 3; j++){
      TestComponent * tempComp = new TestComponent(global_cnt,std::to_string(j));
      tempCont->AddComponents({tempComp});
    }
    global_cnt++;
    BuildTree(num-1, tempCont);
    cont->AddContainer(tempCont);
  }
}

void DestroyTree(Container * cont){
  for (auto iter = cont->cont_begin(); iter != cont->cont_end(); iter++){
    TestContainer * testCont = static_cast<TestContainer*>(cont->GetContainer(iter->second));
    DestroyTree(testCont);
  }

  for (auto iter = cont->comp_begin(); iter != cont->comp_end(); iter++){
    TestComponent * testComp = static_cast<TestComponent*>(cont->GetComponent(iter->second));
    delete(testComp);
  }
  delete(cont);
}


void DestroyTreeBackwards(Container * cont){
  std::vector<TestContainer*> containers;
  std::vector<TestComponent*> components;

  for (auto iter = cont->cont_begin(); iter != cont->cont_end(); iter++){
    containers.push_back(static_cast<TestContainer*>(cont->GetContainer(iter->second)));
  }
  for (auto iter = cont->comp_begin(); iter != cont->comp_end(); iter++){
    components.push_back(static_cast<TestComponent*>(cont->GetComponent(iter->second)));
  }

  delete(cont);

  for (int i = 0; i < components.size(); i++){
    delete(components.at(i));
  }

  for (int i = 0; i < containers.size(); i++){
    DestroyTreeBackwards(containers.at(i));
  }

}



// Test using ID's instead of names
// Change names to blocks and boxes
// Redesign Debugger to support better messages and handle testing


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


int main(void){

  Test1(5);
  Test2(5);
  Test3(5);
  Test4(5);
  printf("Errors:%d\n", errors);

}
