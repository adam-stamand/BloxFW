#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <stdint.h>
#include <map>
#include "component.h"
#include "container.h"



// Entities are never acted upon directly. Must use the entity manager
class Entity
{
public:
  // Allow access to EntityManager
  friend class EntityManager;

private:
  // Name Must be provided upon instantiation
  Entity(EntityManager * manager, std::string name): name(name){this->manager = manager;}
  // Allow destructor to be overriden
  virtual ~Entity(){}

  // Publishing and Subscription System for Intra Entity communication
  void SubscribeMessage(std::string message, Subscription sub);
  void PublishMessage(std::string message, Message const & msg);
  MessageID GetMessageID(std::string message);
  std::string GetMessageName(MessageID id);
  MessageID CreateMessageID(){return messageIDcounter++;}

  // Setters/Getters
  ComponentID GetComponentID(std::string name);
  std::string GetComponentName(ComponentID id);
  std::string GetName(){return name;}
  EntityID GetEntityID(){return id;}
  void SetID(EntityID id) {this->id = id;}

  // Modify Components within Entity
  void RemoveComponent(ComponentID);
  void RemoveComponent(std::string name);
  void AddComponents(std::vector<Component*> comps);

  // Private Members
  EntityManager * manager = NULL;
  MessageID messageIDcounter = 0;
  EntityID id;
  std::string name;
  std::map<std::string, ComponentID> componentNames;
  std::map<std::string, MessageID> messageIDmap;
  std::vector<std::vector<Subscription>> subscriptions;
  container<ComponentID, Component*> components;
};


#endif
