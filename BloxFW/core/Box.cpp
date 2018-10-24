#include "Box.h"
#include "ManagerBox.h"

using namespace bx;


BoxID Box::GetGlobalID(){
  return this->globalID;
}

BoxID Box::GetLocalID(){
  return this->localID;
}

BoxID Box::GetID(){
  return this->GetGlobalID();
}


Box::~Box(){
  if (this->GetBox() != NULL){
    this->GetBox()->RemoveBox(this->GetLocalID());
  }
  for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
    Block * block = GetBlock(iter->second);
    if (block == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Block Failed Destructor", this->Print());
      #endif
    }
    block->SetBox(NULL);
  }

  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = GetBox(iter->second);
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed Destructor", this->Print());
      #endif
    }
    box->SetBox(NULL);
  }
  #ifdef BLOX_DEBUG
  puts("box deleted");
  //DebugLog(BLOX_ACTIVITY, "Cont Deleted", this->Print()); //TODO Long string in destructor crashes with seg fault
  #endif
}


void Box::SetGlobalID(BoxID boxID){
  this->globalID = boxID;
}

void Box::SetLocalID(BoxID boxID){
  this->localID = boxID;
}

Box * Box::GetBox(){
  return this->box;
}

void Box::SetBox(Box * box){
  this->box = box;
}


std::string Box::GetName() {
  return this->name;
}


ManagerBox * Box::GetManagerBox(){
  return this->manager;
}


std::string Box::Print(){
  if (this->GetBox() != NULL){
    return "Box: " + this->GetName() + "; Parent Box: " + (this->GetBox()->GetName());
  }else{
    return "Box: " + this->GetName();
  }
}

int Box::AddToManager(ManagerBox * manager){
  assert(manager != NULL);
  this->manager = manager;
  this->manager->RegisterBox(this);
  printf("Addting to manager: manager %s\n", this->manager->GetName().c_str());

  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = GetBox(iter->second);
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed Init", this->Print());
      #endif
      return -1;
    }
    box->SetBox(this);
    box->AddToManager(manager);
  }

  for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
    Block * block = GetBlock(iter->second);
    if (block == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Block Failed Init", this->Print());
      #endif
      return -1;
    }

    block->SetBox(this);
    block->AddToManager(manager);
  }
  return 0;
}


int Box::RemoveFromManager(){
  int rv = 0;

  for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
    Block * block = GetBlock(iter->second);
    if (block == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Block Failed UnInit", this->Print());
      #endif
      return -1;
    }
    block->RemoveFromManager();
  }

  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = GetBox(iter->second);
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed UnInit", this->Print());
      #endif
      return -1;
    }
    box->RemoveFromManager();
  }

  for (auto iter = subscriptions.begin(); iter != subscriptions.end(); iter++){
    MessageItem item;
    item.id = iter->second;
    rv = subscriptions.remove(item, item.id); //TODO check rv
    delete(item.data);
  }

  this->manager->DeregisterBox(this);
  this->manager = NULL;
  return rv;

}



// Modify Entities
int Box::AddBox(Box * box){
  assert(box != NULL);
  int rv;
  box->SetBox(this);
  printf("adding box %s to %s %d\n", box->GetName().c_str(), this->GetName().c_str(), this->manager);
  if (this->manager != NULL){
    rv = box->AddToManager(this->manager);
    if (rv != 0){
      return rv;
    }
  }

  BoxItem item(box);
  rv = boxes.add(item, box->GetName());
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Add Box", this->Print());
    return rv;
  }

  box->SetLocalID(item.id);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Box Added", box->Print());
  #endif

  return 0;
}



// Modify Blocks within Entity
int Box::AddBlocks(std::vector<Block*> newBlocks){
  for (unsigned int i = 0; i < newBlocks.size(); i++){
    Block * block = newBlocks.at(i);
    assert(block != NULL);
    int rv;

    block->SetBox(this);
    if (this->manager != NULL){
      block->AddToManager(this->manager);
    }


    BlockItem item(block);
    rv = blocks.add(item, block->GetName());
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Block", this->Print());
      return rv;
    }
    // TODO consider returning ID or assiging block ID
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ACTIVITY, "Block Added", block->Print());
    #endif

  }
  return 0;
}


int Box::AddSubscription(Subscription &sub, SubscriptionID subID){ // TODO does this need to exist
  MessageItem item;
  int rv = subscriptions.get(item, subID);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
    return rv; //TODO
  }
  SubscriptionElem elem;
  elem.data = sub;
  item.data->add(elem);

  sub.subID = elem.id;
  sub.msgID = item.id;
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : "  + item.name);
  #endif
  return 0;
}


int Box::AddSubscription(Subscription &sub, std::string msgName){
  MessageItem item;
  int rv = subscriptions.get(item, msgName);
  if (rv != 0){
    item.data = new(container<SubscriptionID,Subscription>);
    rv = subscriptions.add(item, msgName);
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
      return rv; //TODO
    }
  }
  SubscriptionElem elem;
  elem.data = sub;
  item.data->add(elem);

  sub.subID = elem.id;
  sub.msgID = item.id;
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : "  + item.name);
  #endif
  return 0;
}



int Box::RemoveSubscription(SubscriptionReceipt &rect){
  MessageItem item;
  int rv = subscriptions.get(item, rect.msgID);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Subscription Failed Remove", this->Print());
    #endif
    return -1;
  }
  if (item.data->size() <= 1){
    subscriptions.remove(item, item.id); // Remove entire box if only one elemnt left
    delete(item.data);
    return rv;
  }
  SubscriptionElem elem;
  elem.id = rect.subID;
  item.data->remove(elem); // TODO check return value
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Removed", elem.data.subscriber->Print() + " removed from " + this->Print() + " : "  + item.name);
  #endif
  return 0;
}
