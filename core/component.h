
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



class Component
{

public:
  Component(std::string compName) : name(compName){} // Must give Component Name upon instantiation
  virtual ~Component(){}; // Allow destructor to be overriden

  virtual void UserInit() = 0; // User must define behavior when component is added to Container

  // Getters
  std::string GetName();
  Container * GetParent();
  Manager * GetManager();
  std::string Print(); // for debug

  // Give Entity Access to private members
  //friend class Container; // consider friend functions


  // Subscription and Publishing System
  template <class T1, class T2, class T3>
  void SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 contIdentifier);
  template <class T1, class T2>
  void SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier) {SubscribeToContainerMessage(f, msgIdentifier, this->GetParentID());}

  template <class T1, class T2>
  void PublishMessageToContainer(Message &msg, T1 msgIdentifier, T2 contIdentifier);
  template <class T>
  void PublishMessageToContainer(Message &msg, T msgIdentifier){PublishMessageToContainer(msgIdentifier, this->GetParentID());}

  void Init();
  void Uninit();

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
  //void SetID(ComponentID compID);
  void SetParent(Container * cont);
  void SetManager(Manager * manager);
  //void SetInit(bool state);

  // Private members
  bool initialized    = false;
  Container * parent  = NULL;
  Manager * manager   = NULL;
  const std::string   name;
};



template <class T1, class T2, class T3>
void Component::SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 contIdentifier){
  if (!this->initialized){
    int rv;
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Component NOT Initialized", this->Print());
    #endif
    return;
  }
  assert(f != NULL);
  MessageFunction function = std::bind(f, (T1*)(this), std::placeholders::_1);
  Subscription sub;
  sub.subscriber = this;
  sub.callback = function;
  SubscribeHelper(sub, msgIdentifier, contIdentifier);
}


template <class T1, class T2>
void Component::PublishMessageToContainer(Message &msg, T1 msgIdentifier, T2 contIdentifier){
  if (!this->initialized){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Component NOT Initialized", this->Print());
    #endif
    return;
  }
  msg.publisher = this;
  PublishHelper(msg, msgIdentifier, contIdentifier);
}


}

#endif
