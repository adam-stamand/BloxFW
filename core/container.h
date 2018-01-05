#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <stdint.h>
#include <map>
#include "component.h"
#include <assert.h>

#include "types.h"
#include "../data_structures/labeled_box.h"

namespace bx{


class Container
{
public:
  // Name Must be provided upon instantiation
  Container(std::string name): name(name){}
  // Allow destructor to be overriden
  virtual ~Container(){}


  // Getters
  //ComponentID GetComponentID(std::string name);
  ContainerID GetID();
  std::string GetName();
  ContainerID GetParentID(std::string name);
  Manager *   GetManager();
  std::string Print(); // for debug

  // Allow acces to private setters // TODO change to friend functions
  friend class Manager;

  // Modify Containers
  ContainerID AddContainer(Container * cont);
  Container * RemoveContainer(ContainerID contID);
  Container * RemoveContainer(std::string contName);

  // Modify Components within Container
  void AddComponents(std::vector<Component*> comps);
  Component * RemoveComponent(ComponentID compID);
  Component * RemoveComponent(std::string compName);

  // Publishing and Subscription System for Intra Entity communication
  template <typename T>
  SubscriptionID AddSubscription(Subscription sub, std::string msgName);
  template <typename T>
  SubscriptionID PublishMessageLocally(Message const & msg, T msgIdentifier);
  template <typename T>
  SubscriptionID PublishMessageRecursively(Message const & msg, T msgIdentifier);


private:

  // Setters to be used by Entity only
  void SetID(ComponentID compID);
  void SetParentID(ContainerID contID);
  void SetManager(Manager * manager);

  // Private Members
  Manager * manager = NULL;
  ContainerID id;
  ContainerID parentID;
  const std::string name;

  labeled_box<MessageID,std::vector<Subscription>> subscriptions;
  labeled_box<ContainerID,Container*> containers;
  labeled_box<ComponentID,Component*> components;
};



template <typename T>
SubscriptionID PublishMessageLocally(Message const & msg, T msgIdentifier){
  std::vector<Subscription> subs;
  SubscriptionID subID = subscriptions.GetItem(subs, msgIdentifier);
  if (subID != 0){
    for (unsigned int i = 0; i < subs.size(); i++){
      subs.at(i).callback(msg);
    }
  }
  return subID;
}


template <typename T>
SubscriptionID PublishMessageRecursively(Message const & msg, T msgIdentifier){
  // Not implemented yet
}

// Publishing and Subscription System for Intra Entity communication
template <typename T>
SubscriptionID Container::AddSubscription(Subscription sub, T msgIdentifier){
  std::vector<Subscription> subs;
  SubscriptionID subID = subscriptions.at(subs, msgIdentifier);
  if (subID == 0){
    subID = subscriptions.add({sub}, msgName);
  } else{
    subs.push_back(sub);
  }
  return subID;
}



}

#endif
