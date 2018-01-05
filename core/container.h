#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <stdint.h>
#include <map>
#include "component.h"
#include "../data_structures/labeled_box.h"
#include <assert.h>

namespace bx{



struct Subscription{
  Component * subscriber;
  MessageFunction callback;
};


// Entities are never acted upon directly. Must use the entity manager
class Container
{
public:
  // Name Must be provided upon instantiation
  Container(std::string name, ContainerManager * manager): name(name){this->manager = manager;}
  // Allow destructor to be overriden
  virtual ~Container(){}

  ComponentID GetComponentID(std::string name);
  std::string GetComponentName(ComponentID id);
  ContainerID GetContainerID(std::string name);
  std::string GetContainerName(ContainerID id);
  ContainerID GetID();
  std::string GetName();
  std::string Print(); // for debug
  void SetID(ContainerID id);
  void SetContainerID(ContainerID id);

  // Modify Entities
  ContainerID AddContainer(Container * cont);

  Container * RemoveContainer(ContainerID id);
  Container * RemoveContainer(std::string contName);

  // Modify Components within Entity
  void AddComponents(std::vector<Component*> comps);

  Component * RemoveComponent(ComponentID id);
  Component * RemoveComponent(std::string compName);

  // Publishing and Subscription System for Intra Entity communication
  SubscriptionID AddSubscription(std::string message, Subscription sub);
  unsigned int PublishMessageLocally(std::string message, Message const & msg);

private:

  // Private Members
  ContainerManager * manager = NULL;
  ContainerID id;
  ContainerID contID;
  const std::string name;

  labeled_box<MessageID,std::vector<Subscription>> subscriptions;
  labeled_box<ContainerID,Container*> containers;
  labeled_box<ComponentID,Component*> components;

};


}

#endif
