#include "core/manager.h"
#include <stdio.h>

using namespace bx;

class GameComponent : public Component
{
public:
  GameComponent(std::string name) : Component(name){}
  ~GameComponent(){}

  virtual void Update(){};

};


class MovementMessage : public Message
{
public:
  float moveX = 0;
  float moveY = 0;
};


class Physics : public GameComponent
{
public:
  Physics(std::string name):GameComponent(name){}
  ~Physics(){}

  void UserInit(){
    SubscribeToContainerMessage(&Physics::Move, "move", this->GetParent()->GetID());
    SubscribeToContainerMessage(&Physics::GetPos, "get_pos", this->GetParent()->GetID());
  };

private:
  float x = 0;
  float y = 0;

  void Move(Message & msg){
    MovementMessage * move = static_cast<MovementMessage*>(&msg);
    x += move->moveX;
    y += move->moveY;
  }

  void GetPos(Message & msg){
    MovementMessage * move = static_cast<MovementMessage*>(&msg);
    move->moveX = x;
    move->moveY = y;
  }
};


class Controls : public GameComponent
{
public:
  Controls(std::string name):GameComponent(name){}
  ~Controls(){}
  void UserInit(){
    //SubscribeToContainerMessage(&Person::Control, "Control", "manager");
  }

  void UpArrowPressed(){
    MovementMessage move;
    move.moveX = 5;
    move.moveY = 3;
    PublishMessageToContainer(move, "move", this->GetParent()->GetID());
  }

  void Update(){
    UpArrowPressed();
  }

};

/*
class Graphics : public GameComponent
{
public:

};
*/

class State : public GameComponent
{
public:
  State(std::string name):GameComponent(name){}
  ~State(){}
  void UserInit(){
    //SubscribeToContainerMessage(&Person::Control, "Control", "manager");
  }

  void PrintState(){
    MovementMessage move;
    PublishMessageToContainer(move, "get_pos", this->GetParent()->GetID());
    printf("State: X position is %f, Y position is %f\n", move.moveX, move.moveY);
  }

  void Update(){
    PrintState();
  }
};

class Entity : public Container{
public:
  Entity(std::string name) : Container(name){}
  virtual ~Entity(){}

  void Update(){
    for (auto iter = components.begin(); iter != components.end(); iter++){
      GameComponent *comp = static_cast<GameComponent*>(GetComponent(iter->second));
      comp->Update();
    }
  };


};

// Add unsubscribe and make sure remove component calls it
// Make friend functions
// FIgure out why manager has to be friend
// Fix up Container interface //provide a safe way to iterate through labeled boxes
// Test using ID's instead of names
// Create message file with a bunch of message types
// Change names to blocks and boxes
// Find better way to print debug messages
// Clean up core interfaces
// test it out

int main(void){
  Manager * manager = new Manager("manager");
  Entity * character = new Entity("Character");
  Entity * enemy = new Entity("Enemy");

  manager->AddContainer(character);
  character->AddContainer(enemy);

  enemy->AddComponents({
    new Controls("controls"),
    new Physics("physics"),
    new State("state"),
  });


  character->AddComponents({
    new Controls("controls"),
    new Physics("physics"),
    new State("state"),
  });

  character->RemoveComponent("physics");
  character->Update();



}
