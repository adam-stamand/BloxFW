#ifndef Manager_H
#define Manager_H

#include "container.h"
#include "component.h"
#include "../data_structures/labeled_box.h"
#include <assert.h>

namespace bx{



class Manager : public Container
{
public:
  Manager(std::string contName) : Container (contName, this){
    managedContainers.AddItem(this, contName); // Manager manages itself as well
  }
  ~Manager(){}

  // Modify Containers
  template <typename T>
  ContainerID CreateContainer(std::string newContainer, T parent);
  ContainerID CreateContainer(std::string newContainer) {return CreateContainer(newContainer, this->GetID());}

  template <typename T1, typename T2>
  void DestroyContainer(T1 newContainer, T2 parentContainer);
  template <typename T>
  void DestroyContainer(T newContainer){DestroyContainer(newContainer, this->GetID());}

  // Modify Components
  template <typename T>
  void InsertComponents(std::vector<Component*> comps, T parent);
  void InsertComponents(std::vector<Component*> comps){InsertComponents(comps, this->GetID());}

  template <typename T1, typename T2>   // Does not delete/free component
  void EraseComponent(T1 component, T2 parent);
  template <typename T>                 // Does not delete/free component
  void EraseComponent(T component){EraseComponent(component, this->GetID());}


  // Messaging Interface
  template <typename T>
  void Subscribe(std::string subName, Subscription sub, T parent);
  void Subscribe(std::string subName, Subscription sub){Subscribe(subName, sub, this->GetID());}

  template <typename T>
  void Publish(std::string subName, Message const & msg, T parent);
  void Publish(std::string subName, Message const & msg){Publish(subName, msg, this->GetID());}


private:
  labeled_box<ContainerID, Container*> managedContainers;
};













template <typename T>
ContainerID Manager::CreateContainer(std::string newContName, T parent){
  // Get reference to parent and new containers
  Container * parentCont;
  Container * newCont = new Container(newContName, this);
  int rv = this->managedContainers.GetItem(parentCont, parent);
  if (rv != 0) assert(0);

  // Add reference to manager and parent
  ContainerID contID = this->managedContainers.AddItem(newCont, newContName);
  newCont->SetID(contID);
  parentCont->AddContainer(newCont);

  // Return new container ID
  return newCont->GetID();
}


template <typename T1, typename T2>
void Manager::DestroyContainer(T1 container, T2 parent){
  // Get reference to parent
  Container* parentCont;
  int rv = this->managedContainers.GetItem(parentCont, parent);
  if (rv != 0) assert(0);

  // Remove reference from both parent and manager
  parentCont->RemoveContainer(container);
  delete(managedContainers.RemoveItem(container));
}



template <typename T>
void Manager::Subscribe(std::string subName, Subscription sub, T container){
  Container * cont;
  int rv = this->managedContainers.GetItem(cont, container);
  if (rv != 0) assert(0);
  cont->AddSubscription(subName, sub);
}


template <typename T>
void Manager::Publish(std::string subName, Message const & msg, T container){
  Container * cont;
  int rv = this->managedContainers.GetItem(cont, container);
  if (rv != 0) assert(0);
  cont->PublishMessageLocally(subName, msg);
}



template <typename T>
void Manager::InsertComponents(std::vector<Component*> comps, T container){
  Container* cont;
  int rv = this->managedContainers.GetItem(cont, container);
  if (rv != 0) assert(0);
  cont->AddComponents({comps});
}


template <typename T1, typename T2>
void Manager::EraseComponent(T1 component, T2 container){
  Container * parentCont;
  int rv = this->managedContainers.GetItem(parentCont, container);
  if (rv != 0) assert(0);
  parentCont->RemoveComponent(component);
}

}
#endif
