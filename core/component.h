
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
  virtual ~Component();

  // Getters
  std::string GetName();
  Container * GetParent();
  Manager * GetManager();
  std::string Print(); // for debug

  // Subscription and Publishing System //TODO how to return error value??
  template <class T1, class T2, class T3>
  ReceiptID SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 contIdentifier); //TODO shitty interface
  template <class T1, class T2>
  ReceiptID SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier) {return SubscribeToContainerMessage(f, msgIdentifier, this->GetParentID());}

  int UnsubscribeContainerMessage(ReceiptID rectID);

  template <class T1, class T2>
  int PublishMessageToContainer(Message &msg, T1 msgIdentifier, T2 contIdentifier);
  template <class T>
  int PublishMessageToContainer(Message &msg, T msgIdentifier){return PublishMessageToContainer(msgIdentifier, this->GetParentID());}


  friend class Container;



private:
  
  void RemovedFromManager();
  ComponentID GetParentID(); //TODO fix hack maybe?
  virtual void UserInit(){}; // User can define behavior when component is added to Manager
  void AddedToManager(Manager * manager);
  void SetParent(Container * cont);


  // Message Helpers //TODO FIx this shit
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, std::string contIdentifier);
  int SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, std::string contIdentifier);
  int SubscribeHelper(Subscription &sub, std::string msgIdentifier, ContainerID contIdentifier);
  int SubscribeHelper(Subscription &sub, SubscriptionID msgIdentifier, ContainerID contIdentifier);

  int PublishHelper(Message  & msg, std::string msgIdentifier, std::string contIdentifier);
  int PublishHelper(Message  & msg, SubscriptionID msgIdentifier, std::string contIdentifier);
  int PublishHelper(Message  & msg, std::string msgIdentifier, ContainerID contIdentifier);
  int PublishHelper(Message  & msg, SubscriptionID msgIdentifier, ContainerID contIdentifier);




  box<ReceiptID, SubscriptionReceipt> receipts;

  // Private members
  Container * parent  = NULL;
  Manager * manager   = NULL;
  const std::string   name;
};


template <class T1, class T2, class T3>
ReceiptID Component::SubscribeToContainerMessage(void (T1::*f)(Message &), T2 msgIdentifier, T3 contIdentifier){
  assert(this->manager != NULL);
  assert(f != NULL);
  Subscription sub;
  MessageFunction function = std::bind(f, (T1*)(this), std::placeholders::_1);
  sub.subscriber = this;
  sub.callback = function;
  return SubscribeHelper(sub, msgIdentifier, contIdentifier);
}



template <class T1, class T2>
int Component::PublishMessageToContainer(Message &msg, T1 msgIdentifier, T2 contIdentifier){
  if (this->manager == NULL){
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ERROR, "Publish Failed; No Manager", GetName());
    #endif
    return -1;
  }
  msg.publisher = this;
  return PublishHelper(msg, msgIdentifier, contIdentifier);
}


}

#endif
