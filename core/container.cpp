#include "container.h"


ContainerID GetID(){return this->id;};
std::string GetName() {return this->name;}
std::string Print(){return "to be implemented";}
void SetID(ContainerID id) {this->id = id;}
void SetContainerID(ContainerID id) {this->contID = id};)

// Modify Entities
ContainerID AddContainer(Container * cont){
  cont->SetContainerID = this->GetID();
  cont->SetManager = this->manager;
  return containers.AddItem(cont);
}

Container * RemoveContainer(ContainerID id){
  return container.RemoveItem(id);
}

Container * RemoveContainer(std::string contName){
  return container.RemoveItem(contName);
}



// Modify Components within Entity
void AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    Component * comp = comps.at(i);
    comp->SetContainerID = this->GetID();
    comp->SetManager = this->manager;
    components.AddItem(comp, comp->GetName());
  }
}

Component * RemoveComponent(ComponentID id){
  return components.RemoveItem(id);
}

Component * RemoveComponent(std::string compName){
  return components.RemoveItem(compName);
}



// Publishing and Subscription System for Intra Entity communication
SubscriptionID AddSubscription(std::string message, Subscription sub){
  std::vector<Subscription*> subs;
  int rv = subscriptions.GetItem(message, subs);
  if (rv != 0){
    return subscriptions.AddItem({sub}, message);
  }
  subs.push_back(sub);
  return 0; //TODO fix
}


unsigned int PublishMessageLocally(std::string message, Message const & msg){
  std::vector<Subscription*> subs;
  int rv = subscriptions.GetItem(message, subs);
  if (rv != 0){
    return 0 // No subcribers
  }
  for (unsigned int i = 0; i < subs.size(); i++){
    subs.at(i).callback(msg);
  }
  return subs.size(); //TODO fix
}
