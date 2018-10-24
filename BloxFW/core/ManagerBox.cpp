#include "ManagerBox.h"

using namespace bx;



ManagerBox::ManagerBox(std::string boxName) : Box (boxName){
  this->AddToManager(this);
  printf("Still: manager %s\n", this->manager->GetName().c_str());
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "ManagerBox Created", this->Print());
  #endif
}



ManagerBox::~ManagerBox(){
  for (auto iter = boxes.begin(); iter != boxes.end(); iter++){
    Box * box = GetBox(iter->second);
    if (box == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Box Failed Destructor", this->Print());
      #endif
    }
    box->RemoveFromManager();
  }
}

int ManagerBox::Unsubscribe(SubscriptionReceipt &rect){
  Box * box = GetManagedBox(rect.box->GetGlobalID());
  if (box == NULL){
    return -1;
  }
  return box->RemoveSubscription(rect);
}

int ManagerBox::RegisterBox(Box * box){
  BoxItem item(box);
  printf("registering %s\n", box->GetName().c_str());
  int rv = this->managedBoxes.add(item, box->GetName());
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "ManagerBox Failed Registration", this->Print());
    #endif
    return -1;
  }

  box->SetGlobalID(item.id);
  return 0;
}


int ManagerBox::DeregisterBox(Box * box){
  BoxItem item;
  int rv = this->managedBoxes.remove(item, box->GetGlobalID());
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "ManagerBox Failed Deregistration", this->Print());
    #endif
    return -1;
  }
  return 0;
}
