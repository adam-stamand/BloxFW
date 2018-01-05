#include "component.h"
#include "containermanager.h"
#include <stdio.h>
#include <iostream>


using namespace bx;

void Component::SubscribeToContainerMessage(std::string message, MessageFunction function, std::string contName){
  Subscription sub;
  sub.subscriber = this;
  sub.callback = function;
  manager->Subscribe(message, sub, contName);
}


void Component::PublishMessageToContainer(std::string message, std::string contName){
  Message msg;
  msg.publisher = this;
  manager->Publish(message, msg, contName);
}


std::string Component::GetName(){
  return name;
}


void Component::SetContainerID(ContainerID id){
  this->contID = id;
}


void Component::SetComponentID(ComponentID id){
  this->id = id;
}

void Component::SetManager(ContainerManager * manager){
  this->manager = manager;
}


std::string Component::Print(){
  return "Component: " + name + "/" + std::to_string(id) + "; Container: "  + "/" + std::to_string(contID) + "; ";
}
