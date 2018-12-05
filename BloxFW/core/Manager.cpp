#include <BloxFW/core/Manager.h>

using namespace bx;



Manager::Manager(std::string boxName) : Box (boxName){
  // Manages itself
  this->manager = this;
  // Register manger's box with itself
  this->RegisterBox(this);
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Manager Created", this->Print());
  #endif
}



Manager::~Manager(){
  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = iter->second;
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed Destructor", this->Print());
      #endif
    }
    box->RemoveFromManager();
  }
}

int Manager::Unsubscribe(const SubscriptionReceipt &rect){
  Box * box = GetManagedBox(rect.box->GetName());
  if (box == NULL){
    return -1;
  }
  return box->RemoveSubscription(rect);
}

int Manager::RegisterBox(Box * box){
  BoxElem elem;
  elem.id = box->GetName();
  elem.data = box;
  int rv = this->managedBoxes.add(elem);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Registration", this->Print());
    #endif
    return -1;
  }
  return 0;
}


int Manager::DeregisterBox(Box * box){
  BoxElem elem;
  elem.id = box->GetName();
  int rv = this->managedBoxes.remove(elem);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Deregistration", this->Print());
    #endif
    return -1;
  }
  return 0;
}


// Modify Boxes
Box * Manager::GetManagedBox(std::string boxIdentifier){
  BoxElem elem;
  elem.id = boxIdentifier;
  int rv = this->managedBoxes.get(elem);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Get Box", this->Print());
    #endif
    return NULL;
  }
  return elem.data;
}


// Modify Boxes
Block * Manager::GetManagedBlock(std::string blockIdentifier, std::string boxIdentifier){
  Box *box = GetManagedBox(boxIdentifier);
  if (box == NULL){
    return NULL;
  }
  return box->GetBlock(blockIdentifier);
}


int Manager::Subscribe(Subscription &sub, std::string subIdentifier, std::string boxIdentifier){
  Box * box = GetManagedBox(boxIdentifier);
  if (box == NULL){
    return -1;
  }
  sub.box = box;
  return box->AddSubscription(sub, subIdentifier);
}




int Manager::Publish(Message & msg, std::string subIdentifier, std::string boxIdentifier){
  Box * box = GetManagedBox(boxIdentifier);
  if (box == NULL){
    return -1;
  }
  return box->PublishMessage(msg, subIdentifier);
}
