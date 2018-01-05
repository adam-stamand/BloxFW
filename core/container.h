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

  // Modify Entities
  ContainerID AddContainer(Container * cont);

  //void RemoveContainer(ContainerID id);
  Container * RemoveContainer(std::string contName);

  // Modify Components within Entity
  void AddComponents(std::vector<Component*> comps);

  //void RemoveComponent(ComponentID);
  Component * RemoveComponent(std::string compName);



  // Publishing and Subscription System for Intra Entity communication
  void AddSubscription(std::string message, Subscription sub);
  void PublishMessageLocally(std::string message, Message const & msg);
  //void PublishMessageRecursively(std::string message, Message const & msg);
  void SetID(ContainerID id) {this->id = id;}

private:

  // Private Members
  ContainerManager * manager = NULL;
  ContainerID id;
  const std::string name;

  labeled_box<MessageID,std::vector<Subscription>> subscriptions;
  labeled_box<ContainerID,Container*> containers;
  labeled_box<ComponentID,Component*> components;

};


}

#endif
