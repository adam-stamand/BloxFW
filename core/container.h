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
  virtual ~Container(){
    if (this->manager != NULL){
      this->RemovedFromManager();
    }
    for (auto iter = components.begin(); iter != components.end(); iter++){
      Component * comp = GetComponent(iter->second);
      if (comp == NULL){
        #ifdef BLOX_DEBUG
        DebugLog(BLOX_ERROR, "Component Failed Destructor", this->Print());
        #endif
      }
      comp->SetParent(NULL);
    }

    for (auto iter = containers.begin(); iter != containers.end(); iter++){
      Container * cont = GetContainer(iter->second);
      if (cont == NULL){
        #ifdef BLOX_DEBUG
        DebugLog(BLOX_ERROR, "Container Failed Desctructor", this->Print());
        #endif
      }
      cont->SetParent(NULL);
    }

  }

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


  std::map<std::string, ComponentID>::const_iterator comp_begin(){return components.begin();}
  std::map<std::string, ComponentID>::const_iterator comp_end(){return components.end();}

  std::map<std::string, ContainerID>::const_iterator cont_begin(){return containers.begin();}
  std::map<std::string, ContainerID>::const_iterator cont_end(){return containers.end();}

  friend class Manager;

  int RemovedFromManager();
private:
  int AddedToManager(Manager * manager);
  void SetParent(Container * cont);
  void SetID(ContainerID contID);

  // Publishing and Subscription System for Intra Entity communication
  int AddSubscription(Subscription &sub, std::string subName);
  int AddSubscription(Subscription &sub, SubscriptionID subID);
  int RemoveSubscription(SubscriptionReceipt &rect);
  //template <typename T>
  //int RemoveSubscription(T subIdentifier);
  template <typename T>
  int PublishMessageLocally(Message & msg, T subIdentifier);
  template <typename T>
  int PublishMessageRecursively(Message  & msg, T subIdentifier);

  // Private Members
  Manager * manager = NULL;
  Container * parent = NULL;
  const std::string name;
  ContainerID id;
  labeled_box<MessageID,box<SubscriptionID,Subscription>*> subscriptions;
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

  cont->RemovedFromManager(); //TODO consider rv
  cont->SetParent(NULL);

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Removed", cont->GetName());
  #endif

  ContainerItem item;
  return this->containers.remove(item, contIdentifier);
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
  MessageItem item;
  int rv = subscriptions.get(item, subIdentifier);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Publish Failed; No Subscriptions", GetName());
    #endif
    return rv;
  }

  box<SubscriptionID,Subscription>* subs = item.data;

  for (auto iter = subs->begin(); iter!=subs->end(); iter++){
    SubscriptionElem elem;
    elem.id = iter->second;
    subs->get(elem);
    elem.data.callback(msg);
  }
  return 0;
}



}


#endif
