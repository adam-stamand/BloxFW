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
  for (auto iter = receipts.begin(); iter != receipts.end(); iter++){
    ReceiptElem elem;
    elem.id = iter->second;
    receipts.get(elem);
    manager->Unsubscribe(elem.data);
  }
  this->manager = NULL;
}



int Component::SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.cont = sub.cont;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}

int Component::SubscribeHelper(Subscription &sub, std::string msgIdentifier, ContainerID contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.cont = sub.cont;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}

int Component::SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, std::string contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.cont = sub.cont;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}




int Component::SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, ContainerID contIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, contIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.cont = sub.cont;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}


int Component::UnsubscribeContainerMessage(ReceiptID rectID){
  assert(this->manager != NULL);
  ReceiptElem elem;
  elem.id = rectID;
  receipts.get(elem);

  SubscriptionReceipt rect;
  rect = elem.data;
  return manager->Unsubscribe(rect);
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
