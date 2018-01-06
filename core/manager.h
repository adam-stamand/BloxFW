#ifndef Manager_H
#define Manager_H

#include "container.h"
#include "component.h"
#include "../utils/labeled_box.h"
#include "../utils/types.h"
#include <assert.h>
#include "../tools/debug.h"

namespace bx{



class Manager : public Container
{
public:
  Manager(std::string contName) : Container (contName){
    this->SetID(managedContainers.add(this, contName)); // Manager manages itself as well
    this->SetParentID(this->GetID());
    this->manager = this;
    #ifdef BLOX_DEBUG
    DebugLog(BLOX_ACTIVITY, "Manager Created", contName + "/" + std::to_string(this->GetID()));
    #endif
  }
  ~Manager(){}

  // Modify Containers
  template <typename T>
  Container * GetContainer(T contIdentifier);

  // Modify Containers
  template <typename T1, typename T2>
  Component * GetComponent(T1 compIdentifier, T2 parentIdentifier);


  template <typename T1, typename T2>
  void DestroyContainer(T1 contIdentifier, T2 parentIdentifier);
  template <typename T>
  void DestroyContainer(T contIdentifier){DestroyContainer(contIdentifier, this->GetID());}



  // Messaging Interface
  template <typename T1, typename T2>
  void Subscribe(Subscription sub, T1 subIdentifier, T2 parentIdentifier);
  template <typename T>
  void Subscribe(Subscription sub, T subIdentifier){Subscribe(sub, subIdentifier, this->GetID());}

  template <typename T1, typename T2>
  void Publish(Message & msg, T1 subIdentifier, T2 parentIdentifier);
  template <typename T>
  void Publish(Message  & msg, T subIdentifier){Publish(msg, subIdentifier, this->GetID());}

  template <typename T>
  ContainerID RegisterContainer(Container * cont, T parentIdentifier);
  ContainerID RegisterContainer(Container * cont) {return RegisterContainer(cont, this->GetID());}

private:
  // Modify Containers

  labeled_box<ContainerID, Container*> managedContainers;
};






// Modify Containers
template <typename T>
Container * Manager::GetContainer(T contIdentifier){
  return  this->managedContainers.at(contIdentifier);
}

// Modify Containers
template <typename T1, typename T2>
Component * Manager::GetComponent(T1 compIdentifier, T2 parentIdentifier){
  Container * parentCont = this->managedContainers.at(parentIdentifier);
  return parentCont->GetComponent(compIdentifier);
}


template <typename T>
ContainerID Manager::RegisterContainer(Container * cont, T parentIdentifier){
  // Get reference to parent and new containers
  Container * parentCont = this->managedContainers.at(parentIdentifier);

  // Add reference to manager and parent
  ContainerID contID = this->managedContainers.add(cont, cont->GetName());
  cont->SetID(contID);
  cont->SetManager(this);
  cont->SetInit(true);
  //parentCont->AddContainer(cont);

  cont->AddedToManager();
  // Return new container ID
  return cont->GetID();
}


template <typename T1, typename T2>
void Manager::DestroyContainer(T1 contIdentifier, T2 parentIdentifier){
  // Get reference to parent
  Container* parentCont = this->managedContainers.at(parentIdentifier);
  this->managedContainers.remove(contIdentifier);

  // Remove reference from both parent and manager
  //parentCont->RemoveContainer(contIdentifier);
  parentCont->SetInit(false);
}



template <typename T1, typename T2>
void Manager::Subscribe(Subscription sub, T1 subIdentifier, T2 parentIdentifier){
  Container * cont = this->managedContainers.at(parentIdentifier);
  cont->AddSubscription(sub, subIdentifier);
}


template <typename T1, typename T2>
void Manager::Publish(Message & msg, T1 subIdentifier, T2 parentIdentifier){
  Container * cont = this->managedContainers.at(parentIdentifier);
  cont->PublishMessageLocally(msg, subIdentifier);
}


}
#endif
