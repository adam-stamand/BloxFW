#include "container.h"
#include "manager.h"

using namespace bx;


ContainerID Container::GetID(){
  return this->id;
}


void Container::SetID(ContainerID contID){
  this->id = contID;
}

Container * Container::GetParent(){
  return this->parent;
}

void Container::SetParent(Container * cont){
  this->parent = cont;
}


std::string Container::GetName() {
  return this->name;
}


Manager * Container::GetManager(){
  return this->manager;
}


std::string Container::Print(){
  //return "Container: " + this->GetName() + "/" + std::to_string(this->GetID())   + \
  //"; Parent Container: " + manager->GetContainer(this->GetParentID())->GetName() + \
  //"/" + std::to_string(this->GetParentID()) + "; ";
  return "hi";
}

int Container::AddedToManager(Manager * manager){
  assert(manager != NULL);
  this->manager = manager;
  this->manager->RegisterContainer(this);

  for (auto iter = containers.begin(); iter != containers.end(); iter++){
    Container * cont = GetContainer(iter->second);
    if (cont == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Container Failed Init", this->Print());
      #endif
      return -1;
    }
    cont->SetParent(this);
    cont->AddedToManager(manager);
  }

  for (auto iter = components.begin(); iter != components.end(); iter++){
    Component * comp = GetComponent(iter->second);
    if (comp == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Component Failed Init", this->Print());
      #endif
      return -1;
    }

    comp->SetParent(this);
    comp->AddedToManager(manager);
  }
  return 0;
}


int Container::RemovedFromManager(){
  this->manager->DeregisterContainer(this);
  this->manager = NULL;

  for (auto iter = containers.begin(); iter != containers.end(); iter++){
    Container * cont = GetContainer(iter->second);
    if (cont == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Container Failed UnInit", this->Print());
      #endif
      return -1;
    }
    cont->RemovedFromManager();
  }

  for (auto iter = components.begin(); iter != components.end(); iter++){
    Component * comp = GetComponent(iter->second);
    if (comp == NULL){
      #ifdef BLOX_DEBUG
      DebugLog(BLOX_ERROR, "Component Failed UnInit", this->Print());
      #endif
      return -1;
    }
    comp->RemovedFromManager();
  }
  return 0;

}



// Modify Entities
int Container::AddContainer(Container * cont){
  assert(cont != NULL);
  int rv;
  cont->SetParent(this);
  if (this->manager != NULL){
    rv = cont->AddedToManager(this->manager);
    if (rv != 0){
      return rv;
    }
  }

  ContainerItem item(cont, cont->GetName());
  rv = containers.add(item);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Add Container", this->Print());
    return rv;
  }

  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Added", cont->Print());
  #endif

  return 0;
}



// Modify Components within Entity
int Container::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    Component * comp = comps.at(i);
    assert(comp != NULL);
    int rv;
    comp->SetParent(this);
    if (this->manager != NULL){
      comp->AddedToManager(this->manager);
    }
    ComponentItem item(comp, comp->GetName());
    rv = components.add(item);
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Component", this->Print());
      return rv;
    }
    // TODO consider returning ID or assiging comp ID
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ACTIVITY, "Component Added", comp->Print());
    #endif

  }
  return 0;
}


int Container::AddSubscription(Subscription &sub, SubscriptionID subID){
  SubscriptionItem item;
  int rv = subscriptions.get(item, subID);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
    return rv; //TODO
  }

  item.data->push_back(sub);

  #ifdef BLOX_DEBUG
   //DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : " );//+ subscriptions.get_label(subID) );
  #endif
  return 0;
}


int Container::AddSubscription(Subscription &sub, std::string subName){
  SubscriptionItem item;
  int rv = subscriptions.get(item, subName);
  if (rv != 0){
    item.data = new(std::vector<Subscription>);
    item.name = subName;
    rv = subscriptions.add(item);
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
      return rv; //TODO
    }
  }

  item.data->push_back(sub);

  #ifdef BLOX_DEBUG
   //DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : " );//+ subscriptions.get_label(subID) );
  #endif
  return 0;
}
