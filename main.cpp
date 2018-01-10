#include "core/manager.h"
#include "messages.h"
#include <stdio.h>
#include <unistd.h>

using namespace bx;

class GameComponent : public Component
{
public:
  GameComponent(std::string name) : Component(name){}
  ~GameComponent(){}

  virtual void Update(){};

};


class Physics : public GameComponent
{
public:
  Physics(std::string name):GameComponent(name){}
  ~Physics(){}

  void UserInit(){
    SubscribeToContainerMessage(&Physics::AddVelocity, "move", this->GetParent()->GetID());
    SubscribeToContainerMessage(&Physics::GetPos, "get_pos", this->GetParent()->GetID());
  };

  void Update(){
    y_pos -= 9.8; // apply gravity
  }

private:
  float x_pos = 0;
  float y_pos = 0;


  void AddVelocity(Message & msg){
    VelocityMessage * vel = static_cast<VelocityMessage*>(&msg);
    x_pos += vel->x;
    y_pos += vel->y;
  }

  void GetPos(Message & msg){
    PositionMessage * pos = static_cast<PositionMessage*>(&msg);
    pos->x = x_pos;
    pos->y = y_pos;
  }

  void ApplyForce(Message & msg){
    VectorMessage * vec = static_cast<VectorMessage*>(&msg);
    // do math
  }



};


class Controls : public GameComponent
{
public:
  Controls(std::string name):GameComponent(name){}
  ~Controls(){}
  void UserInit(){
  }

  void Update(){
    UpArrowPressed();
  }


private:

  void UpArrowPressed(){
    VelocityMessage vel;
    vel.x = 0;
    vel.y = 13;
    PublishMessageToContainer(vel, "move", this->GetParent()->GetID());
  }

};


class State : public GameComponent
{
public:
  State(std::string name):GameComponent(name){}
  ~State(){}
  void UserInit(){
  }

  void Update(){
    PrintState();
  }

private:

  void PrintState(){
    PositionMessage pos;
    PublishMessageToContainer(pos, "get_pos", this->GetParent()->GetID());
    printf("State: X position is %f, Y position is %f\n", pos.x, pos.y);
  }

};




class Entity : public Container{
public:
  Entity(std::string name) : Container(name){}
  virtual ~Entity(){}

  void Update(){
    for (auto iter = cont_begin(); iter != cont_end(); iter++){
      Entity *ent = static_cast<Entity*>(GetContainer(iter->second));
      ent->Update();
    }

    for (auto iter = comp_begin(); iter != comp_end(); iter++){
      GameComponent *comp = static_cast<GameComponent*>(GetComponent(iter->second));
      comp->Update();
    }
  };


};


class Supervisor : public Manager
{

public:
  Supervisor(std::string name) : Manager(name){}
  virtual ~Supervisor(){}

  void Update(){
    for (auto iter = cont_begin(); iter != cont_end(); iter++){
      Entity *ent = static_cast<Entity*>(GetContainer(iter->second));
      ent->Update();
    }
  }

};


int global_cnt = 0;




class TestComponent : public Component
{
public:
  TestComponent(int num, std::string name) : Component(name) {testValue = num;}

  void UserInit(){
    SubscribeToContainerMessage(&TestComponent::Test, GetName(), std::to_string(testValue-1));
    SubscribeToContainerMessage(&TestComponent::BIG, "BIG", 0);
  }

  void Update(){
    TestPublish();
  }

  void BIG(Message & msg){
    TestMessage * test = static_cast<TestMessage*>(&msg);
    if (test->test != 100){
      puts("FCUKED AGAIN");
    }
  }

  void TestBIG(){
    TestMessage test;
    test.test = 100;
    PublishMessageToContainer(test, "BIG", 0);
  }


  void TestPublish(){
    if (testValue-1 < 0){return;}
    TestMessage test;
    PublishMessageToContainer(test, GetName(), testValue-1);
    if (test.test != testValue-1){
      puts("FUCKED");
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
      TestComponent *testComp = static_cast<TestComponent*>(GetComponent(iter->second));
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


// Test using ID's instead of names
// Change names to blocks and boxes
// Find better way to print debug messages
// Clean up core interfaces
// test it out


void BuildTree(int num, Container * cont){
  if (num <= 0) return;

  for (int i = 0; i < 3; i++){
    TestContainer * tempCont = new TestContainer(std::to_string(global_cnt));
    cont->AddContainer(tempCont);
    for (int j = 0; j < 10; j++){
      TestComponent * tempComp = new TestComponent(global_cnt,std::to_string(j));
      tempCont->AddComponents({tempComp});
      //tempComp->TestPublish();
    }

    global_cnt++;
    BuildTree(num-1, tempCont);
  }
}

void DestroyTree(Container * cont){
  for (auto iter = cont->cont_begin(); iter != cont->cont_end(); iter++){
    TestContainer * testCont = static_cast<TestContainer*>(cont->GetContainer(iter->second));
    DestroyTree(testCont);
  }
  for (auto iter = cont->cont_begin(); iter != cont->cont_end(); iter++){
    cont->RemoveContainer(iter->second);
  }
}



int main(void){
  TestManager * sup = new TestManager("supervisor");
  Entity * characters = new Entity("Characters");
  Entity * vehicles = new Entity("Vehicles");

  Entity * car = new Entity("Car");
  Entity * rocket = new Entity("Rocket");
  Entity * enemy = new Entity("Enemy");
  Entity * player = new Entity("Player");


  BuildTree(3, sup);

  sup->Update();

  DestroyTree(sup);
  sup->Update();

/*
  sup->AddContainer(characters);
  sup->AddContainer(vehicles);

  characters->AddContainer(enemy);
  characters->AddContainer(player);
  vehicles->AddContainer(rocket);
  vehicles->AddContainer(car);

  enemy->AddComponents({
    new Controls("controls"),
    new Physics("physics"),
    new State("state"),
  });

  player->AddComponents({
    new Controls("controls"),
    new Physics("physics"),
    new State("state"),
  });

  rocket->AddComponents({
    new Physics("physics"),
    new State("state"),
  });

  car->AddComponents({
    new Physics("physics"),
    new State("state"),
  });

*/
/*
  for (int i = 0; i < 3; i ++){
  //while(1){
    sup->Update();
    sleep(1);
  }

  car->RemoveComponent("physics");
  player->RemoveComponent("state");

  for (int i = 0; i < 3; i ++){
    sup->Update();
    sleep(1);
  }


*/


}
