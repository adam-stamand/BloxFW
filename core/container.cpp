#include "container.h"

using namespace bx;

ContainerID Container::GetID(){return this->id;};
std::string Container::GetName() {return this->name;}
std::string Container::Print(){return "to be implemented";}
void Container::SetID(ContainerID id) {this->id = id;}
void Container::SetContainerID(ContainerID id) {this->contID = id;}

// Modify Entities
ContainerID Container::AddContainer(Container * cont){
  return containers.AddItem(cont, cont->GetName());
}

Container * Container::RemoveContainer(ContainerID id){
  return containers.RemoveItem(id);
}

Container * Container::RemoveContainer(std::string contName){
  return containers.RemoveItem(contName);
}



// Modify Components within Entity
void Container::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    Component * comp = comps.at(i);
    comp->SetContainerID(this->GetID());
    comp->SetManager(this->manager);
    components.AddItem(comp, comp->GetName());
  }
}

Component * Container::RemoveComponent(ComponentID id){
  return components.RemoveItem(id);
}

Component * Container::RemoveComponent(std::string compName){
  return components.RemoveItem(compName);
}



// Publishing and Subscription System for Intra Entity communication
SubscriptionID Container::AddSubscription(std::string message, Subscription sub){
  std::vector<Subscription> subs;
  int rv = subscriptions.GetItem(subs, message);
  if (rv != 0){
    return subscriptions.AddItem({sub}, message);
  }
  subs.push_back(sub);
  return 0; //TODO fix
}


unsigned int Container::PublishMessageLocally(std::string message, Message const & msg){
  std::vector<Subscription> subs;
  int rv = subscriptions.GetItem(subs, message);
  if (rv != 0){
    return 0; // No subcribers
  }
  for (unsigned int i = 0; i < subs.size(); i++){
    subs.at(i).callback(msg);
  }
  return subs.size(); //TODO fix
}
