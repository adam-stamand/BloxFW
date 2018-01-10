#include "manager.h"

using namespace bx;

Manager::Manager(std::string contName) : Container (contName){
  this->SetParent(this);
  this->AddedToManager(this);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Manager Created", contName);
  #endif
}


int Manager::RegisterContainer(Container * cont){
  ContainerItem item(cont, cont->GetName());
  int rv = this->managedContainers.add(item);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Registration", this->GetName());
    #endif
    return -1;
  }
  
  cont->SetID(item.id);
  return 0;
}


int Manager::DeregisterContainer(Container * cont){
  ContainerItem item;
  int rv = this->managedContainers.remove(item, cont->GetID());
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Deregistration", this->GetName());
    #endif
    return -1;
  }
  return 0;
}
