#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <stdint.h>
#include <map>
#include "component.h"
#include <assert.h>

#include "../utils/types.h"
#include "../utils/labeled_box.h"
#include "../tools/debug.h"

namespace bx{

class Container
{
public:
  // Name Must be provided upon instantiation
  Container(std::string name): name(name){}
  // Allow destructor to be overriden
  virtual ~Container(){}

  // Getters
  ContainerID GetID(); //TODO look into how to handle ID's
  std::string GetName();
  Container * GetParent();
  Manager *   GetManager();
  std::string Print(); // for debug

  // Modify Components within Container
  int AddComponents(std::vector<Component*> comps); //TODO look into return value
  template <typename T>
  Component * GetComponent(T compIdentifier);
  template <typename T>
  int RemoveComponent(T compIdentifier);

  int AddContainer(Container * cont);
  template <typename T>
  Container * GetContainer(T contIdentifier);
  template <typename T>
  int RemoveContainer(T contIdentifier);




  friend class Manager; //TODO wtf
  // TODO make friend functions
  int AddedToManager(Manager * manager);
  int RemovedFromManager();
  void SetParent(Container * cont);
  void SetID(ContainerID contID);

protected:

  // Publishing and Subscription System for Intra Entity communication
  int AddSubscription(Subscription &sub, std::string subName);
  int AddSubscription(Subscription &sub, SubscriptionID subID);
  template <typename T>
  int PublishMessageLocally(Message & msg, T msgIdentifier);
  template <typename T>
  int PublishMessageRecursively(Message  & msg, T msgIdentifier);

  // Private Members
  Manager * manager = NULL;
  Container * parent = NULL;
  const std::string name;
  ContainerID id;

  labeled_box<MessageID,std::vector<Subscription>*> subscriptions;
  labeled_box<ContainerID,Container*> containers;
  labeled_box<ComponentID,Component*> components;
};



template <typename T>
Container * Container::GetContainer(T contIdentifier){
  ContainerItem item;
  int rv = this->containers.get(item, contIdentifier);
  if (rv != 0){
    return NULL;
  }
  return item.data;
}

template <typename T>
int Container::RemoveContainer(T contIdentifier){
  Container * cont = GetContainer(contIdentifier);
  if (cont == NULL){
    return NULL;
  }
  cont->SetParent(NULL);
  cont->RemovedFromManager();

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Removed", cont->Print());
  #endif

  return this->containers.remove(contIdentifier);
}

template <typename T>
Component * Container::GetComponent(T compIdentifier){
  ComponentItem item;
  int rv = this->components.get(item, compIdentifier);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Get Component", this->Print());
    return NULL;
  }
  return item.data;
}


template <typename T>
int Container::RemoveComponent(T compIdentifier){
  Component * comp = GetComponent(compIdentifier);
  if (comp == NULL){
    return -1;
  }

  comp->SetParent(NULL);
  comp->RemovedFromManager();
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Component Removed", comp->GetName());
  #endif

  ComponentItem item;
  return this->components.remove(item, compIdentifier);
}


template <typename T>
int Container::PublishMessageLocally(Message & msg, T subIdentifier){
  SubscriptionItem item;
  int rv = subscriptions.get(item, subIdentifier);
  if (rv != 0){
    return rv;
  }

  std::vector<Subscription> * subs = item.data;
  for (unsigned int i = 0; i < subs->size(); i++){
      subs->at(i).callback(msg);
  }
  return 0;
}


template <typename T>
int Container::PublishMessageRecursively(Message  & msg, T subIdentifier){
  int rv;
  Container * cont;
  for (auto iter = containers.begin(); iter != containers.end(); iter++){
    Container * cont = GetContainer(iter->second);
    rv |= cont->PublishMessageRecursively(msg, subIdentifier);
  }
  rv |= PublishMessageLocally(msg, subIdentifier);
  return rv;
}

}


#endif
