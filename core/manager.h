#ifndef Manager_H
#define Manager_H

#include "container.h"
#include "component.h"
#include "../utils/labeled_box.h"
#include "../utils/types.h"
#include <assert.h>

namespace bx{



class Manager : public Container
{
public:
  Manager(std::string contName) : Container (contName){
    this->SetID(managedContainers.add(this, contName)); // Manager manages itself as well
    this->manager = this;
  }
  ~Manager(){}

  // Modify Containers
  template <typename T>
  Container * GetContainer(T contIdentifier);

  // Modify Containers
  template <typename T1, typename T2>
  Component * GetComponent(T1 parentIdentifier, T2 compIdentifier);

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
  template <typename T1, typename T2>
  void Subscribe(Subscription sub, T1 subIdentifier, T2 parentIdentifier);
  template <typename T>
  void Subscribe(Subscription sub, T subIdentifier){Subscribe(sub, subIdentifier, this->GetID());}

  template <typename T1, typename T2>
  void Publish(Message const & msg, T1 subIdentifier, T2 parentIdentifier);
  template <typename T>
  void Publish(Message const & msg, T subIdentifier){Publish(msg, subIdentifier, this->GetID());}


private:
  labeled_box<ContainerID, Container*> managedContainers;
};






// Modify Containers
template <typename T>
Container * Manager::GetContainer(T contIdentifier){
  Container * cont;
  this->managedContainers.at(cont, contIdentifier);
  return cont;
}

// Modify Containers
template <typename T1, typename T2>
Component * Manager::GetComponent(T1 parentIdentifier, T2 compIdentifier){
  Container * parentCont;
  Container * cont;
  this->managedContainers.at(parentCont, parentIdentifier);
  if (parentCont == NULL){
    return parentCont;
  }
  return parentCont->GetComponent(compIdentifier);
}


template <typename T>
ContainerID Manager::CreateContainer(T parentIdentifier, std::string contName){
  // Get reference to parent and new containers
  Container * parentCont;
  Container * newCont = new Container(contName);
  int rv = this->managedContainers.at(parentCont, parentIdentifier);

  if (rv == 0) assert(0); // parent doesn't exist
  // Add reference to manager and parent
  ContainerID contID = this->managedContainers.add(newCont, contName);
  newCont->SetID(contID);
  parentCont->AddContainer(newCont);

  // Return new container ID
  return newCont->GetID();
}


template <typename T1, typename T2>
void Manager::DestroyContainer(T1 contIdentifier, T2 parentIdentifier){
  // Get reference to parent
  Container* parentCont;
  int rv = this->managedContainers.at(parentCont, parentIdentifier);
  if (rv == 0) assert(0);

  // Remove reference from both parent and manager
  parentCont->RemoveContainer(contIdentifier);
  delete(managedContainers.remove(contIdentifier));
}



template <typename T1, typename T2>
void Manager::Subscribe(Subscription sub, T1 subIdentifier, T2 parentIdentifier){
  Container * cont;
  int rv = this->managedContainers.at(cont, parentIdentifier);
  if (rv == 0) assert(0);
  cont->AddSubscription(sub, subIdentifier);
}


template <typename T1, typename T2>
void Manager::Publish(Message const & msg, T1 subIdentifier, T2 parentIdentifier){
  Container * cont;
  int rv = this->managedContainers.at(cont, parentIdentifier);
  if (rv == 0) assert(0);
  cont->PublishMessageLocally(msg, subIdentifier);
}



template <typename T>
void Manager::InsertComponents(std::vector<Component*> comps, T parentIdentifier){
  Container* cont;
  int rv = this->managedContainers.at(cont, parentIdentifier);
  if (rv == 0) assert(0);
  cont->AddComponents({comps});
}


template <typename T1, typename T2>
void Manager::EraseComponent(T1 compIdentifier, T2 parentIdentifier){
  Container * parentCont;
  int rv = this->managedContainers.at(parentCont, parentIdentifier);
  if (rv == 0) assert(0);
  parentCont->RemoveComponent(compIdentifier);
}

}
#endif
