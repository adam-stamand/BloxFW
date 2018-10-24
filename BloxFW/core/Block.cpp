#include "Block.h"
#include "ManagerBox.h"
#include <stdio.h>
#include <iostream>


using namespace bx;

Block::~Block(){
  if (this->GetBox() != NULL){
    this->GetBox()->RemoveBlock(GetName()); //TODO consider adding component ID for this
  }
  #ifdef BLOX_DEBUG
  puts("box deleted");
  //DebugLog(BLOX_ACTIVITY, "Block Deleted", Print()); //TODO long prints in desctructor cause seg fault
  #endif
} // Allow destructor to be overriden


void Block::SetBox(Box * box){
  this->box = box;
}

BlockID Block::GetBoxID(){
  return this->box->GetID();
}

Box * Block::GetBox(){
  return this->box;
}


ManagerBox * Block::GetManagerBox (){
  return this->manager;
}


std::string Block::GetName(){
  return this->name;
}


std::string Block::Print(){
  if (this->GetBox() != NULL){
    return "Block: " + this->GetName(); //TODO wtf??
    return "Block: " + this->GetName() + "; Box: " + this->GetBox()->GetName();
  }else{
    return "Block: " + this->GetName();
  }
}


void Block::AddToManager(ManagerBox * manager){
  assert(manager != NULL);
  this->manager = manager;
  this->Init();
}


void Block::RemoveFromManager(){
  for (auto iter = receipts.begin(); iter != receipts.end(); iter++){
    ReceiptElem elem;
    elem.id = iter->second;
    receipts.get(elem);
    manager->Unsubscribe(elem.data);
  }
  this->manager = NULL;
}



int Block::SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string boxIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, boxIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.box = sub.box;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}

int Block::SubscribeHelper(Subscription &sub, std::string msgIdentifier, BoxID boxIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, boxIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.box = sub.box;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}

int Block::SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, std::string boxIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, boxIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.box = sub.box;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}




int Block::SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, BoxID boxIdentifier){
  int rv = manager->Subscribe(sub, msgIdentifier, boxIdentifier);
  if (rv != 0){
    return -1;
  }
  SubscriptionReceipt rect;
  rect.subID = sub.subID;
  rect.msgID = sub.msgID;
  rect.box = sub.box;

  ReceiptElem elem;
  elem.data = rect;
  receipts.add(elem);
  return elem.id;
}


int Block::UnsubscribeMessage(ReceiptID rectID){
  assert(this->manager != NULL);
  ReceiptElem elem;
  elem.id = rectID;
  receipts.get(elem);

  SubscriptionReceipt rect;
  rect = elem.data;
  return manager->Unsubscribe(rect);
}



int Block::PublishHelper(Message & msg, std::string msgIdentifier, std::string boxIdentifier){
  return manager->Publish(msg, msgIdentifier, boxIdentifier);
}


int Block::PublishHelper(Message  & msg, SubscriptionID msgIdentifier, std::string boxIdentifier){
  return manager->Publish(msg, msgIdentifier, boxIdentifier);
}


int Block::PublishHelper(Message  & msg, std::string msgIdentifier, BoxID boxIdentifier){
  return manager->Publish(msg, msgIdentifier, boxIdentifier);
}


int Block::PublishHelper(Message & msg, SubscriptionID msgIdentifier, BoxID boxIdentifier){
  return manager->Publish(msg, msgIdentifier, boxIdentifier);
}
