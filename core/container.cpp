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
  return "Container: " + this->GetName() + "; Parent Container: " + (this->GetParent()->GetName());
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
  int rv = 0;

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
  //TODO Remove all subscriptions to container? --Yes
  for (auto iter = subscriptions.begin(); iter != subscriptions.end(); iter++){
    MessageItem item;
    item.id = iter->second;
    rv = subscriptions.remove(item, item.id); //TODO check rv
    //delete(item.data);
  }

  this->manager->DeregisterContainer(this);
  this->manager = NULL;
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


int Container::AddSubscription(Subscription &sub, SubscriptionID subID){ // TODO does this need to exist
  MessageItem item;
  int rv = subscriptions.get(item, subID);
  if (rv != 0){
    DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
    return rv; //TODO
  }
  SubscriptionElem elem;
  elem.data = sub;
  item.data->add(elem);

  sub.subID = elem.id;
  sub.msgID = item.id;
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : "  + item.name);
  #endif
  return 0;
}


int Container::AddSubscription(Subscription &sub, std::string msgName){
  MessageItem item;
  int rv = subscriptions.get(item, msgName);
  if (rv != 0){
    item.data = new(box<SubscriptionID,Subscription>);
    item.name = msgName;
    rv = subscriptions.add(item);
    if (rv != 0){
      DebugLog(BLOX_ERROR, "Failed Add Subscription", this->Print());
      return rv; //TODO
    }
  }
  SubscriptionElem elem;
  elem.data = sub;
  item.data->add(elem);

  sub.subID = elem.id;
  sub.msgID = item.id;
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : "  + item.name);
  #endif
  return 0;
}



int Container::RemoveSubscription(SubscriptionReceipt &rect){
  MessageItem item;
  int rv = subscriptions.get(item, rect.msgID);
  if (rv != 0){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Subscription Failed Remove", this->GetName());
    #endif
    return -1;
  }
  if (item.data->size() <= 1){
    subscriptions.remove(item, item.id); // Remove entire box if only one elemnt left
    delete(item.data);
    return rv;
  }
  SubscriptionElem elem;
  elem.id = rect.subID;
  item.data->remove(elem); // TODO check return value
  return 0;
}
