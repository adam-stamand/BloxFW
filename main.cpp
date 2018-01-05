#include "core/manager.h"
#include <stdio.h>

using namespace bx;


class President : public Component
{
public:
  President(std::string name):Component(name){
  }
  ~President(){}

  void AddedToContainer(){};

  void control(){PublishMessageToContainer("Control", "cont1");}
  std::string Name = "PRESIDENTE";
};


class Person : public Component
{
public:
  Person(std::string name):Component(name){}
  ~Person(){}
  int age = 30;
  void test(){PublishMessageToContainer("Birthday", "cont1" );}

  void AddedToContainer(){
    SubscribeToContainerMessage(&Person::Control, "Control", "cont1");
  }

private:
  void Control(Message const & msg){
    President * pres = (President*)msg.publisher;
    printf("person controlled by %s\n", pres->Name.c_str());
  }
};

class Company : public Component
{
public:
  Company(std::string name):Component(name){
  }
  ~Company(){}

 void AddedToContainer(){
   SubscribeToContainerMessage(&Company::Birthday,"Birthday",  "cont1");
   SubscribeToContainerMessage(&Company::Control,"Control",  "cont1");
 }


private:
  void Birthday(Message const & msg){
    Person * person = (Person*)msg.publisher;
    printf("age = %d\n", person->age);
  }

  void Control(Message const & msg){
    President * pres = (President*)msg.publisher;
    printf("company controlled by %s\n", pres->Name.c_str());
  }

};



int main(void){
  Person * jeff = new Person("Jeff");
  Person * bob = new Person("Bob");
  Company * hp = new Company("HP");
  Company * intel = new Company("Intel");
  President * pres = new President("Jerry");
  bob->age = 20;

  Manager manager("Manager");

  ContainerID id = manager.CreateContainer("cont1");
  manager.InsertComponents({jeff, hp}, "cont1");
  manager.InsertComponents({intel}, "cont1");
  id = manager.CreateContainer("cont2");
  manager.InsertComponents({hp, bob, pres}, "cont2");

  jeff->test();
  bob->test();
  pres->control();
  manager.EraseComponent("Jeff", "cont1");

}
