#include "entitymanager.h"
#include <stdio.h>


class President : public Component
{
public:
  President(std::string name):Component(name){
  }
  ~President(){}

  void AddedToEntity(){};

  void control(){PublishGlobalMessage("Control");}
  std::string Name = "PRESIDENTE";
};


class Person : public Component
{
public:
  Person(std::string name):Component(name){}
  ~Person(){}
  int age = 30;
  void test(){PublishLocalMessage("Birthday");}

  void AddedToEntity(){
    SubscribeGlobalMessage("Control", &Person::Control);
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

 void AddedToEntity(){
   SubscribeLocalMessage("Birthday", &Company::Birthday);
   SubscribeGlobalMessage("Control", &Company::Control);
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

  EntityManager manager;

  EntityID id = manager.CreateEntity("entity1");
  manager.AddComponents(id, {jeff, intel});

  id = manager.CreateEntity("entity2");
  manager.AddComponents(id, {hp, bob, pres});

  jeff->test();
  bob->test();
  pres->control();

  manager.RemoveComponent(id, "Jeff");

  puts("hello");
}
