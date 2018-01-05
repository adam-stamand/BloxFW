#include "container.h"
#include "manager.h"

using namespace bx;


ContainerID Container::GetID(){
  return this->id;
}


ContainerID Container::GetParentID(){
  return this->parentID;
}


std::string Container::GetName() {
  return this->name;
}


Manager * Container::GetManager(){
  return this->manager;
}


std::string Container::Print(){
  return "Container: " + this->GetName() + "/" + std::to_string(this->GetID())   + \
  "; Parent Container: " + manager->GetContainer(this->GetParentID())->GetName() + \
  "/" + std::to_string(this->GetParentID()) + "; ";
}


void Container::SetID(ContainerID contID) {
  assert(contID != 0);
  this->id = contID;
}


void Container::SetParentID(ContainerID contID) {
  assert(contID != 0);
  this->parentID = contID;
}


void Container::SetManager(Manager * manager) {
  assert(manager != NULL);
  this->manager = manager;
}


// Modify Entities
ContainerID Container::AddContainer(Container * cont){
  assert(cont != NULL);
  #ifdef BLOX_DEBUG
  puts("SDFSDFSD");
  #endif
  cont->SetParentID(this->GetID());
  cont->SetManager(this->manager);
  ContainerID contID = containers.add(cont, cont->GetName());
  cont->SetID(contID);
  #ifdef BLOX_DEBUG
  DebugLog(BLOX_ACTIVITY, "Container Added - " + cont->Print());
  #endif
  return contID;
}

Container * Container::GetContainer(ContainerID contID){
  assert(contID != 0);
  Container * cont = NULL;
  this->containers.at(cont, contID);
  return cont;
}

Container * Container::GetContainer(std::string contName){
  Container * cont = NULL;
  this->containers.at(cont, contName);
  return cont;
}


Container * Container::RemoveContainer(ContainerID contID){
  assert(contID != 0);
  return containers.remove(contID);
}


Container * Container::RemoveContainer(std::string contName){
  return containers.remove(contName);
}


// Modify Components within Entity
void Container::AddComponents(std::vector<Component*> comps){
  for (unsigned int i = 0; i < comps.size(); i++){
    Component * comp = comps.at(i);
    assert(comp != NULL);
    comp->SetParentID(this->GetID());
    comp->SetManager(this->manager);
    ComponentID compID = components.add(comp, comp->GetName());
    comp->SetID(compID);
    comp->AddedToContainer();
  }
}



Component * Container::GetComponent(ComponentID compID){
  Component * comp = NULL;
  this->components.at(comp, compID);
  return comp;
}

Component * Container::GetComponent(std::string compName){
  Component * comp = NULL;
  this->components.at(comp, compName);
  return comp;
}


Component * Container::RemoveComponent(ComponentID compID){
  assert(compID != 0);
  return components.remove(compID);
}


Component * Container::RemoveComponent(std::string compName){
  return components.remove(compName);
}
