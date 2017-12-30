#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "entity.h"
#include "container.h"





class EntityManager
{

public:
  EntityManager(){}
  ~EntityManager(){}

  // Subscription and Publishing System //TODO consider making friend functions
  void PublishMessage(std::string message, Message msg, EntityID id);
  void SubscribeMessage(std::string message, Subscription sub, EntityID id);
  void PublishMessage(std::string message, Message msg);
  void SubscribeMessage(std::string message, Subscription sub);
  MessageID GetMessageID(std::string message);
  MessageID CreateMessageID(){return messageIDcounter++;}

  // Get ID's
  EntityID GetEntityID(std::string name);
  ComponentID GetComponentID(std::string entityName, std::string compName);
  ComponentID GetComponentID(EntityID id, std::string compName);

  // Modify Entities
  EntityID CreateEntity(std::string name);
  void RemoveEntity(EntityID id);
  void RemoveEntity(std::string name);

  // Modify Components within selected entity
  void AddComponents(EntityID id, std::vector<Component*> comps);
  void AddComponents(std::string name, std::vector<Component*> comps);
  void RemoveComponent(EntityID entityID, ComponentID compID);
  void RemoveComponent(std::string entityName, ComponentID compID);
  void RemoveComponent(EntityID entityID, std::string compName);
  void RemoveComponent(std::string entityName, std::string compName);


private:
  MessageID messageIDcounter = 0;
  std::map<std::string , MessageID> messageIDmap;
  std::vector<std::vector<Subscription>> subscriptions;
  std::map<std::string,EntityID> entityNames;
  container<EntityID,Entity*> entities;
};


#endif
