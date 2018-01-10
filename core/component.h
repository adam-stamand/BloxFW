
#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <string>
#include <functional>
#include <assert.h>
#include "../tools/debug.h"
#include "../utils/types.h"



namespace bx{

class Manager;
class Container;



class Component
{

public:
  Component(std::string compName) : name(compName){} // Must give Component Name upon instantiation
  virtual ~Component(){}; // Allow destructor to be overriden


  // Getters
  std::string GetName();
  Container * GetParent();
  Manager * GetManager();
  std::string Print(); // for debug

  // Subscription and Publishing System
  template <class T1, class T2, class T3>
  int SubscribeToContainerMessage(void (T1::*f)(Message &), T2 subIdentifier, T3 contIdentifier); //TODO shitty interface
  template <class T1, class T2>
  int SubscribeToContainerMessage(void (T1::*f)(Message &), T2 subIdentifier) {return SubscribeToContainerMessage(f, subIdentifier, this->GetParentID());}

  template <class T1, class T2>
  int UnsubscribeContainerMessage(T1 subIdentifier, T2 contIdentifier);
  template <class T>
  int UnsubscribeContainerMessage(T subIdentifier) {return UnsubscribeContainerMessage(subIdentifier, this->GetParentID());}

  template <class T1, class T2>
  int PublishMessageToContainer(Message &msg, T1 subIdentifier, T2 contIdentifier);
  template <class T>
  int PublishMessageToContainer(Message &msg, T subIdentifier){return PublishMessageToContainer(subIdentifier, this->GetParentID());}


  friend class Container;



private:
  virtual void UserInit(){}; // User can define behavior when component is added to Manager
  // Make friend functions //TODO
  void AddedToManager(Manager * manager);
  void RemovedFromManager();
  void SetParent(Container * cont);
  ComponentID GetParentID(); //TODO fix hack maybe?

  std::list<std::pair<SubscriptionID, ContainerID>> subList;

  // Message Helpers //TODO FIx this shit
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string contIdentifier);
  int SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, std::string contIdentifier);
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, ContainerID contIdentifier);
  int SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, ContainerID contIdentifier);

  int UnsubscribeHelper(std::string subIdentifier, std::string contIdentifier);
  int UnsubscribeHelper(std::string subIdentifier, ContainerID contIdentifier);
  int UnsubscribeHelper(SubscriptionID subIdentifier, std::string contIdentifier);
  int UnsubscribeHelper(SubscriptionID subIdentifier, ContainerID contIdentifier);

  int PublishHelper(Message  & msg, std::string msgIdentifier, std::string contIdentifier);
  int PublishHelper(Message  & msg, SubscriptionID msgIdentifier, std::string contIdentifier);
  int PublishHelper(Message  & msg, std::string msgIdentifier, ContainerID contIdentifier);
  int PublishHelper(Message  & msg, SubscriptionID msgIdentifier, ContainerID contIdentifier);

  // Private members
  Container * parent  = NULL;
  Manager * manager   = NULL;
  const std::string   name;
};


template <class T1, class T2, class T3>
int Component::SubscribeToContainerMessage(void (T1::*f)(Message &), T2 subIdentifier, T3 contIdentifier){
  assert(this->manager != NULL);
  assert(f != NULL);
  Subscription sub;
  MessageFunction function = std::bind(f, (T1*)(this), std::placeholders::_1);
  sub.subscriber = this;
  sub.callback = function;
  return SubscribeHelper(sub, subIdentifier, contIdentifier);
}

template <class T1, class T2>
int Component::UnsubscribeContainerMessage(T1 subIdentifier, T2 contIdentifier){
  assert(this->manager != NULL);
  return UnsubscribeHelper(subIdentifier, contIdentifier);
}


template <class T1, class T2>
int Component::PublishMessageToContainer(Message &msg, T1 subIdentifier, T2 contIdentifier){
  assert(this->manager != NULL);
  msg.publisher = this;
  return PublishHelper(msg, subIdentifier, contIdentifier);
}


}

#endif
