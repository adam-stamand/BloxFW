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
  Container(std::string name, Manager * manager): name(name){this->manager = manager;}
  // Allow destructor to be overriden
  virtual ~Container(){}


  // Getters
  //ComponentID GetComponentID(std::string name);
  ContainerID GetID();
  std::string GetName();
  ContainerID GetParentID(std::string name);
  ContianerManager * GetManager();
  std::string Print(); // for debug

  // Allow acces to private setters // TODO change to friend functions
  friend class Manager;

  // Modify Containers
  ContainerID AddContainer(Container * cont);
  Container * RemoveContainer(ContainerID id);
  Container * RemoveContainer(std::string contName);

  // Modify Components within Container
  void AddComponents(std::vector<Component*> comps);
  Component * RemoveComponent(ComponentID id);
  Component * RemoveComponent(std::string compName);

  // Publishing and Subscription System for Intra Entity communication
  SubscriptionID AddSubscription(std::string message, Subscription sub);
  unsigned int PublishMessageLocally(std::string message, Message const & msg);

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


}

#endif
