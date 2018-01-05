
#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <string>
#include <functional>
//#include "core/message.h"

namespace bx{

class ContainerManager;
class Component;



typedef uint32_t MessageID;
typedef uint32_t ComponentID;
typedef uint32_t ContainerID;


struct Message{
  Component * publisher;
  void * params;
};
typedef std::function<void(Message const &)> MessageFunction;



class Component
{


public:







  // Must give Component Name upon instantiation
  Component(std::string name) : name(name){}
  // Allow destructor to be overriden
  virtual ~Component(){};

  virtual void AddedToContainer() = 0;

  // Subscription and Publishing System
  template <class T>
  void SubscribeToContainerMessage(std::string message, void (T::*f)(Message const &), std::string containerName);
  void PublishMessageToContainer(std::string message, std::string containerName);
  //void SubscribeToLocalMessage(std::string message, void (T::*f)(Message const &));
  //void PublishLocalMessage(std::string message);

  // Getters/Setters
  std::string GetName();
  ComponentID GetID();
  std::string Print(); // for debug

  // Give Entity Access to private members
  friend class Container; // consider friend functions

private:
  // Setters to be used by Entity only
  void SetContainerID(ContainerID id);
  void SetComponentID(ComponentID id);
  void SetManager(ContainerManager * manager);

  // Message Helper
  //void SubscribeLocalMessage(std::string message, MessageFunction function);
  void SubscribeToContainerMessage(std::string message, MessageFunction function, std::string containerName);
  //void SubscribeToMessage(std::string message, MessageFunction function);

  // Private members
  ComponentID id;
  ContainerID contID = 0;
  const std::string name;
  ContainerManager * manager = NULL;
};

// Template Interface Method
//template <class T>
//void Component::SubscribeToLocalMessage(std::string message, void (T::*f)(Message const &)){
//  SubscribeToMessage(message, std::bind(f, (T*)(this), std::placeholders::_1));
//}
// Template Interface Method
template <class T>
void Component::SubscribeToContainerMessage(std::string message, void (T::*f)(Message const &), std::string containerName){
  SubscribeToContainerMessage(message, std::bind(f, (T*)(this), std::placeholders::_1), containerName);
}


}

#endif
