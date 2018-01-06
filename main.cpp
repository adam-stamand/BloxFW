#include "core/manager.h"
#include <stdio.h>

using namespace bx;


class Adult : public Component
{
public:
  President(std::string name):Component(name){
  }
  ~President(){}

  void AddedToContainer(){};

  void control(){PublishMessageToContainer("Control", "cont1");}
};


class Child : public Component
{
public:
  Person(std::string name):Component(name){}
  ~Person(){}
  int age = 30;
  void test(){PublishMessageToContainer("Birthday", "cont1" );}
  void test2(){PublishMessageToContainer("Birthday", "manager" );}

  void AddedToContainer(){
    SubscribeToContainerMessage(&Person::Control, "Control", "manager");
  }

private:
  void Control(Message const & msg){
    President * pres = (President*)msg.publisher;
    printf("person controlled by %s\n", pres->GetName().c_str());
  }
};

class Company : public Component
{
public:
  Company(std::string name):Component(name){
  }
  ~Company(){}

 void AddedToContainer(){
   SubscribeToContainerMessage(&Company::Birthday,"Birthday",  "manager");
   SubscribeToContainerMessage(&Company::Birthday,"Birthday",  "cont1");
   SubscribeToContainerMessage(&Company::Control,"Control",  "cont1");
 }


private:
  void Birthday(Message const & msg){
    Person * person = (Person*)msg.publisher;
    printf("Company %s reporting employee %s  is %d years old.\n", GetName().c_str(), person->GetName().c_str(), person->age);
  }

  void Control(Message const & msg){
    President * pres = (President*)msg.publisher;
    printf("Company %s controlled by %s\n", GetName().c_str(), pres->GetName().c_str());
  }

};



int main(void){
  Person * jeff = new Person("Jeff");
  Person * bob = new Person("Bob");
  Person * mike = new Person("Mike");
  Person * will = new Person("Will");


  Company * hp = new Company("HP");
  Company * intel = new Company("Intel");
  President * pres = new President("President Sam");
  bob->age = 20;
  mike->age = 90;

  Manager manager("manager");

  manager.InsertComponents({mike});

  manager.CreateContainer("cont1");
  manager.InsertComponents({jeff, hp}, "cont1");
  manager.InsertComponents({intel}, "cont1");

  manager.CreateContainer("cont2");
  manager.InsertComponents({bob, pres}, "cont2");

  manager.CreateContainer("cont2", "cont3");
  manager.InsertComponents({will}, "cont3");

  //jeff->test();
  bob->test();
  pres->control();
  mike->test2();
  will->test2();
  manager.EraseComponent("Jeff", "cont1");

}
