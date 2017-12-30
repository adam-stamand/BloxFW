#include "entity.h"
#include <iostream>


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


void Entity::PublishMessage(MessageID id, Message const & msg){
  std::vector<Subscription> subs = subscriptions.at(id);

  for (unsigned int i = 0; i < subs.size(); i++){
    Subscription & subscription = subs.at(i);
    subscription.callback(msg);
  }
}


void Entity::SubscribeMessage(MessageID id, Subscription sub){
  #ifdef FW_DEBUG
  sub.subscriber->Print();
  std::cout << "Subscribed to " + GetMessageName(id) << std::endl;
  #endif
  subscriptions.at(id).push_back(sub);
}

void Entity::RemoveComponent(ComponentID id){
  delete(components.at(id));
}

void Entity::RemoveComponent(std::string name){
  ComponentID id = GetComponentID(name);
  delete(components.at(id));
}

ComponentID Entity::GetComponentID(std::string name){
  auto iter = componentNames.find(name);
  if (iter != componentNames.end()){
    return iter->second;
  }
  return 0;
}


void Entity::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    ComponentID id = components.push_back(comps.at(i));
    comps.at(i)->SetEntityID(this->id);
    comps.at(i)->SetManager(manager);
    componentNames.insert(std::pair<std::string,ComponentID>(comps.at(i)->GetName(), id));
    comps.at(i)->AddedToEntity();
  }
}
