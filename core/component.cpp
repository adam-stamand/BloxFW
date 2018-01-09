#include "component.h"
#include "manager.h"
#include <stdio.h>
#include <iostream>


using namespace bx;


//ComponentID Component::GetID(){
//  return this->id;
//}


Container * Component::GetParent(){
  return this->parent;
}


Manager * Component::GetManager (){
  return this->manager;
}


std::string Component::GetName(){
  return this->name;
}


std::string Component::Print(){
  if (manager == NULL){
    puts("Failed print: null manager");
    return "";
  }
  return "Component: " + this->GetName() + "/" + std::to_string(this->GetID()) + "; Container: " + \
  manager->GetContainer(this->GetParentID())->GetName() + "/" + std::to_string(this->GetParentID()) + "; ";
}


//void Component::SetID(ComponentID compID){
//  this->id = compID;
//}


void Component::SetParent(Container * cont){
  assert(cont != NULL);
  this->parent = cont;
}


void Component::SetManager(Manager * manager){
  assert(manager != NULL);
  this->manager = manager;
}


//void Component::SetInit(bool state){
//  this->initialized = state;
//}


void Component::SubscribeHelper(Subscription sub, std::string msgIdentifier, std::string contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


void Component::SubscribeHelper(Subscription sub, MessageID msgIdentifier, std::string contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


void Component::SubscribeHelper(Subscription sub, std::string msgIdentifier, ContainerID contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


void Component::SubscribeHelper(Subscription sub, MessageID msgIdentifier, ContainerID contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


void Component::PublishHelper(Message & msg, std::string msgIdentifier, std::string contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}


void Component::PublishHelper(Message  & msg, MessageID msgIdentifier, std::string contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}


void Component::PublishHelper(Message  & msg, std::string msgIdentifier, ContainerID contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}


void Component::PublishHelper(Message & msg, MessageID msgIdentifier, ContainerID contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}
