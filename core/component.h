
#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <string>
#include <functional>
#include "types.h"



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
  ContainerID GetID();
  ContianerManager * GetManager();
  std::string Print(); // for debug

  // Give Entity Access to private members
  friend class Container; // consider friend functions


  // Subscription and Publishing System
  template <class T1 class T2, class T3>
  void SubscribeToContainerMessage(void (T1::*f)(Message const &), T2 msgIdentifier, T3 contIdentifier);
  template <class T1 class T2>
  void SubscribeToContainerMessage(void (T1::*f)(Message const &), T2 msgIdentifier) {SubscribeToContainerMessage(f, msgIdentifier, this->GetContainerID());}

  template <class T1, class T2>
  void PublishMessageToContainer(T1 msgIdentifier, T2 contIdentifier);
  template <class T>
  void PublishMessageToContainer(T msgIdentifier){PublishMessageToContainer(msgIdentifier, this->GetContainerID());}


private:
  // Message Helpers
  void SubscribeHelper(Subscription sub, std::string msgIdentifier, std::string contIdentifier);
  void SubscribeHelper(Subscription sub, MessageID msgIdentifier, std::string contIdentifier);
  void SubscribeHelper(Subscription sub, std::string msgIdentifier, ContainerID contIdentifier);
  void SubscribeHelper(Subscription sub, MessageID msgIdentifier, ContainerID contIdentifier);


  void PublishHelper(std::string msgIdentifier, std::string contIdentifier);
  void PublishHelper(MessageID msgIdentifier, std::string contIdentifier);
  void PublishHelper(std::string msgIdentifier, ContainerID contIdentifier);
  void PublishHelper(MessageID msgIdentifier, ContainerID contIdentifier);

  // Setters to be used by Container only
  void SetComponentID(ComponentID compID);
  void SetContainerID(ContainerID contID);
  void SetManager(Manager * manager);

  // Private members
  ComponentID id;
  ContainerID parentID = 0;
  Manager * manager = NULL;
  const std::string name;
};



template <class T1 class T2, class T3>
void SubscribeToContainerMessage(void (T1::*f)(Message const &), T2 msgIdentifier, T3 contIdentifier){
  MessageFunction function = std::bind(f, (T*)(this), std::placeholders::_1);
  Subscription sub;
  sub.subscriber = this;
  sub.callback = function;
  SubscribeHelper(sub, msgIdentifier, contIdentifier);
}


template <class T1, class T2>
void PublishMessageToContainer(T1 msgIdentifier, T2 contIdentifier){
  Message msg;
  msg.publisher = this;
  PublishHelper(message, msgIdentifier, contIdentifier);
}


}

#endif
