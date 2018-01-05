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
  Manager(std::string contName) : Container (contName){
    managedContainers.add(this, contName); // Manager manages itself as well
  }
  ~Manager(){}

  // Modify Containers
  template <typename T>
  ContainerID CreateContainer(T parentIdentifier, std::string contName);
  ContainerID CreateContainer(std::string contName) {return CreateContainer(this->GetID(), contName);}

  template <typename T1, typename T2>
  void DestroyContainer(T1 contIdentifier, T2 parentIdentifier);
  template <typename T>
  void DestroyContainer(T contIdentifier){DestroyContainer(contIdentifier, this->GetID());}

  // Modify Components
  template <typename T>
  void InsertComponents(std::vector<Component*> comps, T parentIdentifier);
  void InsertComponents(std::vector<Component*> comps){InsertComponents(comps, this->GetID());}

  template <typename T1, typename T2>   // Does not delete/free component
  void EraseComponent(T1 compIdentifier, T2 parentIdentifier);
  template <typename T>                 // Does not delete/free component
  void EraseComponent(T compIdentifier){EraseComponent(compIdentifier, this->GetID());}


  // Messaging Interface
  template <typename T>
  void Subscribe(Subscription sub, T parentIdentifier, std::string subName);
  void Subscribe(Subscription sub, std::string subName){Subscribe(sub, this->GetID(), subName);}

  template <typename T>
  void Publish(Message const & msg, T parent, std::string subName);
  void Publish(Message const & msg, std::string subName){Publish(msg, this->GetID(), subName);}


private:
  labeled_box<ContainerID, Container*> managedContainers;
};













template <typename T>
ContainerID Manager::CreateContainer(T parentIdentifier, std::string contName){
  // Get reference to parent and new containers
  Container * parentCont;
  Container * newCont = new Container(this, contName);
  int rv = this->managedContainers.GetItem(parentCont, parentIdentifier);
  if (rv != 0) assert(0); // parent doesn't exist

  // Add reference to manager and parent
  ContainerID contID = this->managedContainers.AddItem(newCont, contName);
  newCont->SetID(contID);
  parentCont->AddContainer(newCont);

  // Return new container ID
  return newCont->GetID();
}


template <typename T1, typename T2>
void Manager::DestroyContainer(T1 contIdentifier, T2 parentIdentifier){
  // Get reference to parent
  Container* parentCont;
  int rv = this->managedContainers.GetItem(parentCont, parentIdentifier);
  if (rv != 0) assert(0);

  // Remove reference from both parent and manager
  parentCont->RemoveContainer(contIdentifier);
  delete(managedContainers.RemoveItem(contIdentifier));
}



template <typename T>
void Manager::Subscribe(Subscription sub, T contIdentifier, std::string subName){
  Container * cont;
  int rv = this->managedContainers.GetItem(cont, contIdentifier);
  if (rv != 0) assert(0);
  cont->AddSubscription(subName, sub);
}


template <typename T>
void Manager::Publish(Message const & msg, T contIdentifier, std::string subName){
  Container * cont;
  int rv = this->managedContainers.GetItem(cont, contIdentifier);
  if (rv != 0) assert(0);
  cont->PublishMessageLocally(subName, msg);
}



template <typename T>
void Manager::InsertComponents(std::vector<Component*> comps, T contIdentifier){
  Container* cont;
  int rv = this->managedContainers.GetItem(cont, contIdentifier);
  if (rv != 0) assert(0);
  cont->AddComponents({comps});
}


template <typename T1, typename T2>
void Manager::EraseComponent(T1 compIdentifier, T2 contIdentifier){
  Container * parentCont;
  int rv = this->managedContainers.GetItem(parentCont, contIdentifier);
  if (rv != 0) assert(0);
  parentCont->RemoveComponent(compIdentifier);
}

}
#endif
