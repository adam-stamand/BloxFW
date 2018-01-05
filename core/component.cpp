#include "component.h"
#include "manager.h"
#include <stdio.h>
#include <iostream>


using namespace bx;


ComponentID Component::GetID(){
  return this->id;
}


ComponentID Component::GetParentID(){
  return this->parentID;
}


Manager * Component::GetManager (){
  return this->manager;
}


std::string Component::GetName(){
  return this->name;
}


std::string Component::Print(){
  return "Component: " + this->GetName() + "/" + std::to_string(this->GetID()) + "; Container: " + \
  manager->GetContainer(this->GetParentID())->GetName() + "/" + std::to_string(this->GetParentID()) + "; ";
}


void Component::SetID(ComponentID compID){
  assert(compID != 0);
  this->id = compID;
}


void Component::SetParentID(ContainerID contID){
  assert(contID != 0);
  this->parentID = contID;
}


void Component::SetManager(Manager * manager){
  assert(manager != NULL);
  this->manager = manager;
}


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


void Component::PublishHelper(Message const & msg, std::string msgIdentifier, std::string contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}
void Component::PublishHelper(Message const & msg, MessageID msgIdentifier, std::string contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}
void Component::PublishHelper(Message const & msg, std::string msgIdentifier, ContainerID contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}
void Component::PublishHelper(Message const & msg, MessageID msgIdentifier, ContainerID contIdentifier){
  manager->Publish(msg, msgIdentifier, contIdentifier);
}
