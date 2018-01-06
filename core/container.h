#ifndef CONTAINER_H
#define CONTAINER_H

#include <vector>
#include <stdint.h>
#include <map>
#include "component.h"
#include <assert.h>

#include "../utils/types.h"
#include "../utils/labeled_box.h"
#include "../tools/debug.h"

namespace bx{


class Container
{
public:
  // Name Must be provided upon instantiation
  Container(std::string name): name(name){}
  // Allow destructor to be overriden
  virtual ~Container(){}

  void AddedToManager();

  // Getters
  //ComponentID GetComponentID(std::string name);
  ContainerID GetID();
  std::string GetName();
  ContainerID GetParentID();
  Manager *   GetManager();
  std::string Print(); // for debug

  // Allow acces to private setters // TODO change to friend functions
  friend class Manager;
  friend class Container;


  // Modify Components within Container
  void AddComponents(std::vector<Component*> comps);
  Component * GetComponent(ComponentID compID);
  Component * GetComponent(std::string compName);
  Component * RemoveComponent(ComponentID compID);
  Component * RemoveComponent(std::string compName);

  ContainerID AddContainer(Container * cont);
  Container * GetContainer(ContainerID contID);
  Container * GetContainer(std::string contName);
  //Container * RemoveContainer(ContainerID contID); // make templates out of these
  void RemoveContainer();

protected:
  // Modify Containers
  void InitComponent(Component * comp);


  // Publishing and Subscription System for Intra Entity communication
  template <typename T>
  void AddSubscription(Subscription sub, T msgName);
  template <typename T>
  void PublishMessageLocally(Message & msg, T msgIdentifier);
  template <typename T>
  void PublishMessageRecursively(Message  & msg, T msgIdentifier);

  // Setters to be used by Entity only
  void SetInit(bool state);
  void SetID(ComponentID compID);
  void SetParentID(ContainerID contID);
  void SetManager(Manager * manager); // consider passing in as initialization argument

  // Private Members
  bool initialized = false;
  Manager * manager = NULL;
  ContainerID id = 0;
  ContainerID parentID = 0;
  const std::string name;

  labeled_box<MessageID,std::vector<Subscription>*> subscriptions;
  labeled_box<ContainerID,Container*> containers;
  labeled_box<ComponentID,Component*> components;
};



template <typename T>
void Container::PublishMessageLocally(Message & msg, T subIdentifier){
  std::vector<Subscription> * subs;
  subs = subscriptions.at(subIdentifier);
  for (unsigned int i = 0; i < subs->size(); i++){
    subs->at(i).callback(msg);
  }
}


template <typename T>
void Container::PublishMessageRecursively(Message  & msg, T subIdentifier){

  for (unsigned int i = 0; i < containers.size(); i++){
    Container * cont = containers.at(i);
    cont->PublishMessageRecursively(msg, subIdentifier);
  }
  PublishMessageLocally(msg, subIdentifier);
}

// Publishing and Subscription System for Intra Entity communication
template <typename T>
void Container::AddSubscription(Subscription sub, T subIdentifier){
  std::vector<Subscription> * subs;
  if (subscriptions.valid(subIdentifier)){
    subs = subscriptions.at(subIdentifier);
  }else{
    subs = new(std::vector<Subscription>);
    subscriptions.add(subs, subIdentifier);
  }
  subs->push_back(sub);

  #ifdef BLOX_DEBUG
   DebugLog(BLOX_ACTIVITY, "Subscription Added", sub.subscriber->Print() + " subscribed to " + this->Print() + " : " );//+ subscriptions.get_label(subID) );
  #endif
}
}


#endif
