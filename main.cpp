#include "core/manager.h"
#include <stdio.h>

using namespace bx;

class MovementMessage : public Message
{
public:
  float moveX = 0;
  float moveY = 0;
};


class Physics : public Component
{
public:
  Physics(std::string name):Component(name){}
  ~Physics(){}

  void AddedToContainer(){
    SubscribeToContainerMessage(&Physics::Move, "move", "Character");
    SubscribeToContainerMessage(&Physics::GetPos, "get_pos", "Character");
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


class Controls : public Component
{
public:
  Controls(std::string name):Component(name){}
  ~Controls(){}
  void AddedToContainer(){
    //SubscribeToContainerMessage(&Person::Control, "Control", "manager");
  }

  void UpArrowPressed(){
    MovementMessage move;
    move.moveX = 5;
    move.moveY = 3;
    PublishMessageToContainer(move, "move", "Character");
  }

};


class Graphics : public Component
{
public:

};

class State : public Component
{
public:
  State(std::string name):Component(name){}
  ~State(){}
  void AddedToContainer(){
    //SubscribeToContainerMessage(&Person::Control, "Control", "manager");
  }

  void PrintState(){
    MovementMessage move;
    PublishMessageToContainer(move, "get_pos", "Character");
    printf("State: X position is %f, Y position is %f\n", move.moveX, move.moveY);
  }

};



int main(void){

  Controls * controls = new Controls("control");
  Physics * physics = new Physics("physics");
  State * state = new State("state");

  Manager manager("manager");

  manager.CreateContainer("Character");

  manager.InsertComponents({physics, controls, state}, "Character");

  controls->UpArrowPressed();

  state->PrintState();

}
