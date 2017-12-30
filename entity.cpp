#include "entity.h"
#include <iostream>
#include "debug.h"


MessageID Entity::GetMessageID(std::string message){
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

std::string Entity::GetMessageName(MessageID id){
  auto iter = messageIDmap.begin();
  while (iter != messageIDmap.end()){
    if (iter->second == id){
      return iter->first;
    }
    iter++;
  }
  return "";
}


ComponentID Entity::GetComponentID(std::string name){
  auto iter = componentNames.find(name);
  if (iter != componentNames.end()){
    return iter->second;
  }
  return 0;
}

std::string Entity::GetComponentName(ComponentID id){
  auto iter = componentNames.begin();
  while (iter != componentNames.end()){
    if (iter->second == id){
      return iter->first;
    }
    iter++;
  }
  return "";
}

void Entity::PublishMessage(std::string message, Message const & msg){
  MessageID id = GetMessageID(message);
  std::vector<Subscription> subs = subscriptions.at(id);
  #ifdef BLOX_DEBUG
  std::stringstream temp;
  temp <<   "Published " + GetMessageName(id)  + "; "+ msg.publisher->Print() ;
  DebugLog(BLOX_ACTIVITY, temp.str());
  #endif

  for (unsigned int i = 0; i < subs.size(); i++){
    Subscription & subscription = subs.at(i);
    subscription.callback(msg);
  }
}


void Entity::SubscribeMessage(std::string message, Subscription sub){
  #ifdef BLOX_DEBUG
  std::stringstream temp;
  temp <<   "Subscribed to " + GetMessageName(id)  + "; "+ sub.subscriber->Print() ;
  DebugLog(BLOX_ACTIVITY, temp.str());
  #endif
  MessageID id = GetMessageID(message);
  subscriptions.at(id).push_back(sub);
}

void Entity::RemoveComponent(ComponentID id){
  #ifdef BLOX_DEBUG
  std::stringstream temp;
  temp << "Removing " + components.at(id)->Print() + GetMessageName(id);
  DebugLog(BLOX_ACTIVITY, temp.str());
  #endif
  delete(components.at(id));
}

void Entity::RemoveComponent(std::string name){
  #ifdef BLOX_DEBUG
  std::stringstream temp;
  temp << "Removing " + components.at(id)->Print() + GetMessageName(id);
  DebugLog(BLOX_ACTIVITY, temp.str());
  #endif
  ComponentID id = GetComponentID(name);
  delete(components.at(id));
}




void Entity::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    ComponentID id = components.push_back(comps.at(i));
    comps.at(i)->SetEntityID(this->id);
    comps.at(i)->SetManager(manager);
    componentNames.insert(std::pair<std::string,ComponentID>(comps.at(i)->GetName(), id));
    comps.at(i)->AddedToEntity();
    #ifdef BLOX_DEBUG
    std::stringstream temp;
    temp << "Added " + comps.at(id)->Print() + GetMessageName(id);
    DebugLog(BLOX_ACTIVITY, temp.str());
    #endif
  }
}
