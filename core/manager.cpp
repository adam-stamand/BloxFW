#include "manager.h"

using namespace bx;

Manager::Manager(std::string contName) : Container (contName){
  //this->SetParent(this);
  this->AddedToManager(this);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Manager Created", this->Print());
  #endif
}


int Manager::Unsubscribe(SubscriptionReceipt &rect){
  Container * cont = GetManagedContainer(rect.cont->GetGlobalID());
  if (cont == NULL){
    return -1;
  }
  return cont->RemoveSubscription(rect);
}

int Manager::RegisterContainer(Container * cont){
  ContainerItem item(cont);
  int rv = this->managedContainers.add(item, cont->GetName());
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Registration", this->Print());
    #endif
    return -1;
  }

  cont->SetGlobalID(item.id);
  return 0;
}


int Manager::DeregisterContainer(Container * cont){
  ContainerItem item;
  int rv = this->managedContainers.remove(item, cont->GetGlobalID());
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Deregistration", this->Print());
    #endif
    return -1;
  }
  return 0;
}
