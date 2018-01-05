#include "component.h"
#include "Manager.h"
#include <stdio.h>
#include <iostream>


using namespace bx;


ComponentID Component::GetComponentID(){
  return this->compID;
}


ComponentID Component::GetContainerID(){
  return this->contID;
}


Manager * Component::GetManager (){
  return this->manager;
}


std::string Component::GetComponentName(){
  return this->compName;
}


std::string Component::Print(){
  return "Component: " + name + "/" + std::to_string(id) + "; Container: "  + "/" + std::to_string(contID) + "; ";
}


void Component::SetComponentID(ComponentID compID){
  this->compID = compID;
}


void Component::SetContainerID(ContainerID contID){
  this->contID = contID;
}


void Component::SetManager(Manager * manager){
  this->manager = manager;
}


void Component::SubscribeHelper(){
  manager->Subscribe(message, sub, contName);
}

void SubscribeHelper(Subscription sub, std::string msgIdentifier, std::string contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}

void SubscribeHelper(Subscription sub, MessageID msgIdentifier, std::string contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}

void SubscribeHelper(Subscription sub, std::string msgIdentifier, ContainerID contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}

void SubscribeHelper(Subscription sub, MessageID msgIdentifier, ContainerID contIdentifier){
  manager->Subscribe(sub, msgIdentifier, contIdentifier);
}


void PublishHelper(std::string msgIdentifier, std::string contIdentifier){
  manager->Publish(msgIdentifier, contIdentifier);
}
void PublishHelper(MessageID msgIdentifier, std::string contIdentifier){
  manager->Publish(msgIdentifier, contIdentifier);
}
void PublishHelper(std::string msgIdentifier, ContainerID contIdentifier){
  manager->Publish(msgIdentifier, contIdentifier);
}
void PublishHelper(MessageID msgIdentifier, ContainerID contIdentifier){
  manager->Publish(msgIdentifier, contIdentifier);
}
