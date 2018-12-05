#include <BloxFW/core/Box.h>
#include <BloxFW/core/Manager.h>

using namespace bx;


Box::~Box(){
  if (this->GetBox() != NULL){
    // Remove self from parent box
    this->GetBox()->RemoveBox(this->GetName());
  }
  for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
    Block * block = iter->second;
    if (block == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Block Failed Destructor", this->Print());
      #endif
    }
    block->AddToBox(NULL);
  }

  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = iter->second;
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed Destructor", this->Print());
      #endif
    }
    box->AddToBox(NULL);
  }
  #ifdef BLOX_DEBUG
  puts("box deleted");
  //DebugLog(BLOX_ACTIVITY, "Cont Deleted", this->Print()); //TODO Long string in destructor crashes with seg fault
  #endif
}


Box * Box::GetBox(){
  return this->box;
}

void Box::AddToBox(Box * box){
  this->box = box;
}

std::string Box::GetBoxName(){
  assert(this->box != NULL); //TODO replace assert with BLOX_ASSERT
  return this->box->GetName();
}

std::string Box::GetName() {
  return this->name;
}


Manager * Box::GetManager(){
  return this->manager;
}


std::string Box::Print(){
  std::string box_str, man_str;
  if (this->GetBox() != NULL){
    box_str = this->GetBox()->GetName();
  }

  if (this->GetManager() != NULL){
    man_str = this->GetManager()->GetName();
  }

    return "Name: " + this->GetName() + "; Parent Box: " + box_str + "; Manager: " + man_str;
}

int Box::AddToManager(Manager * manager){
  assert(manager != NULL);
  this->manager = manager;
  this->manager->RegisterBox(this);
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Box +> Manager", this->Print());
  #endif

  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = iter->second;
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed Init", this->Print());
      #endif
      return -1;
    }
    box->AddToBox(this);
    box->AddToManager(manager);
  }

  for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
    Block * block = iter->second;
    if (block == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Block Failed Init", this->Print());
      #endif
      return -1;
    }

    block->AddToBox(this);
    block->AddToManager(manager);
  }
  return 0;
}


int Box::RemoveFromManager(){
  int rv = 0;

  for (auto iter = blocks.begin(); iter != blocks.end(); iter++){
    Block * block = iter->second;
    if (block == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Block Failed UnInit", this->Print());
      #endif
      return -1;
    }
    block->RemoveFromManager();
  }

  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = iter->second;
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed UnInit", this->Print());
      #endif
      return -1;
    }
    box->RemoveFromManager();
  }

  MessageElem elem;
  container<SubscriptionID,Subscription>* sub_map;
  for (auto iter = subscriptions.begin(); iter != subscriptions.end(); iter++){
    sub_map = iter->second;
    elem.id = iter->first;
    rv = subscriptions.remove(elem); //TODO check rv
    delete(sub_map);
  }

  this->manager->DeregisterBox(this);
  this->manager = NULL;
  return rv;

}



// Modify Entities
int Box::AddBox(Box * box){
  assert(box != NULL);
  int rv;
  box->AddToBox(this);
  BoxElem elem;
  elem.id = box->GetName();
  elem.data = box;
  rv = boxes.add(elem);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Add Box", this->Print());
    return rv;
  }


  if (this->manager != NULL){
    rv = box->AddToManager(this->manager);
    if (rv != 0){
      return rv;
    }
  }

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Box +> Box", box->Print());
  #endif
  return 0;
}



// Modify Blocks within Entity
int Box::AddBlocks(std::vector<Block*> newBlocks){
  for (unsigned int i = 0; i < newBlocks.size(); i++){
    Block * block = newBlocks.at(i);
    assert(block != NULL);
    int rv;

    block->AddToBox(this);
    BlockElem elem;
    elem.data = block;
    elem.id = block->GetName();
    rv = blocks.add(elem);
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Block", this->Print());
      return rv;
    }


    if (this->manager != NULL){
      block->AddToManager(this->manager);
    }
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ACTIVITY, "Block +> Box", block->Print());
    #endif
  }

  return 0;
}



int Box::AddSubscription(Subscription &sub, std::string msgName){
  static uint32_t subID_counter = 0; //TODO fix using U32 and wrapareound
  MessageElem elem;
  SubscriptionElem elem_sub;

  elem.id = msgName;
  int rv = subscriptions.get(elem);
  if (rv != 0){
    elem.data = new(container<SubscriptionID,Subscription>);
    rv = subscriptions.add(elem);
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
      return rv; //TODO
    }
  }

  elem_sub.data = sub;
  elem_sub.id = subID_counter;
  elem.data->add(elem_sub);
  sub.subID = subID_counter;
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : "  + elem.id);
  #endif

  subID_counter++;
  return 0;
}



int Box::RemoveSubscription(const SubscriptionReceipt &rect){
  MessageElem elem;
  SubscriptionElem elem_sub;

  elem.id = rect.msgName;
  int rv = subscriptions.get(elem);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Subscription Failed Remove", this->Print());
    #endif
    return -1;
  }

  elem_sub.id = rect.subID;
  elem.data->remove(elem_sub); // TODO check return value

  if (elem.data->size() <= 1){
    subscriptions.remove(elem); // Remove entire container if only one elemnt left
    delete(elem.data);
  }

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Removed", elem_sub.data.subscriber->Print() + " removed from " + this->Print() + " : "  + elem.id);
  #endif
  return 0;
}



Box * Box::GetBox(std::string boxIdentifier){
  BoxElem elem;
  elem.id = boxIdentifier;
  int rv = this->boxes.get(elem);
  if (rv != 0){
    return NULL;
  }
  return elem.data;
}

int Box::RemoveBox(std::string boxIdentifier){
  Box * box = GetBox(boxIdentifier);
  if (box == NULL){
    return -1;
  }
  if (this->manager != NULL){ // TODO consider added flag
    box->RemoveFromManager(); //TODO consider rv
  }
  
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Box Removed", box->Print());
  #endif
  box->AddToBox(NULL);


  BoxElem elem;
  elem.id = boxIdentifier;
  return this->boxes.remove(elem);
}

Block * Box::GetBlock(std::string blockIdentifier){
  BlockElem elem;
  elem.id = blockIdentifier;
  int rv = this->blocks.get(elem);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Get Block", this->Print());
    return NULL;
  }
  return elem.data;
}


int Box::RemoveBlock(std::string blockIdentifier){
  BlockElem elem;
  elem.id = blockIdentifier;
  int rv = this->blocks.remove(elem);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Block Failed Removed", elem.data->Print());
    #endif
    return rv;
  }
  if (elem.data == NULL){
    return -1;
  }

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Block Removed", elem.data->Print());
  #endif
  elem.data->AddToBox(NULL);


  if (this->manager != NULL){
    elem.data->RemoveFromManager();
  }

  return rv;
}



int Box::PublishMessage(Message & msg, std::string subIdentifier){
  MessageElem elem;
  elem.id = subIdentifier;
  int rv = subscriptions.get(elem);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Publish Failed; No Subscriptions", Print());
    #endif
    return rv;
  }

  container<SubscriptionID,Subscription>* subs = elem.data;

  for (auto iter = subs->begin(); iter!=subs->end(); iter++){
    iter->second.callback(msg);
  }
  return 0;
}
