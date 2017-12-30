#include "component.h"
#include "entitymanager.h"
#include <stdio.h>
#include <iostream>

void Component::SubscribeLocalMessage(std::string message, MessageFunction function){
  Subscription sub;
  sub.subscriber = this;
  sub.callback = function;
  manager->SubscribeMessage(message, sub, entityID);
}


void Component::PublishLocalMessage(std::string message){
  Message msg;
  msg.publisher = this;
  manager->PublishMessage(message, msg, entityID);
}


void Component::SubscribeGlobalMessage(std::string message, MessageFunction function){
  Subscription sub;
  sub.subscriber = this;
  sub.callback = function;
  manager->SubscribeMessage(message, sub);
}


void Component::PublishGlobalMessage(std::string message){
  Message msg;
  msg.publisher = this;
  manager->PublishMessage(message, msg);
}


std::string Component::GetName(){
  return name;
}


void Component::SetEntityID(EntityID id){
  this->entityID = id;
}


void Component::SetComponentID(ComponentID id){
  this->id = id;
}


void Component::SetManager(EntityManager * manager){
  this->manager = manager;
}


void Component::Print(){
  std::cout << "Component Name: " + name + "; Component ID: " + std::to_string(id) + "; ";
}
