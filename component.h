
#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <string>
#include <functional>

class EntityManager;
class Component;



struct Message{
  Component * publisher;
  void * params;
};

typedef std::function<void(Message const &)> MessageFunction;
typedef uint32_t MessageID;
typedef uint32_t ComponentID;
typedef uint32_t EntityID;


struct Subscription{
  Component * subscriber;
  MessageFunction callback;
};


class Component
{

public:
  // Must give Component Name upon instantiation
  Component(std::string name) : name(name){}
  // Allow destructor to be overriden
  virtual ~Component(){};

  virtual void AddedToEntity() = 0;

  // Subscription and Publishing System
  template <class T>
  void SubscribeLocalMessage(std::string message, void (T::*f)(Message const &));
  void PublishLocalMessage(std::string message);
  template <class T>
  void SubscribeGlobalMessage(std::string message, void (T::*f)(Message const &));
  void PublishGlobalMessage(std::string message);

  // Getters/Setters
  std::string GetName();
  ComponentID GetID();
  void Print();

  // Give Entity Access to private members
  friend class Entity;

private:
  // Setters to be used by Entity only
  void SetEntityID(EntityID id);
  void SetComponentID(ComponentID id);
  void SetManager(EntityManager * manager);

  // Message Helper
  void SubscribeLocalMessage(std::string message, MessageFunction function);  
  void SubscribeGlobalMessage(std::string message, MessageFunction function);

  // Private members
  EntityManager * manager = NULL;
  EntityID entityID = 0;
  std::string name;
  ComponentID id;
};

// Template Interface Method
template <class T>
void Component::SubscribeLocalMessage(std::string message, void (T::*f)(Message const &)){
  SubscribeLocalMessage(message, std::bind(f, (T*)(this), std::placeholders::_1));
}
// Template Interface Method
template <class T>
void Component::SubscribeGlobalMessage(std::string message, void (T::*f)(Message const &)){
  SubscribeGlobalMessage(message, std::bind(f, (T*)(this), std::placeholders::_1));
}

#endif
