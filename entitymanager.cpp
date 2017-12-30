#include "entitymanager.h"

MessageID EntityManager::GetMessageID(std::string message){
  auto iter = messageIDmap.find(message);
  MessageID id;
  if (iter != messageIDmap.end()){
    id = iter->second;
  }else{
    id = CreateMessageID();
    std::vector<Subscription> subs;
    subscriptions.push_back(subs);
    messageIDmap.insert(std::pair<std::string,MessageID>(message,id));
  }

  return id;
}


void EntityManager::PublishMessage(std::string message, Message msg, EntityID id){
  MessageID msgID = entities.at(id)->GetMessageID(message);
  std::vector<Subscription> subs = entities.at(id)->subscriptions.at(msgID);
  for (unsigned int i = 0; i < subs.size(); i++){
    subs.at(i).callback(msg);
  }
}


void EntityManager::SubscribeMessage(std::string message, Subscription sub, EntityID id){
  MessageID subID = entities.at(id)->GetMessageID(message);
  entities.at(id)->SubscribeMessage(subID, sub);
}


void EntityManager::PublishMessage(std::string message, Message msg){
  MessageID msgID = GetMessageID(message);
  std::vector<Subscription> subs = subscriptions.at(msgID);
  for (unsigned int i = 0; i < subs.size(); i++){
    subs.at(i).callback(msg);
  }
}


void EntityManager::SubscribeMessage(std::string message, Subscription sub){
  MessageID subID = GetMessageID(message);
  subscriptions.at(subID).push_back(sub);
}



EntityID EntityManager::CreateEntity(std::string name){
  Entity * entity = new Entity(this, name);
  EntityID id = entities.push_back(entity);
  entity->SetID(id);
  entityNames.insert(std::pair<std::string,EntityID>(name, id));
  return id;
}

EntityID EntityManager::GetEntityID(std::string name){
  auto iter = entityNames.find(name);
  if (iter != entityNames.end()){
    return iter->second;
  }
  return 0;
}

void EntityManager::RemoveEntity(EntityID id){
  delete(entities.at(id));
  entities.remove(id);
}

void EntityManager::RemoveEntity(std::string name){
  EntityID id = GetEntityID(name);
  delete(entities.at(id));
  entities.remove(id);
}


void EntityManager::AddComponents(EntityID id, std::vector<Component*> comps){
  entities.at(id)->AddComponents(comps);
}

void EntityManager::AddComponents(std::string entityName, std::vector<Component*> comps){
  entities.at(GetEntityID(entityName))->AddComponents(comps);
}

void EntityManager::RemoveComponent(EntityID entityID, ComponentID compID){
  entities.at(entityID)->RemoveComponent(compID);
}

void EntityManager::RemoveComponent(std::string entityName, ComponentID compID){
  entities.at(GetEntityID(entityName))->RemoveComponent(compID);
}

void EntityManager::RemoveComponent(EntityID entityID, std::string compName){
  entities.at(entityID)->RemoveComponent(compName);
}

void EntityManager::RemoveComponent(std::string entityName, std::string compName){
  entities.at(GetEntityID(entityName))->RemoveComponent(compName);
}
