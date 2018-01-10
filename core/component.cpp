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
  for (auto iter = subList.begin(); iter != subList.end(); iter++){
    UnsubscribeContainerMessage(iter->first, iter->second); // TODO hadnle return value
  }
  this->manager = NULL;
}



int Component::SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  subList.push_back(std::pair<SubscriptionID, ContainerID>(sub.id,sub.subscribee->GetID()));
  return 0;
}


int Component::SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, std::string contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  subList.push_back(std::pair<SubscriptionID, ContainerID>(sub.id,sub.subscribee->GetID()));
  return 0;
}


int Component::SubscribeHelper(Subscription &sub, std::string msgIdentifier, ContainerID contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  subList.push_back(std::pair<SubscriptionID, ContainerID>(sub.id,sub.subscribee->GetID()));
  return 0;
}


int Component::SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, ContainerID contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  subList.push_back(std::pair<SubscriptionID, ContainerID>(sub.id,sub.subscribee->GetID()));
  return 0;
}

int Component::UnsubscribeHelper(std::string subIdentifier, std::string contIdentifier){
  return manager->Unsubscribe(subIdentifier, contIdentifier);
}
int Component::UnsubscribeHelper(std::string subIdentifier, ContainerID contIdentifier){
  return manager->Unsubscribe(subIdentifier, contIdentifier);
}
int Component::UnsubscribeHelper(SubscriptionID subIdentifier, std::string contIdentifier){
  return manager->Unsubscribe(subIdentifier, contIdentifier);
}
int Component::UnsubscribeHelper(SubscriptionID subIdentifier, ContainerID contIdentifier){
  return manager->Unsubscribe(subIdentifier, contIdentifier);
}



int Component::PublishHelper(Message & msg, std::string msgIdentifier, std::string contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message  & msg, SubscriptionID msgIdentifier, std::string contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message  & msg, std::string msgIdentifier, ContainerID contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}


int Component::PublishHelper(Message & msg, SubscriptionID msgIdentifier, ContainerID contIdentifier){
  return manager->Publish(msg, msgIdentifier, contIdentifier);
}
