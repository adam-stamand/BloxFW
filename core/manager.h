#ifndef Manager_H
#define Manager_H

#include "container.h"
#include "component.h"
#include "../utils/labeled_box.h"
#include "../utils/types.h"
#include <assert.h>
#include "../tools/debug.h"

namespace bx{



class Manager : public Container
{
public:
  Manager(std::string contName);
  ~Manager(){}

  // Modify Containers //TODO look into merging getting and removing from container and manager
  template <typename T>
  Container * GetManagedContainer(T contIdentifier);
  template <typename T>
  Container * RemoveManagedContainer(T contIdentifier);

  // Modify Containers
  template <typename T1, typename T2>
  Component * GetManagedComponent(T1 compIdentifier, T2 parentIdentifier);
  template <typename T1, typename T2>
  Component * RemoveManagedComponent(T1 compIdentifier, T2 parentIdentifier);





  // Messaging Interface // TODO Protext these as well
  template <typename T1, typename T2>
  int Subscribe(Subscription &sub, T1 subIdentifier, T2 parentIdentifier);
  template <typename T>
  int Subscribe(Subscription &sub, T subIdentifier){return Subscribe(sub, subIdentifier, this->GetID());}

  template <typename T1, typename T2>
  int Publish(Message & msg, T1 subIdentifier, T2 parentIdentifier);
  template <typename T>
  int Publish(Message  & msg, T subIdentifier){return Publish(msg, subIdentifier, this->GetID());}

  // TODO Protext these
  int RegisterContainer(Container * cont);
  int DeregisterContainer(Container * cont);

private:
  // Modify Containers
  labeled_box<ContainerID, Container*> managedContainers;
};


// Modify Containers
template <typename T>
Container * Manager::GetManagedContainer(T contIdentifier){
  ContainerItem item;
  int rv = this->managedContainers.get(item, contIdentifier);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Manager Failed Get Container", this->Print());
    #endif
    return NULL;
  }
  return item.data;
}


// Modify Containers
template <typename T>
Container * Manager::RemoveManagedContainer(T contIdentifier){
  Container * cont = GetManagedContainer(contIdentifier);
  if (cont == NULL){
    return cont;
  }
  return cont->GetParent()->RemoveContainer(cont->GetID());
}


// Modify Containers
template <typename T1, typename T2>
Component * Manager::GetManagedComponent(T1 compIdentifier, T2 parentIdentifier){
  Container *cont = GetManagedContainer(parentIdentifier);
  if (cont == NULL){
    return cont;
  }
  return cont->GetComponent(compIdentifier);
}


template <typename T1, typename T2>
int Manager::Subscribe(Subscription &sub, T1 subIdentifier, T2 parentIdentifier){
  Container * cont = GetManagedContainer(parentIdentifier);
  if (cont == NULL){
    return -1;
  }
  return cont->AddSubscription(sub, subIdentifier);
}


template <typename T1, typename T2>
int Manager::Publish(Message & msg, T1 subIdentifier, T2 parentIdentifier){
  Container * cont = GetManagedContainer(parentIdentifier);
  if (cont == NULL){
    return -1;
  }
  cont->PublishMessageLocally(msg, subIdentifier);
  return 0;
}


}
#endif
