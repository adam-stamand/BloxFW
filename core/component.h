
#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <string>
#include <functional>
#include <assert.h>
#include "../utils/types.h"



namespace bx{

class Manager;



class Component
{

public:
  // Must give Component Name upon instantiation
  Component(std::string compName) : name(compName){}
  // Allow destructor to be overriden
  virtual ~Component(){};

  virtual void AddedToContainer() = 0;

  // Getters
  ComponentID GetID();
  std::string GetName();
  ContainerID GetParentID();
  Manager * GetManager();
  std::string Print(); // for debug

  // Give Entity Access to private members
  friend class Container; // consider friend functions


  // Subscription and Publishing System
  template <class T1, class T2, class T3>
  void SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 contIdentifier);
  template <class T1, class T2>
  void SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier) {SubscribeToContainerMessage(f, msgIdentifier, this->GetParentID());}

  template <class T1, class T2>
  void PublishMessageToContainer(Message &msg, T1 msgIdentifier, T2 contIdentifier);
  template <class T>
  void PublishMessageToContainer(Message &msg, T msgIdentifier){PublishMessageToContainer(msgIdentifier, this->GetParentID());}


private:
  // Message Helpers
  void SubscribeHelper(Subscription sub, std::string msgIdentifier, std::string contIdentifier);
  void SubscribeHelper(Subscription sub, MessageID msgIdentifier, std::string contIdentifier);
  void SubscribeHelper(Subscription sub, std::string msgIdentifier, ContainerID contIdentifier);
  void SubscribeHelper(Subscription sub, MessageID msgIdentifier, ContainerID contIdentifier);


  void PublishHelper(Message & msg, std::string msgIdentifier, std::string contIdentifier);
  void PublishHelper(Message  & msg, MessageID msgIdentifier, std::string contIdentifier);
  void PublishHelper(Message  & msg, std::string msgIdentifier, ContainerID contIdentifier);
  void PublishHelper(Message  & msg, MessageID msgIdentifier, ContainerID contIdentifier);

  // Setters to be used by Container only
  void SetID(ComponentID compID);
  void SetParentID(ContainerID contID);
  void SetManager(Manager * manager);

  // Private members
  ComponentID id = 0;
  ContainerID parentID = 0;
  Manager * manager = NULL;
  const std::string name;
};



template <class T1, class T2, class T3>
void Component::SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 contIdentifier){
  assert(f != NULL);
  MessageFunction function = std::bind(f, (T1*)(this), std::placeholders::_1);
  Subscription sub;
  sub.subscriber = this;
  sub.callback = function;
  SubscribeHelper(sub, msgIdentifier, contIdentifier);
}


template <class T1, class T2>
void Component::PublishMessageToContainer(Message &msg, T1 msgIdentifier, T2 contIdentifier){
  msg.publisher = this;
  PublishHelper(msg, msgIdentifier, contIdentifier);
}


}

#endif
