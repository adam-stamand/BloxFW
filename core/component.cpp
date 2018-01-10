#include "component.h"
#include "manager.h"
#include <stdio.h>
#include <iostream>


using namespace bx;


void Component::SetParent(Container * cont){
  this->parent = cont;
}

ComponentID Component::GetParentID(){
  return this->parent->GetID();
}

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
  return "Component: " + this->GetName() + "; Container: " + this->GetParent()->GetName();
}


void Component::AddedToManager(Manager * manager){
  assert(manager != NULL);
  this->manager = manager;
  this->UserInit();
}


void Component::RemovedFromManager(){
  this->manager = NULL;
}



int Component::SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string contIdentifier){
  return manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


int Component::SubscribeHelper(Subscription &sub, MessageID msgIdentifier, std::string contIdentifier){
  return manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


int Component::SubscribeHelper(Subscription &sub, std::string msgIdentifier, ContainerID contIdentifier){
  return manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


int Component::SubscribeHelper(Subscription &sub, MessageID msgIdentifier, ContainerID contIdentifier){
  return manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message & msg, std::string msgIdentifier, std::string contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message  & msg, MessageID msgIdentifier, std::string contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message  & msg, std::string msgIdentifier, ContainerID contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message & msg, MessageID msgIdentifier, ContainerID contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}
